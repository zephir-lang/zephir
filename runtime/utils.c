
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

int _zephir_array_isset_quick_string(const zval *arr, const char *index, uint index_length, unsigned long key) {
	if (Z_TYPE_P(arr) == IS_ARRAY) {
		return zend_hash_quick_exists(Z_ARRVAL_P(arr), index, index_length, key);
	}
	return 0;
}

int _zephir_array_fetch_string(zval **return_value, zval *arr, const char *index, uint index_length TSRMLS_DC){

	zval **zv;

	if (zend_hash_find(Z_ARRVAL_P(arr), index, index_length, (void**) &zv) == SUCCESS) {
		*return_value = *zv;
		return SUCCESS;
	}

	*return_value = EG(uninitialized_zval_ptr);
	return FAILURE;
}

int _zephir_hash_fetch_string(zval **return_value, HashTable *arr, const char *index, uint index_length TSRMLS_DC){

	zval **zv;

	if (zend_hash_find(arr, index, index_length, (void**) &zv) == SUCCESS) {
		*return_value = *zv;
		return SUCCESS;
	}

	*return_value = EG(uninitialized_zval_ptr);
	return FAILURE;
}
