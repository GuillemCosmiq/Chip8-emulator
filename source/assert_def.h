#ifndef __ASSERT_DEF_H__
#define __ASSERT_DEF_H__

#ifdef _DEBUG
#include <assert.h>
#define emu_assert(expr) do { assert(expr); } while(0)
#else
#define emu_assert(expr) do {} while(0)
#endif

#endif
