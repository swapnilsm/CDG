#include <stdio.h>
#include "stack.h"

typedef struct user_data {
  char c;
  int i;
} user_data;

int main(void) {

  Stack* user_data_stack = stackNew(sizeof(user_data));

  user_data x, y, z, temp;

  x.c = 'a';
  y.c = 'b';
  z.c = 'c';

  x.i = 1;
  y.i = 2;
  z.i = 3;  

  if(stackIsEmpty(user_data_stack)){
    printf("The stack is empty.\n");
  } else {
    printf("The stack is not empty.\n");
  }

  printf("Pushing : { %c, %d }\n", x.c, x.i);  
  stackPush(user_data_stack, &x);
  printf("Pushing : { %c, %d }\n", y.c, y.i);
  stackPush(user_data_stack, &y);
  printf("Pushing : { %c, %d }\n", z.c, z.i);  
  stackPush(user_data_stack, &z);  
  
  if(stackIsEmpty(user_data_stack)){
    printf("The stack is empty.\n");
  } else {
    printf("The stack is not empty.\n");
  }

  stackPeek(user_data_stack, &temp);
  printf("Stack top is : { %c, %d }\n", temp.c, temp.i);

  stackPop(user_data_stack, &temp);
  printf("Popping : { %c, %d }\n", temp.c, temp.i);
  stackPop(user_data_stack, &temp);
  printf("Popping : { %c, %d }\n", temp.c, temp.i);
  stackPop(user_data_stack, &temp);
  printf("Popping : { %c, %d }\n", temp.c, temp.i);

  if(stackIsEmpty(user_data_stack)){
    printf("The stack is empty.\n");
  } else {
    printf("The stack is not empty.\n");
  }  
  
  stackFree(user_data_stack);


  Stack* p;
  int i;
  int ia[10] = {0,1,2,3,4,5,6,7,8,9};
  int *ptr;
  p = stackNew(sizeof(ptr));
  for ( i = 0; i < 10; i++ ) {
    ptr = &ia[i];
    stackPush(p, &ptr);
  }

  for ( i = 0; i < 10; i++ ) {
    stackPop(p, &ptr);
    printf("%d ", *ptr);
  }
                              
  return 0;
}
