/*
 * stack.h
 *
 * Created: 6/3/2020 11:51:28 AM
 *  Author: MrTrashCan
 */ 


#ifndef STACK_H_
#define STACK_H_

#define STACK_MAX_SIZE 50

void push(int num);
int pop();
int peek();
int empty();
int full();
int size();
void clr();
void pushf(float num);
float popf();
float peekf();
int emptyf();
int fullf();
int sizef();
void clrf();

#endif /* STACK_H_ */