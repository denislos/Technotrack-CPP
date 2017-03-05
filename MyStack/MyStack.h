
// This is the header file of C++ library of stack created by Denis Los
// This version is from 05.03.2017 (v2.0) 




#ifndef MYSTACK_H

#define MYSTACK_H







extern const size_t STACK_DEFAULT_MAX_SIZE;
extern const size_t STACK_DEFAULT_ELEM_SIZE;






#ifndef NDEBUG

extern const unsigned char* STACK_DUMPSTER_NAME;


enum STACK_ERROR_CODE
  {
    STACK_IS_OK = 0,
    STACK_IS_DESTROYED = 0xFFFAEF,
    ERROR_BROKEN_DATA_PTR = 0xAFEF3,
    ERROR_LEFT_CANARY = 0xBABEFF,
    ERROR_RIGHT_CANARY = 0xCDEDF,
    ERROR_HASH_VALUE = 0xEBDBFA,
    ERROR_ELEM_SIZE = 0xECEABF,
    ERROR_MAX_SIZE = 0xEEFABCC,
    ERROR_STACK_SIZE = 0xADFABF
  };

enum STACK_CANARY
  {
    LEFT_CANARY_VALUE = 0xAFBFDAEE,
    RIGHT_CANARY_VALUE = 0xFEFEFEFE
  };

#endif





class Stack
  {

    public:
      Stack(size_t max_size = STACK_DEFAULT_MAX_SIZE, size_t elem_size = STACK_DEFAULT_ELEM_SIZE);
      ~Stack();
      bool push(void* elem);

      //WARNING! 
      /*
        pop() returns a pointer to data located in the stack,
        thus do not deallocate or reallocate the memory using the returned pointer
      */
      void* pop();
      
      size_t size() const;
      bool isEmpty() const;
      size_t maxSize() const;
      size_t elemSize() const;

      #ifndef NDEBUG
        int verify();
        bool dump(unsigned char* dumpster_name = STACK_DUMPSTER_NAME) const;
        int getErrno() const;
      #endif   
    private:
      #ifndef NDEBUG
        size_t hash_function() const;
      #endif


    private:
      #ifndef NDEBUG
        int _LEFT_CANARY;
        size_t _hash_value;
        int _errno;
      #endif
      
      size_t _size;
      void* _data;
      size_t _elem_size;
      size_t _max_size;

      #ifndef NDEBUG
        int _RIGHT_CANARY;
      #endif
    

    #ifndef NDEBUG
      friend class MyStackUnitTests;
    #endif

  };




#endif // MYSTACK_H
