
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <php.h>
#include "php_ext.h"
#include <ext/standard/php_string.h>
#include "ext.h"
#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/concat.h"

void zephir_concat_sv(zval *result, const char *op1, zend_uint op1_len, zval *op2, int self_var){

	zval result_copy, op2_copy;
	int use_copy = 0, use_copy2 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op2) != IS_STRING) {
	   use_copy2 = zend_make_printable_zval(op2, &op2_copy);
	   if (use_copy2) {
	       op2 = &op2_copy;
	   }
	}

	length = op1_len + Z_STRLEN_P(op2);
	if (self_var) {

		if (Z_TYPE_P(result) != IS_STRING) {
			use_copy = zend_make_printable_zval(result, &result_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(result, (&result_copy));
			}
		}

		offset = Z_STRLEN_P(result);
		length += offset;
		Z_STR_P(result) = zend_string_realloc(Z_STR_P(result), length, 0);

	} else {
		ZVAL_STR(result, zend_string_alloc(length, 0));
	}

	memcpy(Z_STRVAL_P(result) + offset, op1, op1_len);
	memcpy(Z_STRVAL_P(result) + offset + op1_len, Z_STRVAL_P(op2), Z_STRLEN_P(op2));
	Z_STRVAL_P(result)[length] = 0;
	zend_string_forget_hash_val(Z_STR_P(result));
	if (use_copy2) {
	   zval_dtor(op2);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_vs(zval *result, zval *op1, const char *op2, zend_uint op2_len, int self_var){

	zval result_copy, op1_copy;
	int use_copy = 0, use_copy1 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op1) != IS_STRING) {
	   use_copy1 = zend_make_printable_zval(op1, &op1_copy);
	   if (use_copy1) {
	       op1 = &op1_copy;
	   }
	}

	length = Z_STRLEN_P(op1) + op2_len;
	if (self_var) {

		if (Z_TYPE_P(result) != IS_STRING) {
			use_copy = zend_make_printable_zval(result, &result_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(result, (&result_copy));
			}
		}

		offset = Z_STRLEN_P(result);
		length += offset;
		Z_STR_P(result) = zend_string_realloc(Z_STR_P(result), length, 0);

	} else {
		ZVAL_STR(result, zend_string_alloc(length, 0));
	}

	memcpy(Z_STRVAL_P(result) + offset, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
	memcpy(Z_STRVAL_P(result) + offset + Z_STRLEN_P(op1), op2, op2_len);
	Z_STRVAL_P(result)[length] = 0;
	zend_string_forget_hash_val(Z_STR_P(result));
	if (use_copy1) {
	   zval_dtor(op1);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_vv(zval *result, zval *op1, zval *op2, int self_var){

	zval result_copy, op1_copy, op2_copy;
	int use_copy = 0, use_copy1 = 0, use_copy2 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op1) != IS_STRING) {
	   use_copy1 = zend_make_printable_zval(op1, &op1_copy);
	   if (use_copy1) {
	       op1 = &op1_copy;
	   }
	}

	if (Z_TYPE_P(op2) != IS_STRING) {
	   use_copy2 = zend_make_printable_zval(op2, &op2_copy);
	   if (use_copy2) {
	       op2 = &op2_copy;
	   }
	}

	length = Z_STRLEN_P(op1) + Z_STRLEN_P(op2);
	if (self_var) {

		if (Z_TYPE_P(result) != IS_STRING) {
			use_copy = zend_make_printable_zval(result, &result_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(result, (&result_copy));
			}
		}

		offset = Z_STRLEN_P(result);
		length += offset;
		Z_STR_P(result) = zend_string_realloc(Z_STR_P(result), length, 0);

	} else {
		ZVAL_STR(result, zend_string_alloc(length, 0));
	}

	memcpy(Z_STRVAL_P(result) + offset, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
	memcpy(Z_STRVAL_P(result) + offset + Z_STRLEN_P(op1), Z_STRVAL_P(op2), Z_STRLEN_P(op2));
	Z_STRVAL_P(result)[length] = 0;
	zend_string_forget_hash_val(Z_STR_P(result));
	if (use_copy1) {
	   zval_dtor(op1);
	}

	if (use_copy2) {
	   zval_dtor(op2);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_function(zval *result, zval *op1, zval *op2)
{
	zval tmp;
	SEPARATE_ZVAL_IF_NOT_REF(result);

	/*
		res == op1 == op2: won't leak
		res == op1 != op2: won't leak
		res == op2 != op1: will leak
	 */
	if (result == op2 && result != op1) {
		ZVAL_COPY_VALUE(&tmp, result);
		ZVAL_NULL(result);
		op2 = &tmp;
	}
	else {
		ZVAL_UNDEF(&tmp);
	}

	concat_function(result, op1, op2);
	assert(!Z_REFCOUNTED(tmp) || 1 == Z_REFCOUNT(tmp));
	zval_dtor(&tmp);
}