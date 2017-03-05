

/* WARNING! 
  There are some things you should know before defining your own unit test:
  -- Description of each unit test should be placed as a string constant
  -- Unit test should return whether it is true or false
  -- If there is an error and the unit test is not aborted in the wake of standart MyStack verification,
     it should output the information about its work in stderr and call Stack :: dump() of MyStack. 
*/





DEFINE_UNIT_TEST
(
  CTOR,
  "This test creates a stack sized 1000 with an elemement sized 11 bytes\n",
  {
    const size_t test_max_size = 1000;
    const size_t test_elem_size = 11;

    size_t max_size = 0;
    size_t elem_size = 0;

    Stack instance_stack(test_max_size, test_elem_size);

    if ( ( test_max_size != ( max_size = instance_stack.maxSize() ) ) || (test_elem_size != ( elem_size = instance_stack.elemSize() ) ) )
      {
        fprintf(stderr, "This test has intended to created a stack sized %u with an element sized %u bytes,"
                        "but has got the stack sized %u with the element sized %u bytes\n", test_max_size, test_elem_size, max_size, elem_size);
        instance_stack.dump();                

        return false;                 
      }

    return true;
  }
)





DEFINE_UNIT_TEST
(
  CTOR_AND_IS_EMPTY,
  "Firstly, this test creates a stack size 1200 with an element sized 27\n"
  "Then it checks whether the stack is empty\n",
  {
    const size_t test_max_size = 1200;
    const size_t test_elem_size = 27;

    Stack instance_stack(test_max_size, test_elem_size);

    if ( !instance_stack.isEmpty() )
      {
        fprintf(stderr, "The stack is not empty\n");
        instance_stack.dump();

        return false;
      } 

    return true;  
  }
)





DEFINE_UNIT_TEST
(
  PUSH_AND_POP_1, 
  "Firstly, this test pushs a number to the stack of the capacity 100 with an element with the size sizeof(int)\n"
  "Then it pops the number and checks whether it is equal to the one pushed to the stack\n",  
  {
    Stack instance_stack(100, sizeof(int));

    int test_number = 0xAC35F;
    instance_stack.push(&test_number);
    
    int* pointer = (int*)instance_stack.pop();

    bool isTestOk = true;
    if ( pointer )
      {
        if ( *pointer != test_number)
          {
            fprintf(stderr, "The test has pushed number %d to the stack, but has got the pointer to number %d\n", test_number, *pointer);
            isTestOk = false;
          }
      }
    else
      {
        fprintf(stderr, "The test has pushed number %d to the stack, but has got the NULL as the pointer\n", test_number);
        isTestOk = false;
      }

    if ( !isTestOk ) 
      instance_stack.dump();


    return isTestOk; 
  }
)






DEFINE_UNIT_TEST
(
  PUSH_AND_POP_2,
  "Firstly, this test creates a stack sized 520 with an element sized sizeof(size_t)\n"
  "Then it pushs 520 numbers to the stack and pops them in order to check\n"
  "whether they are organized rightly\n",
  {
    const size_t test_max_size = 520;
    const size_t test_elem_size = sizeof(size_t);

    Stack instance_stack(test_max_size, test_elem_size);
    
    size_t* pointer = NULL;

    size_t cnt = 0;
    for ( cnt = 0; cnt < test_max_size; cnt++)
      instance_stack.push(&cnt);

    for ( cnt = test_max_size - 1; cnt < test_max_size; cnt-- )
      if ( !(pointer = instance_stack.pop()) )
        {
          fprintf(stderr, "Number %u has been expected, but the pointer to the popped number is NULL\n", cnt);

          instance_stack.dump();

          return false; 
        }
      else if ( *pointer != cnt )
        {
          fprintf(stderr, "It has got %u, but number %u was expected\n", *pointer, cnt);

          instance_stack.dump();

          return false;
        }

    return true;
  }
)





DEFINE_UNIT_TEST
(
  SIZE,
  "Firstly, this test creates a stack sized 6500 with an element sized 5 bytes\n"
  "Then it pushs 3267 numbers to the stack and checks whether the size of the stack is equal to 3267\n",
  {
    const size_t test_max_size = 6500;
    const size_t test_elem_size = 3;
    const size_t test_size = 3267;

    size_t size = 0;

    Stack instance_stack(test_max_size, test_elem_size);

    for ( int i = 0; i < test_size; i++ )
      instance_stack.push(&i);

    if ( test_size != ( size = instance_stack.size() ) )
      {
        fprintf(stderr, "The stack is sized %u, but it should be sized %u\n", size, test_size);

        instance_stack.dump();

        return false;
      }  

    return true;  
  }
)