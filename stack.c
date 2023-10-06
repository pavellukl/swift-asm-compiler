#include "gvid.h"       // par drobnosti pro zjednoduseni prace
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>  // pro praci s textovymi retezci
#include <stdbool.h> // pro praci s typem bool a konstantami true a false
// #include <ctype.h>   // isalpha, isspace, islower, isupper, ...
// #include <math.h>    // funkce z matematicke knihovny
// #include <float.h>   // konstanty pro racionalni typy DBL_MAX, DBL_DIG, ...
// #include <limits.h>  // konstanty pro celociselne typy INT_MAX, INT_MIN, ...
// #include <time.h>    // funkce time a dalsi pro praci s casem

typedef struct _element Element;


typedef struct {
  char value;
} Data;

struct _element {
  Element* next;
  Data data;
};


typedef struct {
  Element* top;
  int length;
} Stack;

Stack* stackInit() {
  Stack* stack = malloc(sizeof(stack));
  stack->length = 0;
  stack->top = NULL;

  return stack;
}

bool stackPop(Stack* stack, Data* data) {
  if(stack->top == NULL) return false;

  Element* tmp = stack->top;
  stack->top = stack->top->next;
  stack->length--;
  *data = tmp->data;
  free(tmp);

  return true;
}

bool stackPush(Stack* stack, Data data) {
  Element* newEle = malloc(sizeof(Element));
  if(newEle == NULL) return false;
  stack->length++;
  newEle->data = data;
  newEle->next = stack->top;
  stack->top = newEle;


  return true;
}

void stackFree(Stack* stack) {
  Data data = {.value = ' '};
  while(stackPop(stack, &data));
}

bool isEmpty(Stack* stack) {
  if(stack->top == NULL) return true;
  return false;
}

void readChars(Stack* stack) {
  FILE* f = fopen("a.txt", "r");
  if(f == NULL) return;

  char c;
  while((c = getc(f)) != EOF && c != '\n') {
    Data data = {.value = c};
    stackPush(stack, data);
  }
}

bool checkBrackets(Stack* stack) {
  FILE* f = fopen("a.txt", "r");
  if(f == NULL) return false;

  char c;
  while((c = getc(f)) != EOF && (c != '\n')) {
    Data data = {.value = c};
    if(c == '{' || c == '<' || c == '[' || c == '(') {
      stackPush(stack, data);
    } else {

      Element* last = stack->top;

      if(c == ')' && last->data.value == '(')
      {
        stackPop(stack, &data);
      }
      else if(last->data.value == (c - 2)) {
        stackPop(stack, &data);
      }
    }
  }

  return isEmpty(stack);
}

int main(void)
{
  Stack* stack = stackInit();
  //readChars(stack);
  if(checkBrackets(stack)) printf("Correct\n");
  else printf("Incorrect\n");
  for(Element* tmp = stack->top; tmp != NULL ; tmp = tmp->next) {
    printf("%c ", tmp->data.value);
  }
}
