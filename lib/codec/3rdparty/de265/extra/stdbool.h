/* Copyright (C) 1998-2013 Free Software Foundation, Inc.

This file is part of GCC, modify by Min Chen.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

/*
 * ISO C Standard:  7.16  Boolean type and values  <stdbool.h>
 */

#ifndef _STDBOOL_H
#define _STDBOOL_H

#ifndef __cplusplus

#if defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#define bool	_Bool
#define true	1
#define false	0

#else   /* not C99 */

typedef enum {
    false = 0,
    true = 1
} bool;

#endif /* not C99 */

#else /* __cplusplus */

#if !defined(_MSC_VER)

/* Supporting <stdbool.h> in C++ is a GCC extension.  */
#define _Bool	bool
#define bool	bool
#define false	false
#define true	true

#endif /* _MSC_VER */

#endif /* __cplusplus */

/* Signal that all the definitions are present.  */
#define __bool_true_false_are_defined	1

#endif	/* stdbool.h */
