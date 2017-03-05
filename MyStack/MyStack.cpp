
// This is the source file of C++ library of stack created by Denis Los
// This version is from 05.03.2017 (v2.0)


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "MyStack.h"





const size_t STACK_DEFAULT_MAX_SIZE = 0xFFFF;
const size_t STACK_DEFAULT_ELEM_SIZE = sizeof(int);


#ifndef NDEBUG
  const unsigned char* STACK_DUMPSTER_NAME = "MyStack_dumpster.txt";
#endif




#ifndef STACK_ASSERT_OK()

#ifndef NDEBUG
  #define STACK_ASSERT_OK()          \
    verify();                        \
    if ( _errno )                    \
      {                              \
        fprintf(stderr, "The program has been terminated with error code %X\n"          \
                        "See more information in %s\n", _errno, STACK_DUMPSTER_NAME); \
        dump();                      \
        abort();                     \
      }                              
#else
  
  #define STACK_ASSERT_OK() ;

#endif

#endif








Stack :: Stack(size_t max_size, size_t elem_size):
    _max_size(max_size),
    _elem_size(elem_size),
    _size(0)
  {
    _data = calloc(max_size, elem_size);
    
    #ifndef NDEBUG
      _errno = 0;
      _LEFT_CANARY = LEFT_CANARY_VALUE;
      _RIGHT_CANARY = RIGHT_CANARY_VALUE;
      _hash_value = hash_function();
    #endif

    STACK_ASSERT_OK();
  }





Stack :: ~Stack()
  {
    _max_size = _size = _elem_size = 0;

    free(_data);
    _data = NULL;

    #ifndef NDEBUG
      _hash_value = _LEFT_CANARY = _RIGHT_CANARY = 0;
      _errno = STACK_IS_DESTROYED;
    #endif
  }




bool Stack :: push(void* elem)
  {
    assert(elem);
    assert(_size < _max_size);

    STACK_ASSERT_OK();

    for ( size_t i = 0; i < _elem_size; i++)
      *(unsigned char*)( _data + _size * _elem_size + i ) = *(unsigned char*)( elem + i );
    _size++;

    #ifndef NDEBUG
      _hash_value = hash_function();
    #endif
      
    STACK_ASSERT_OK();

    return true;    
  }





void* Stack :: pop()
  {
    assert(_size);
    STACK_ASSERT_OK();

    void* elem = _data + _elem_size * ( _size - 1 );
    _size--;

    #ifndef NDEBUG
      _hash_value = hash_function();
    #endif

    STACK_ASSERT_OK();

    return elem;
  }




size_t Stack :: size() const
  {
    STACK_ASSERT_OK();

    return _size;
  }



bool Stack :: isEmpty() const
  {
    STACK_ASSERT_OK();

    return !(_size);
  }  



size_t Stack :: maxSize() const
  {
    STACK_ASSERT_OK();

    return _max_size;
  } 



size_t Stack :: elemSize() const 
  {
    STACK_ASSERT_OK();

    return _elem_size;
  }  






#ifndef NDEBUG


int Stack :: getErrno() const
  {
    return _errno;
  }

size_t Stack :: hash_function() const
  {
    return _size * 3 + (size_t)_data + _elem_size * 5 + _max_size * 2 + 'K' + sizeof(size_t);
  }         



int Stack :: verify()
  {
    if ( !_data ) 
      _errno = ERROR_BROKEN_DATA_PTR;
    else if ( _LEFT_CANARY != LEFT_CANARY_VALUE )
      _errno = ERROR_LEFT_CANARY;
    else if ( _RIGHT_CANARY != RIGHT_CANARY_VALUE )
      _errno = ERROR_RIGHT_CANARY;
    else if ( _hash_value != hash_function() )
      _errno = ERROR_HASH_VALUE;
    else if ( !_elem_size )
      _errno = ERROR_ELEM_SIZE;
    else if ( !_max_size )
      _errno = ERROR_MAX_SIZE; 
    else if ( _size > _max_size )
      _errno = ERROR_STACK_SIZE;

    return _errno; 
  }



bool Stack :: dump(unsigned char* dumpster_name) const
  {
    assert(dumpster_name);

    FILE* dumpster = fopen(dumpster_name, "w");
    assert(dumpster);
    
    fprintf(dumpster, "This is the dump of MyStack\n\n\n"
                      "_LEFT_CANARY: %X \t\t\t expected: %X\n"
                      "_hash_value: %X \t\t\t expected: %X\n"
                      "_errno: %X\n"
                      "_size: %u\n"
                      "_data: %p\n"
                      "_elem_size: %u\n"
                      "_max_size: %u\n"
                      "_RIGHT_CANARY: %X \t\t\t expected: %X\n\n\n\n",
                      _LEFT_CANARY, LEFT_CANARY_VALUE, _hash_value, hash_function(),
                      _errno, _size, _data, _elem_size, _max_size, _RIGHT_CANARY, RIGHT_CANARY_VALUE);
    fflush(dumpster);


    if ( _errno != ERROR_BROKEN_DATA_PTR )
      if ( _errno == ERROR_ELEM_SIZE )
        fwrite(_data, sizeof(char), _max_size, dumpster);
      else 
        {
          for ( size_t cnt = 0; cnt < _max_size; cnt++ )
            {
              for ( size_t i = 0; i < _elem_size; i++ )
                fprintf(dumpster, "%X|", *(unsigned char*)(_data + _elem_size * cnt + i ) );
              
              fprintf(dumpster, "   %10c\n", ( cnt <= _size ) ? '*' : ' ');
            }
        }

    
    fclose(dumpster);

    return true;
  }  


#endif 
