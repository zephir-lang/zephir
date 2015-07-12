
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
  +------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ext.h"
#include "php_main.h"
#include "ext/spl/spl_exceptions.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/exception.h"

#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

int zephir_is_iterable_ex(zval *arr, int duplicate)
{
	if (unlikely(Z_TYPE_P(arr) != IS_ARRAY)) {
		return 0;
	}
    //TODO: duplicate
    return 1;
}

/**
 * Parses method parameters with minimum overhead
 */
int zephir_fetch_parameters(int num_args, int required_args, int optional_args, ...)
{
	va_list va;
	int arg_count = ZEND_CALL_NUM_ARGS(EG(current_execute_data));
	zval *arg, *p;
	int i;

	if (num_args < required_args || (num_args > (required_args + optional_args))) {
		zephir_throw_exception_string(spl_ce_BadMethodCallException, SL("Wrong number of parameters"));
		return FAILURE;
	}

	if (num_args > arg_count) {
		zephir_throw_exception_string(spl_ce_BadMethodCallException, SL("Could not obtain parameters for parsing"));
		return FAILURE;
	}

	if (!num_args) {
		return SUCCESS;
	}

	va_start(va, optional_args);

	i = 0;
	while (num_args-- > 0) {
		arg = ZEND_CALL_ARG(EG(current_execute_data), i + 1);
		p = va_arg(va, zval *);
		ZVAL_COPY_VALUE(p, arg);

		i++;
	}

	va_end(va);

	return SUCCESS;
}

/**
 * Gets the global zval into PG macro
 */
int zephir_get_global(zval *arr, const char *global, unsigned int global_length)
{
	zval *gv;
	zend_bool jit_initialization = PG(auto_globals_jit);
	zend_string *str = zend_string_init(global, global_length, 0);

	if (jit_initialization) {
		zend_is_auto_global(str);
	}

	zval_ptr_dtor(arr);
	ZVAL_UNDEF(arr);

	if (&EG(symbol_table)) {
		if ((gv = zend_hash_find(&EG(symbol_table), str)) != NULL) {
			if (Z_TYPE_P(gv) == IS_ARRAY) {
				ZVAL_COPY_VALUE(arr, gv);
				zend_string_free(str);
				return SUCCESS;
			}
		}
	}
	array_init(arr);
	zend_hash_update(&EG(symbol_table), str, arr);
	zend_string_free(str);
	return SUCCESS;
}

/**
 * Makes fast count on implicit array types
 */
void zephir_fast_count(zval *result, zval *value)
{
	if (Z_TYPE_P(value) == IS_ARRAY) {
		ZVAL_LONG(result, zend_hash_num_elements(Z_ARRVAL_P(value)));
		return;
	}

	if (Z_TYPE_P(value) == IS_OBJECT) {

		#ifdef HAVE_SPL
		zval retval;
		#endif

		if (Z_OBJ_HT_P(value)->count_elements) {
			ZVAL_LONG(result, 1);
			if (SUCCESS == Z_OBJ_HT(*value)->count_elements(value, &Z_LVAL_P(result))) {
				return;
			}
		}

		#ifdef HAVE_SPL
		if (instanceof_function(Z_OBJCE_P(value), spl_ce_Countable)) {
			zend_call_method_with_0_params(value, NULL, NULL, "count", &retval);
			if (Z_TYPE(retval) != IS_UNDEF) {
				convert_to_long_ex(&retval);
				ZVAL_LONG(result, Z_LVAL(retval));
				zval_ptr_dtor(&retval);
			}
			return;
		}
		#endif

		ZVAL_LONG(result, 0);
		return;
	}

	if (Z_TYPE_P(value) == IS_NULL) {
		ZVAL_LONG(result, 0);
		return;
	}

	ZVAL_LONG(result, 1);
}

/**
 * Makes fast count on implicit array types without creating a return zval value
 */
int zephir_fast_count_ev(zval *value)
{
	long count = 0;

	if (Z_TYPE_P(value) == IS_ARRAY) {
		return zend_hash_num_elements(Z_ARRVAL_P(value)) > 0;
	}

	if (Z_TYPE_P(value) == IS_OBJECT) {

		#ifdef HAVE_SPL
		zval retval;
		#endif

		if (Z_OBJ_HT_P(value)->count_elements) {
			Z_OBJ_HT(*value)->count_elements(value, &count TSRMLS_CC);
			return (int) count > 0;
		}

		#ifdef HAVE_SPL
		if (instanceof_function(Z_OBJCE_P(value), spl_ce_Countable)) {
			zend_call_method_with_0_params(value, NULL, NULL, "count", &retval);
			if (Z_TYPE(retval) != IS_UNDEF) {
				convert_to_long_ex(&retval);
				count = Z_LVAL(retval);
				zval_ptr_dtor(&retval);
				return (int) count > 0;
			}
			return 0;
		}
		#endif

		return 0;
	}

	if (Z_TYPE_P(value) == IS_NULL) {
		return 0;
	}

	return 1;
}

/**
 * Makes fast count on implicit array types without creating a return zval value
 */
int zephir_fast_count_int(zval *value)
{
	long count = 0;

	if (Z_TYPE_P(value) == IS_ARRAY) {
		return zend_hash_num_elements(Z_ARRVAL_P(value));
	}

	if (Z_TYPE_P(value) == IS_OBJECT) {

		#ifdef HAVE_SPL
		zval retval;
		#endif

		if (Z_OBJ_HT_P(value)->count_elements) {
			Z_OBJ_HT(*value)->count_elements(value, &count TSRMLS_CC);
			return (int) count;
		}

		#ifdef HAVE_SPL
		if (instanceof_function(Z_OBJCE_P(value), spl_ce_Countable)) {
			zend_call_method_with_0_params(value, NULL, NULL, "count", &retval);
			if (Z_TYPE(retval) != IS_UNDEF) {
				convert_to_long_ex(&retval);
				count = Z_LVAL(retval);
				zval_ptr_dtor(&retval);
				return (int) count;
			}
			return 0;
		}
		#endif

		return 0;
	}

	if (Z_TYPE_P(value) == IS_NULL) {
		return 0;
	}

	return 1;
}

zend_class_entry* zephir_get_internal_ce(const char *class_name, unsigned int class_name_len)
{
    zend_class_entry* temp_ce;

    if ((temp_ce = zend_hash_str_find_ptr(CG(class_table), class_name, class_name_len)) == NULL) {
        zend_error(E_ERROR, "Class '%s' not found", class_name);
        return NULL;
    }

    return temp_ce;
}

/* Declare constants */
int zephir_declare_class_constant(zend_class_entry *ce, const char *name, size_t name_length, zval *value)
{
	if (Z_CONSTANT_P(value)) {
		ce->ce_flags &= ~ZEND_ACC_CONSTANTS_UPDATED;
	}
	ZVAL_NEW_PERSISTENT_REF(value, value);
	return zend_hash_str_update(&ce->constants_table, name, name_length, value) ?
		SUCCESS : FAILURE;
}

int zephir_declare_class_constant_null(zend_class_entry *ce, const char *name, size_t name_length)
{
	zval constant;

	ZVAL_NULL(&constant);
	return zephir_declare_class_constant(ce, name, name_length, &constant);
}

int zephir_declare_class_constant_long(zend_class_entry *ce, const char *name, size_t name_length, zend_long value)
{
	zval constant;

	ZVAL_LONG(&constant, value);
	return zephir_declare_class_constant(ce, name, name_length, &constant);
}

int zephir_declare_class_constant_bool(zend_class_entry *ce, const char *name, size_t name_length, zend_bool value)
{
	zval constant;

	ZVAL_BOOL(&constant, value);
	return zephir_declare_class_constant(ce, name, name_length, &constant);
}

int zephir_declare_class_constant_double(zend_class_entry *ce, const char *name, size_t name_length, double value)
{
	zval constant;

	ZVAL_DOUBLE(&constant, value);
	return zephir_declare_class_constant(ce, name, name_length, &constant);
}

int zephir_declare_class_constant_stringl(zend_class_entry *ce, const char *name, size_t name_length, const char *value, size_t value_length)
{
	zval constant;

	ZVAL_NEW_STR(&constant, zend_string_init(value, value_length, ce->type & ZEND_INTERNAL_CLASS));
	return zephir_declare_class_constant(ce, name, name_length, &constant);
}

int zephir_declare_class_constant_string(zend_class_entry *ce, const char *name, size_t name_length, const char *value)
{
	return zephir_declare_class_constant_stringl(ce, name, name_length, value, strlen(value));
}
