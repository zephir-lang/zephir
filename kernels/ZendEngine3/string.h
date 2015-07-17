
/*
  +------------------------------------------------------------------------+
  | Zephir Language                                                        |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2015 Zephir Team (http://www.zephir-lang.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@zephir-lang.com so we can send you a copy immediately.      |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@zephir-lang.com>                     |
  |          Eduar Carvajal <eduar@zephir-lang.com>                        |
  |          Vladimir Kolesnikov <vladimir@extrememember.com>              |
  +------------------------------------------------------------------------+
*/

#ifndef ZEPHIR_KERNEL_STRING_H
#define ZEPHIR_KERNEL_STRING_H

#include <php.h>
#include <Zend/zend.h>
#include "kernel/main.h"

#define ZEPHIR_TRIM_LEFT  1
#define ZEPHIR_TRIM_RIGHT 2
#define ZEPHIR_TRIM_BOTH  3
#define ZEPHIR_SUBSTR_NO_LENGTH 1

int zephir_fast_strlen_ev(zval *str);
void zephir_fast_strtolower(zval *return_value, zval *str);
void zephir_fast_strtoupper(zval *return_value, zval *str);
void zephir_fast_join_str(zval *result, char *glue, unsigned int glue_length, zval *pieces);
void zephir_fast_strpos(zval *return_value, const zval *haystack, const zval *needle, unsigned int offset);
void zephir_fast_strpos_str(zval *return_value, const zval *haystack, char *needle, unsigned int needle_length);
void zephir_fast_trim(zval *return_value, zval *str, zval *charlist, int where);

/** spprintf */
int zephir_spprintf(char **message, int max_len, char *format, ...);

/** JSON */
int zephir_json_encode(zval *return_value, zval *v, int opts);
int zephir_json_decode(zval *return_value, zval *v, zend_bool assoc);

/** Hash */
void zephir_md5(zval *return_value, zval *str);

/** */
void zephir_ucfirst(zval *return_value, zval *s);

#endif /* ZEPHIR_KERNEL_STRING_H */
