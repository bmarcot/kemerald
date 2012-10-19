/**
 * \file stdarg.h
 * \brief POSIX-compliant implementation of variadic functions macros.
 * \author Benoît Marcot
 * \version 0.1
 * \date xx août 2009
 *
 * This implementations relies on GCC variadic builtins
 *
 * These macros enforce the POSIX compliancy.
 *
 */

#ifndef __STDARG_H__
#define __STDARG_H__

#define  va_list	__builtin_va_list

#define va_start(ap, parmN)			\
  __builtin_va_start(ap, parmN);

#define va_arg(ap, type)			\
  __builtin_va_arg(ap, type);

#define va_end(ap)				\
  __builtin_va_end(ap);

#endif /* !__STDARG_H__ */
