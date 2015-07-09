
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

#ifndef ZEPHIR_KERNEL_OBJECT_H
#define ZEPHIR_KERNEL_OBJECT_H

#include <php.h>
#include <Zend/zend.h>

#include "kernel/globals.h"
#include "kernel/main.h"

zend_class_entry *zephir_fetch_class_str_ex(char *class_name, size_t length, int fetch_type);

/** Reading properties */
int zephir_read_property(zval *result, zval *object, const char *property_name, zend_uint property_length, int silent);

/** Updating properties */
int zephir_update_property_zval(zval *obj, const char *property_name, unsigned int property_length, zval *value);

/** Updating array properties */
int zephir_update_property_array(zval *object, const char *property, zend_uint property_length, const zval *index, zval *value);
int zephir_update_property_array_string(zval *object, char *property, unsigned int property_length, char *index, unsigned int index_length, zval *value);
int zephir_update_property_array_append(zval *object, char *property, unsigned int property_length, zval *value);
int zephir_update_property_array_multi(zval *object, const char *property, zend_uint property_length, zval *value, const char *types, int types_length, int types_count, ...);

/** Static properties */
int zephir_read_static_property_ce(zval *result, zend_class_entry *ce, const char *property, int len, int flags);
int zephir_update_static_property_ce(zend_class_entry *ce, const char *name, int len, zval *value);
int zephir_update_static_property_array_multi_ce(zend_class_entry *ce, const char *property, zend_uint property_length, zval *value, const char *types, int types_length, int types_count, ...);

#endif
