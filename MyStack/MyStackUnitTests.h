
//! This is the header file of MyStack unit tests created by Denis Los
//! This version is from 05.03.2017 (v1.1)

//! It is designed for v2.0 of MyStack




#ifndef MYSTACK_UNITTESTS_H

#define MYSTACK_UNITTESTS_H




struct unit_test_t
  {
    unsigned char* name;
    bool (*unit_test)();
  };






#define DEFINE_UNIT_TEST(name, message, unit_test)  \
  bool name () ;

#include "MyStackUnitTestsLib.h"

#undef DEFINE_UNIT_TEST







class MyStackUnitTests
  {
    public:
      MyStackUnitTests();
      bool run() const;

    private:
      size_t _quantity;

      static const unit_test_t _unit_tests[];
  };





bool showInfo();

 


 




#endif // MYSTACK_UNITTESTS_H