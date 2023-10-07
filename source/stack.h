#ifndef __STACK_H__
#define __STACK_H__

#include "types.h"
#include "assert_def.h"

#define STACK_MAX_SIZE 16

typedef struct Stack
{
	uint16 mem[STACK_MAX_SIZE];
	byte *top;
} Stack;

inline void stack_Init(Stack *const stack)
{
	stack->top = (byte*)stack->mem;
}

inline void stack_Push(Stack *const stack, uint16 val)
{
	emu_assert(stack->top != (byte*)&(stack->mem[STACK_MAX_SIZE - 1])
		|| stack->top != (byte*)&(stack->mem[STACK_MAX_SIZE - 2]));
	*(stack->top) = (byte)(val >> 8);
	stack->top += 1;
	*stack->top = val & 0x00FF;
	stack->top += 1;
}

inline uint16 stack_Pop(Stack *const stack)
{
	uint16 rtnVal;
	emu_assert(stack->top != (byte*)(stack->mem)
		|| stack->top != (byte*)(stack->mem) + 1);
	rtnVal = ((uint16)(*(stack->top - 1)) | ((uint16)(*(stack->top - 2)) << 8));
	stack->top -= 2;
	return rtnVal;
}

#endif
