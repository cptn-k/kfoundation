/* Unicode character classification and properties.
   Copyright (C) 2002, 2005-2010 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef _UNICTYPE_H
#define _UNICTYPE_H

#include "unitypes.h"

/* ========================================================================= */

/* Like ISO C <ctype.h> and <wctype.h>.  These functions are deprecated,
   because this set of functions was designed with ASCII in mind and cannot
   reflect the more diverse reality of the Unicode character set.  But they
   can be a quick-and-dirty porting aid when migrating from wchar_t APIs
   to Unicode strings.  */

/* Test for any character for which 'uc_is_alpha' or 'uc_is_digit' is true.  */
extern bool
       uc_is_alnum (ucs4_t uc);

/* Test for any character for which 'uc_is_upper' or 'uc_is_lower' is true,
   or any character that is one of a locale-specific set of characters for
   which none of 'uc_is_cntrl', 'uc_is_digit', 'uc_is_punct', or 'uc_is_space'
   is true.  */
extern bool
       uc_is_alpha (ucs4_t uc);

/* Test for any control character.  */
extern bool
       uc_is_cntrl (ucs4_t uc);

/* Test for any character that corresponds to a decimal-digit character.  */
extern bool
       uc_is_digit (ucs4_t uc);

/* Test for any character for which 'uc_is_print' is true and 'uc_is_space'
   is false.  */
extern bool
       uc_is_graph (ucs4_t uc);

/* Test for any character that corresponds to a lowercase letter or is one
   of a locale-specific set of characters for which none of 'uc_is_cntrl',
   'uc_is_digit', 'uc_is_punct', or 'uc_is_space' is true.  */
extern bool
       uc_is_lower (ucs4_t uc);

/* Test for any printing character.  */
extern bool
       uc_is_print (ucs4_t uc);

/* Test for any printing character that is one of a locale-specific set of
   characters for which neither 'uc_is_space' nor 'uc_is_alnum' is true.  */
extern bool
       uc_is_punct (ucs4_t uc);

/* Test for any character that corresponds to a locale-specific set of
   characters for which none of 'uc_is_alnum', 'uc_is_graph', or 'uc_is_punct'
   is true.  */
extern bool
       uc_is_space (ucs4_t uc);

/* Test for any character that corresponds to an uppercase letter or is one
   of a locale-specific set of character for which none of 'uc_is_cntrl',
   'uc_is_digit', 'uc_is_punct', or 'uc_is_space' is true.  */
extern bool
       uc_is_upper (ucs4_t uc);

/* Test for any character that corresponds to a hexadecimal-digit
   character.  */
extern bool
       uc_is_xdigit (ucs4_t uc);

/* GNU extension. */
/* Test for any character that corresponds to a standard blank character or
   a locale-specific set of characters for which 'uc_is_alnum' is false.  */
extern bool
       uc_is_blank (ucs4_t uc);

/* ========================================================================= */

#endif /* _UNICTYPE_H */
