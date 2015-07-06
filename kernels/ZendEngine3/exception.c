
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ext.h"
#include "php_main.h"
#include "ext/standard/php_string.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"

#include "Zend/zend_exceptions.h"

/**
 * Throws an exception with a single string parameter
 */
void zephir_throw_exception_string(zend_class_entry *ce, const char *message, zend_uint message_len)
{
	zval object, msg;
	int ZEPHIR_LAST_CALL_STATUS = 0;

	object_init_ex(&object, ce);

	ZVAL_STRINGL(&msg, message, message_len);

	ZEPHIR_CALL_METHOD(NULL, &object, "__construct", NULL, 0, &msg);

	if (ZEPHIR_LAST_CALL_STATUS != FAILURE) {
		zend_throw_exception_object(&object);
	}

	zval_ptr_dtor(&msg);
}

/**
 * Throws an exception with a string format as parameter
 */
void zephir_throw_exception_format(zend_class_entry *ce, const char *format, ...)
{
	zval object, msg;
	int ZEPHIR_LAST_CALL_STATUS = 0, len;
	char *buffer;
	va_list args;

	object_init_ex(&object, ce);

	va_start(args, format);
	len = vspprintf(&buffer, 0, format, args);
	va_end(args);

	ZVAL_STRINGL(&msg, buffer, len);
	efree(buffer);

	ZEPHIR_CALL_METHOD(NULL, &object, "__construct", NULL, 0, &msg);

	if (ZEPHIR_LAST_CALL_STATUS != FAILURE) {
		zend_throw_exception_object(&object);
	}
	zend_error(E_ERROR, "Not implemented yet TODO");

	zval_ptr_dtor(&msg);
}
