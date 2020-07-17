/*
 * stack.c
 *
 * Created: 6/3/2020 11:55:27 AM
 *  Author: MrTrashCan
 */ 

#include "stack.h"

int stack[STACK_MAX_SIZE];
float stackf[STACK_MAX_SIZE];
int top = -1;
int topf = -1;


void push(int num)
{
	stack[++top] = num;
}

int pop()
{
	return stack[top--];
}

int peek()
{
	return stack[top];
}

int empty()
{
	if(top == -1)
		return 1;
	return 0;
}

int full()
{
	if(top == (STACK_MAX_SIZE - 1))
		return 1;
	return 0;
}

int size()
{
	return top+1;
}

void clr()
{
	top = -1;
}

void pushf(float num)
{
	stackf[++topf] = num;
}

float popf()
{
	return stackf[topf--];
}

float peekf()
{
	return stackf[topf];
}

int emptyf()
{
	if(topf == -1)
		return 1;
	return 0;
}

int fullf()
{
	if(topf == (STACK_MAX_SIZE - 1))
		return 1;
	return 0;
}

int sizef()
{
	return topf+1;
}

void clrf()
{
	topf = -1;
}