#include<stdio.h>
#include<stdlib.h>

void main () {
  int i;
  int *j;
  printf("%d \n", (int) sizeof(i));
  printf("%d \n", (int) sizeof(int));
  printf("%d \n", (int) sizeof(j));
  printf("%d \n", (int) sizeof(int*));    
}
