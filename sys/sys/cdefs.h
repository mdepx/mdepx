#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#define	__unused
#define	__predict_true(e)	__builtin_expect((e), 1)
#define	__predict_false(e)	__builtin_expect((e), 0)
#define	__packed		__attribute__((__packed__))
#define	__aligned(x)		__attribute__((__aligned__(x)))
