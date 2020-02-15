// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2015, Linaro Limited
 */

#ifndef _LIB_AEABI_SOFTFLOAT_H_
#define	_LIB_AEABI_SOFTFLOAT_H_

/*
 * On ARM32 EABI defines both a soft-float ABI and a hard-float ABI,
 * hard-float is basically a super set of soft-float. Hard-float requires
 * all the support routines provided for soft-float, but the compiler may
 * choose to optimize to not use some of them.
 *
 * The AEABI functions uses soft-float calling convention even if the
 * functions are compiled for hard-float. So where float and double would
 * have been expected we use aeabi_float_t and aeabi_double_t respectively
 * instead.
 */

typedef unsigned aeabi_float_t;
typedef unsigned long long aeabi_double_t;

/* Prototypes */
aeabi_double_t __aeabi_dadd(aeabi_double_t a, aeabi_double_t b);
aeabi_double_t __aeabi_ddiv(aeabi_double_t a, aeabi_double_t b);
aeabi_double_t __aeabi_dmul(aeabi_double_t a, aeabi_double_t b);
aeabi_double_t __aeabi_drsub(aeabi_double_t a, aeabi_double_t b);
aeabi_double_t __aeabi_dsub(aeabi_double_t a, aeabi_double_t b);

int __aeabi_dcmpeq(aeabi_double_t a, aeabi_double_t b);
int __aeabi_dcmplt(aeabi_double_t a, aeabi_double_t b);
int __aeabi_dcmple(aeabi_double_t a, aeabi_double_t b);
int __aeabi_dcmpge(aeabi_double_t a, aeabi_double_t b);
int __aeabi_dcmpgt(aeabi_double_t a, aeabi_double_t b);

aeabi_float_t __aeabi_fadd(aeabi_float_t a, aeabi_float_t b);
aeabi_float_t __aeabi_fdiv(aeabi_float_t a, aeabi_float_t b);
aeabi_float_t __aeabi_fmul(aeabi_float_t a, aeabi_float_t b);
aeabi_float_t __aeabi_frsub(aeabi_float_t a, aeabi_float_t b);
aeabi_float_t __aeabi_fsub(aeabi_float_t a, aeabi_float_t b);

int __aeabi_fcmpeq(aeabi_float_t a, aeabi_float_t b);
int __aeabi_fcmplt(aeabi_float_t a, aeabi_float_t b);
int __aeabi_fcmple(aeabi_float_t a, aeabi_float_t b);
int __aeabi_fcmpge(aeabi_float_t a, aeabi_float_t b);
int __aeabi_fcmpgt(aeabi_float_t a, aeabi_float_t b);

int __aeabi_d2iz(aeabi_double_t a);
unsigned __aeabi_d2uiz(aeabi_double_t a);
long long __aeabi_d2lz(aeabi_double_t a);
unsigned long long __aeabi_d2ulz(aeabi_double_t a);
int __aeabi_f2iz(aeabi_float_t a);
unsigned __aeabi_f2uiz(aeabi_float_t a);
long long __aeabi_f2lz(aeabi_float_t a);
unsigned long long __aeabi_f2ulz(aeabi_float_t a);

aeabi_float_t __aeabi_d2f(aeabi_double_t a);
aeabi_double_t __aeabi_f2d(aeabi_float_t a);

aeabi_double_t __aeabi_i2d(int a);
aeabi_double_t __aeabi_ui2d(unsigned a);
aeabi_double_t __aeabi_l2d(long long a);
aeabi_double_t __aeabi_ul2d(unsigned long long a);
aeabi_float_t __aeabi_i2f(int a);
aeabi_float_t __aeabi_ui2f(unsigned a);
aeabi_float_t __aeabi_l2f(long long a);
aeabi_float_t __aeabi_ul2f(unsigned long long a);

#endif	/* !_LIB_AEABI_SOFTFLOAT_H_ */
