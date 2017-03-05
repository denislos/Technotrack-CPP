
//! This is the source file of MyStack unit tests created by Denis Los
//! This version is from 05.03.2017(v1.1)

//! It is designed for the v2.0 of MyStack


#include <stdio.h>
#include <stdlib.h>

#include "MyStack.h"
#include "MyStackUnitTests.h"





const unit_test_t MyStackUnitTests :: _unit_tests[] =
  {
    #define DEFINE_UNIT_TEST(name, message, unit_test) , {#name, name}

    {"This function displays the information about the library of unit tests", showInfo }
    #include "MyStackUnitTestsLib.h"

    #undef DEFINE_UNIT_TEST 
  };






#define DEFINE_UNIT_TEST(name, info, unit_test) \
  bool name () \
    unit_test

#include "MyStackUnitTestsLib.h"

#undef DEFINE_UNIT_TEST







MyStackUnitTests :: MyStackUnitTests()
  {
    size_t array_tail = &_unit_tests + 1; 
    size_t array_head = &_unit_tests;

    _quantity = ( array_tail - array_head ) / sizeof(unit_test_t);
  }






bool MyStackUnitTests :: run() const 
  {
    showInfo();

    for ( size_t cnt = 1; cnt < _quantity; cnt++ )
      {
        fprintf(stderr,"\n%s\n", _unit_tests[cnt].name);

        fprintf(stderr, "[%s]\n", ( _unit_tests[cnt].unit_test() ) ? "OK" : "ERROR" );
      }
      
    fprintf(stderr, "\nDone\n");    
  }







bool showInfo()
  {
    fprintf(stderr, "-----------------------------------------------------\n"
                    "MyStack Unit Tests | Created by Denis Los\n\n"
                    "Descriptions of all unit tests can be found in MyStackUnitTestLib.h.\n\n" 
                    "If one of the tests is aborted in the wake of the standart verification of MyStack,"
                    "you will be able to see the information in dumpster.\n"
                    "-----------------------------------------------------\n\n");
    
    return true;
  }  


