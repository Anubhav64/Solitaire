#include <stdbool.h>

// Stack: provides a Integer Stack ADT

struct stack {
  int len;
  int maxlen;
  int data[104];
};

// NOTE: ALL stack parameters must be valid stacks

void stack_init(struct stack *s);

// stack_is_empty(s) determines if stack s is empty
bool stack_is_empty(const struct stack *s);

// stack_top(s) returns the top item in stack s
// requires: stack is not empty
int stack_top(const struct stack *s);

// stack_pop(s) returns and pops (removes) the top item in stack s
// requires: stack is not empty
int stack_pop(struct stack *s);

// stack_push(s) adds item to the top of stack s
void stack_push(int item, struct stack *s);
