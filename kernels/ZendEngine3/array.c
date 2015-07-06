
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

#include <php.h>
#include "php_ext.h"
#include <ext/standard/php_array.h>
#include <Zend/zend_hash.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/debug.h"
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/hash.h"
#include "kernel/backtrace.h"


void ZEPHIR_FASTCALL zephir_create_array(zval *return_value, uint size, int initialize)
{
	uint i;
	zval null_value;
	HashTable *hashTable;
	ZVAL_NULL(&null_value);

	if (size > 0) {

		ALLOC_HASHTABLE(hashTable);
		zephir_hash_init(hashTable, size, NULL, ZVAL_PTR_DTOR, 0);

		if (initialize) {
			for (i = 0; i < size; i++) {
				zend_hash_next_index_insert(hashTable, &null_value);
			}
		}
		ZVAL_ARR(return_value, hashTable);
	} else {
		array_init(return_value);
	}
}

int zephir_array_fetch_string(zval *return_value, zval *arr, const char *index, uint index_length, int flags ZEPHIR_DEBUG_PARAMS)
{
	zval *zv;

	if (likely(Z_TYPE_P(arr) == IS_ARRAY)) {
		if ((zv = zend_hash_str_find(Z_ARRVAL_P(arr), index, index_length)) != NULL) {
			ZVAL_COPY_VALUE(return_value, zv);
			if ((flags & PH_READONLY) != PH_READONLY) {
				Z_ADDREF_P(return_value);
			}
			return SUCCESS;
		}
		if ((flags & PH_NOISY) == PH_NOISY) {
			zend_error(E_NOTICE, "Undefined index: %s", index);
		}
	} else {
		if ((flags & PH_NOISY) == PH_NOISY) {
			zend_error(E_NOTICE, "Cannot use a scalar value as an array in %s on line %d", file, line);
		}
	}

	if (return_value == NULL) {
		zend_error(E_ERROR, "No return value passed to zephir_array_fetch_string");
		return FAILURE;
	}
	ZVAL_NULL(return_value);
	if ((flags & PH_READONLY) != PH_READONLY) {
		Z_ADDREF_P(return_value);
	}
	return FAILURE;
}
