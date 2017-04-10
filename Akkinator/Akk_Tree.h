

//Tree

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <io.h>
#include "C:\Users\Δενθρ\Desktop\Projects\C Codeblocks Projects\NodeLib\MyNode.h"

const unsigned long TREE_LEFT_CANARY_VALUE = 0xEEDDFDD;
const unsigned long TREE_RIGHT_CANARY_VALUE = 0xABAFFFF;
const char* TREE_STANDART_DUMP_FILE = "TREE_DUMP_FILE.gv";
const unsigned int MAX_OUTPUT_BUF = 10000;
const int TREE_MAX_ELEM_STRING = 512;


#define tree_t(elem_type) tree_##elem_type##_t


enum tree_item_types { feature, character };


typedef struct tree_item_string_t
  {
    int item_type;
    char* elem;
    struct tree_item_string_t* parent;
    struct tree_item_string_t* left;
    struct tree_item_string_t* right;
  } tree_item_string_t;


typedef struct tree_string_t
  {
    unsigned long tree_left_canary;
    tree_item_string_t* root;
    unsigned long tree_right_canary;
  } tree_string_t;




char* file_get_buf(FILE* input_file)
  {
    assert(input_file);
    unsigned int file_length = filelength(fileno(input_file));
    char* input_buf = (char* ) calloc(file_length, sizeof(*input_buf));
    assert(input_buf);
    fread(input_buf, sizeof(char), file_length, input_file);
    return input_buf;
  }



int Tree_ctor(tree_string_t* Tree)
  {
    assert(Tree);
    Tree -> tree_left_canary = TREE_LEFT_CANARY_VALUE;
    Tree -> tree_right_canary = TREE_RIGHT_CANARY_VALUE;
    Tree -> root = NULL;
    return 0;
  }


unsigned int get_vital_string(char* input_buf, char* str, char trigger)
  {
    assert(input_buf);
    assert(str);
    unsigned int input_pos = 0;
    while ( *(input_buf + input_pos) != trigger ) *str++ = *(input_buf + input_pos++ );
    return input_pos + 1;
  }


tree_item_string_t* Tree_CreateFileElems(tree_string_t* Tree, tree_item_string_t* item_parent, char* input_buf, unsigned long* input_pos )
  {
    tree_item_string_t* new_item = ( tree_item_string_t* ) calloc(1, sizeof(*new_item));
    new_item -> elem = ( char* ) calloc( TREE_MAX_ELEM_STRING, sizeof(char));
    *input_pos += 2;
    assert(*(input_buf + *input_pos) == 'f' || *(input_buf + *input_pos) == 'c');
    new_item -> item_type = ( *(input_buf + *input_pos) == 'f') ? feature : character;
    *input_pos += 2;
    if ( *(input_buf + (*input_pos)++) == '\"' ) *input_pos += get_vital_string(input_buf + *input_pos, new_item -> elem, '\"');
    new_item -> parent = item_parent;
    *input_pos += 1;
    assert(*(input_buf + *input_pos) == 'N' || *(input_buf + *input_pos) == '{');
    if ( *(input_buf + *input_pos) == '{' ) new_item -> left = Tree_CreateFileElems(Tree, new_item, input_buf, input_pos);
    else new_item -> left = NULL;
    *input_pos += 2;
    assert(*(input_buf + *input_pos) == 'N' || *(input_buf + *input_pos) == '{');
    if ( *(input_buf + *input_pos) == '{' ) new_item -> right  = Tree_CreateFileElems(Tree, new_item, input_buf, input_pos);
    else new_item -> right = NULL;
    *input_pos += 2;
    assert( *(input_buf + *input_pos)  == '}');
    return new_item;
  }



int Tree_ReadFile(tree_string_t* Tree, char* input_file_name)
  {
    assert(Tree);
    assert(input_file_name);
    FILE* input_file = fopen(input_file_name, "r");
    assert(input_file);
    char* input_buf = file_get_buf(input_file);
    unsigned long input_pos = 0;
    if ( *input_buf == '{' && Tree -> root == NULL) Tree -> root = Tree_CreateFileElems(Tree, NULL, input_buf, &input_pos);
    else if ( Tree -> root != NULL ) return 1;
    Tree -> root -> parent = Tree -> root;
    free(input_buf); input_buf = NULL;
    fclose(input_file); input_file = NULL;
    return 0;
  }


void Tree_ListFileElems( tree_string_t* Tree, tree_item_string_t* item, char* output_buf, unsigned long* output_pos)
  {
    *output_pos += sprintf(output_buf + *output_pos, "{ %c \"%s\" ", ( item -> item_type == feature ) ? 'f' : 'c', item -> elem);
    if ( item -> left == NULL ) *(output_buf + (*output_pos)++ ) = 'N';
    else Tree_ListFileElems(Tree, item -> left, output_buf, output_pos);
    *(output_buf + (*output_pos) ) = ' '; (*output_pos)++;
    if ( item -> right == NULL ) *(output_buf + (*output_pos)++ ) = 'N';
    else Tree_ListFileElems(Tree, item -> right, output_buf, output_pos);
    *(output_buf + (*output_pos) ) = ' '; (*output_pos)++;
    *(output_buf + (*output_pos)++ ) = '}';
  }


int Tree_WriteFile(tree_string_t* Tree, char* output_file_name)
  {
    assert(Tree);
    assert(output_file_name);
    FILE* output_file = fopen(output_file_name, "w");
    assert(output_file);
    char* output_buf =  ( char* ) calloc(MAX_OUTPUT_BUF, sizeof(*output_buf));
    unsigned long output_pos = 0;
    if ( Tree -> root != NULL ) Tree_ListFileElems(Tree, Tree -> root, output_buf, &output_pos);
    fwrite(output_buf, sizeof(char), output_pos, output_file);
    fclose(output_file); output_file = NULL;
    free(output_buf); output_buf = NULL;
    return 0;
  }


void Tree_CreateDefNode(tree_t(string)* Tree, tree_item_string_t* This, node_t(int)* Node)
  {
    if ( This -> parent -> left == This ) Node_HeadPush(Node, 0);
    else Node_HeadPush(Node, 1);
    if (This -> parent != Tree -> root ) Tree_CreateDefNode(Tree, This -> parent, Node);
  }



int Tree_ElemDefinition(tree_string_t* Tree, tree_item_string_t* This, char* output_buf)
  {
    assert(Tree);
    assert(This);
    assert(output_buf);
    assert( This -> item_type == character);
    node_t(int) def_node;
    unsigned int output_pos = 0;
    tree_item_string_t* item = Tree -> root;
    Node_ctor(&def_node);
    Tree_CreateDefNode(Tree, This, &def_node);
    def_node.current = def_node.head;
    output_pos += sprintf(output_buf, "Character %s: ", This -> elem );
    while ( def_node.current != def_node.tail )
      {
        if ( def_node.current -> elem )
          {
            output_pos += sprintf(output_buf + output_pos, "%s, ",item -> elem);
            item = item -> right;
          }
        else
          {
            output_pos += sprintf(output_buf + output_pos, "not %s, ", item -> elem);
            item = item -> left;
          }
        Node_IncCurrent(&def_node);
      }
    if ( def_node.current -> elem ) sprintf(output_buf + output_pos, "%s.", item -> elem);
    else sprintf(output_buf + output_pos, "not %s.", item -> elem);
    Node_dtor(&def_node);
    return 0;
  }



tree_item_string_t* Tree_Search(tree_string_t* Tree, tree_item_string_t* Tree_item,  char* elem)
  {
    if ( Tree_item != NULL )
      {
        tree_item_string_t* item = NULL;
        if ( strncmp(Tree_item -> elem, elem, TREE_MAX_ELEM_STRING) == 0 ) return Tree_item;
        else if ( (item = Tree_Search(Tree, Tree_item -> left, elem))  == NULL ) return Tree_Search(Tree, Tree_item -> right, elem);
        else return item;
      }
    else return NULL;
  }


int Tree_ElemDefComp(tree_string_t* Tree, tree_item_string_t* t_item1, tree_item_string_t* t_item2, char* output_buf)
  {
    assert(Tree);
    assert(t_item1);
    assert(t_item2);
    assert(output_buf);
    assert( t_item1 -> item_type == character && t_item2 -> item_type == character );
    if ( t_item1 == t_item2 )
      {
        sprintf(output_buf, "Actually, it's one character");
        return 0;
      }
    node_t(int) Node1, Node2;
    unsigned int output_pos = 0;
    tree_item_string_t* item = Tree -> root;
    Node_ctor(&Node1); Node_ctor(&Node2);
    Tree_CreateDefNode(Tree, t_item1, &Node1);
    Tree_CreateDefNode(Tree, t_item2, &Node2);
    Node1.current = Node1.head; Node2.current = Node2.head;
    bool flag_match = true;
    output_pos += sprintf(output_buf, "%s and %s: ", t_item1 -> elem , t_item2 -> elem );
    while ( Node1.current -> elem == Node2.current -> elem && Node1.current != Node1.tail && Node2.current != Node2.tail )
      {
        flag_match = false;
        if ( Node1.current -> elem )
          {
            output_pos += sprintf(output_buf + output_pos, "%s, ",item -> elem);
            item = item -> right;
          }
        else
          {
            output_pos += sprintf(output_buf + output_pos, "not %s, ", item -> elem);
            item = item -> left;
          }
        Node_IncCurrent(&Node1);
        Node_IncCurrent(&Node2);
      }
    if ( flag_match ) output_pos += sprintf(output_buf + output_pos, "do not match at all\n");
    else output_pos += sprintf(output_buf + output_pos, "\n");
    tree_item_string_t* item1 = item;
    output_pos += sprintf(output_buf + output_pos, "But character %s: ", t_item1 -> elem);
    while ( Node1.current != Node1.tail )
      {
        if ( Node1.current -> elem )
          {
            output_pos += sprintf(output_buf + output_pos, "%s, ",item -> elem);
            item = item -> right;
          }
        else
          {
            output_pos += sprintf(output_buf + output_pos, "not %s, ", item -> elem);
            item = item -> left;
          }
        Node_IncCurrent(&Node1);
      }
    if ( Node1.current -> elem ) output_pos += sprintf(output_buf + output_pos, "%s.\n", item -> elem);
    else output_pos += sprintf(output_buf + output_pos, "not %s.\n", item -> elem);
    output_pos += sprintf(output_buf + output_pos, "And character %s: ", t_item2 -> elem);
    while ( Node2.current != Node2.tail )
      {
        if ( Node2.current -> elem )
          {
            output_pos += sprintf(output_buf + output_pos, "%s, ",item1 -> elem);
            item1 = item1 -> right;
          }
        else
          {
            output_pos += sprintf(output_buf + output_pos, "not %s, ", item1 -> elem);
            item1 = item1 -> left;
          }
        Node_IncCurrent(&Node2);
      }
    if ( Node2.current -> elem ) sprintf(output_buf + output_pos, "%s.", item1 -> elem);
    else sprintf(output_buf + output_pos, "not %s.", item1 -> elem);

    Node_dtor(&Node1); Node_dtor(&Node2);
    return 0;
  }





void tree_dt(tree_string_t* Tree, tree_item_string_t* tree_item)
  {
    if ( tree_item != NULL )
      {
        tree_dt(Tree, tree_item -> left );
        tree_dt(Tree, tree_item -> right);
        free(tree_item -> elem); tree_item -> elem = NULL;
        free(tree_item);
      }
  }


int Tree_sub_dtor(tree_string_t* Tree, tree_item_string_t* tree_item)
  {
    assert(Tree);
    assert(tree_item);
    if ( tree_item == Tree -> root ) Tree -> root = NULL;
    else if ( tree_item == tree_item -> parent -> left ) tree_item -> parent -> left = NULL;
    else if ( tree_item == tree_item -> parent -> right ) tree_item -> parent -> right = NULL;
    else return 1;
    tree_dt(Tree, tree_item);
    return 0;
  }



int Tree_dtor(tree_string_t* Tree)
  {
    assert(Tree);
    Tree -> tree_left_canary = Tree -> tree_right_canary = 0;
    if ( Tree -> root != NULL ) tree_dt(Tree, Tree -> root);
    Tree -> root = NULL;
    return 0;
  }


tree_item_string_t* Create_TreeItem( tree_item_string_t* This, char* elem, int item_type )
  {
    assert(This);
    assert(elem);
    tree_item_string_t* new_item = ( tree_item_string_t* ) calloc( 1, sizeof(*new_item));
    new_item -> left = new_item -> right = NULL;
    new_item -> elem = ( char* ) calloc(TREE_MAX_ELEM_STRING, sizeof(char));
    strncpy(new_item -> elem, elem, TREE_MAX_ELEM_STRING);
    new_item -> item_type = item_type;
    new_item -> parent = This;
    return new_item;
  }


int Tree_CreateRoot(tree_string_t* Tree, char* elem, int item_type)
  {
    assert(Tree);
    assert(elem);
    Tree -> root = ( tree_item_string_t* ) calloc(1, sizeof(tree_item_string_t));
    Tree -> root -> elem = ( char* ) calloc( TREE_MAX_ELEM_STRING, sizeof(char));
    strncpy(Tree -> root -> elem, elem, TREE_MAX_ELEM_STRING);
    Tree -> root -> left = Tree -> root -> right = NULL;
    Tree -> root -> item_type = item_type;
    Tree -> root -> parent = Tree -> root;
    return 0;
  }


int Tree_LeftPush(tree_string_t* Tree, tree_item_string_t* This, char* elem, int item_type)
  {
    assert(Tree);
    This -> left = Create_TreeItem(This, elem, item_type);
    return 0;
  }


int Tree_RightPush(tree_string_t* Tree, tree_item_string_t* This, char* elem, int item_type)
  {
    assert(Tree);
    This -> right = Create_TreeItem(This, elem, item_type);
    return 0;
  }



void Tree_MainDump(tree_string_t* Tree, FILE* dump_file, tree_item_string_t* item, unsigned int parent_num)
  {
    static unsigned int item_num = 0;
    if ( item != NULL )
      {
        fprintf(dump_file, "item%d[label = \"%s%s\"];\n", item_num, item -> elem, ( item -> item_type == feature ) ? " ?" : "");
        if ( item != Tree -> root ) fprintf(dump_file, "item%d -> item%d;\n", parent_num, item_num );
        unsigned int t = item_num++;
        Tree_MainDump(Tree, dump_file, item -> left, t);
        Tree_MainDump(Tree, dump_file, item -> right, t);
      }
  }


int Tree_Dump(tree_string_t* Tree, char* dump_file_name)
  {
    assert(Tree);
    assert(dump_file_name);
    FILE* dump_file = fopen(dump_file_name, "w");
    assert(dump_file);
    fprintf(dump_file, "digraph Tree_Dump {\n");
    Tree_MainDump(Tree, dump_file, Tree -> root, 0 );
    fprintf(dump_file, "}");
    fclose(dump_file); dump_file = NULL;
    return 0;
  }


