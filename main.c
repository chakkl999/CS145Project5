/*
 * Project5.c
 *
 * Created: 6/3/2020 11:48:29 AM
 * Author : MrTrashCan
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "avr.h"
#include "lcd.h"
#include "stack.h"

#define DECIMAL '.'
#define DIVIDE '/'
#define MULTIPLY '*'
#define SUBTRACT '-'
#define ADD '+'
#define DELIMITER ':'
#define BUFFER_SIZE 16
#define INTERVAL 500

int toRPN(char infix[], char postfix[], int index);
void calculate(char buffer[], int index);
void display(char buffer[], int index);
int isNumOrDecimal(const char *c);
int operatorLessThanOrEqual(char c1, char c2);
float getNum();
char mapToOp(const char *c);
int get_key();
int is_pressed(int r, int c);

int main(void)
{
    /* Replace with your application code */
	lcd_init();
	char characters[] = "X123A456B789C*0#D";
	int key;
	char infix[BUFFER_SIZE+1] = {0}, postfix[BUFFER_SIZE*2+1] = {0};
	int index = 0, index2;
    while (1) 
    {
		key = get_key();
		if(key > 0 && key != 15)
		{
			infix[index++] = mapToOp(&characters[key]);
			display(infix, index);
		}
		//else if(key == 13)
		//{
		//	index--;
		//	if(index < 0)
		//		index = 0;
		//	display(infix, index);
		//}
		else if(key == 15)
		{
			index2 = toRPN(infix, postfix, index);
			//display(postfix, index2);
			calculate(postfix, index2);
			clr();
			clrf();
			index = 0;
		}
		avr_wait(INTERVAL);
    }
}

int toRPN(char infix[], char postfix[], int index)
{
	int index2 = 0;
	for(int i = 0; i < index; ++i)
	{
		if(isNumOrDecimal(&infix[i]))
			postfix[index2++] = infix[i];
		else
		{
			postfix[index2++] = DELIMITER;
			if(empty())
				push(infix[i]);
			else
			{
				while(!empty() && operatorLessThanOrEqual(infix[i], peek()))
				{
					postfix[index2++] = pop();
				}
				push(infix[i]);
			}
		}
	}
	if(isNumOrDecimal(&postfix[index2-1]))
		postfix[index2++] = DELIMITER;
	while(!empty())
		postfix[index2++] = pop();
	return index2;
}

void calculate(char buffer[], int index)
{
	float temp1, temp2;
	char buf[17] = {0};
	for(int i = 0; i < index; ++i)
	{
		if(isNumOrDecimal(&buffer[i]))
			push(buffer[i]);
		else
		{
			switch(buffer[i])
			{
				case DELIMITER:
					push(buffer[i]);
					pushf(getNum());
					break;
				case MULTIPLY:
					temp1 = popf();
					temp2 = popf();
					pushf(temp2 * temp1);
					break;
				case DIVIDE:
					temp1 = popf();
					temp2 = popf();
					pushf(temp2 / temp1);
					break;
				case ADD:
					temp1 = popf();
					temp2 = popf();
					pushf(temp2 + temp1);
					break;
				case SUBTRACT:
					temp1 = popf();
					temp2 = popf();
					pushf(temp2 - temp1);
					break;
			}
		}
	}
	lcd_pos(1,0);
	sprintf(buf, "=%g", popf());
	lcd_puts(buf);
}

void display(char buffer[], int index)
{
	lcd_clr();
	if(index == 0)
		return;
	lcd_pos(0,0);
	for(int i = 0; i < index; ++i)
		lcd_put(buffer[i]);
}

int isNumOrDecimal(const char *c)
{
	if((*c >= '0' && *c <= '9') || *c == DECIMAL)
		return 1;
	return 0;
}

int operatorLessThanOrEqual(char c1, char c2)
{
	if(c1 == ADD || c1 == SUBTRACT)
		return 1;
	else
		if(c2 == ADD || c2 == SUBTRACT)
			return 0;
		else
			return 1;
}

float getNum()
{
	long num = 0, tenth = 1;
	int digit = 0, decimal = -1;
	pop();
	while(peek() != DELIMITER && !empty())
	{
		if(peek() == DECIMAL)
		{
			decimal = digit;
			pop();
		}
		else
		{
			num += (pop() - '0') * tenth;
			tenth *= 10;
			digit++;
		}
	}
	if(decimal > 0)
	{
		tenth = 1;
		for(int i = 0; i < decimal; ++i)
			tenth *= 10;
		return num / (float)tenth;
	}
	return num;
}

char mapToOp(const char *c)
{
	if(*c == '*')
		return DECIMAL;
	if(isNumOrDecimal(c))
		return *c;
	if(*c == 'A')
		return ADD;
	if(*c == 'B')
		return SUBTRACT;
	if(*c == 'C')
		return MULTIPLY;
	if(*c == 'D')
		return DIVIDE;
	return 'E';
}

int get_key()
{
	for(int r = 0; r < 4; ++r)
		for(int c = 0; c < 4; ++c)
			if(is_pressed(r,c+4))
				return r*4+c+1;
	return 0;
}

int is_pressed(int r, int c)
{
	DDRC = 0b00000000;
	PORTC = 0b00000000;
	SET_BIT(DDRC, r);
	SET_BIT(PORTC, c);
	avr_wait(1);
	if (!GET_BIT(PINC, c))
		return 1;
	return 0;
}