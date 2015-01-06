
/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include "php_zephir.h"
#include "zephir.h"
#include "utils.h"
#include "kernel/main.h"

/**
 * Shows an error + the location where it happened in the zephir file
 */
void zephir_error_ex(char *message, zval *location TSRMLS_DC) {

	zval *file, *line;

	if (location) {

		_zephir_array_fetch_string(&file, location, SS("file") TSRMLS_CC);
		_zephir_array_fetch_string(&line, location, SS("line") TSRMLS_CC);

		if (Z_TYPE_P(file) == IS_STRING && Z_TYPE_P(line) == IS_LONG) {
			zend_error(E_ERROR, "%s in %s on line %ld", message, Z_STRVAL_P(file), Z_LVAL_P(line));
		}
	}

	zend_error(E_ERROR, "%s", message);
}
