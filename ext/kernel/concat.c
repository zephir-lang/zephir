
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

void zephir_concat_ss(zval *result, const char *op1, uint32_t op1_len, const char *op2, uint32_t op2_len, int self_var){

	zval result_copy;
	int use_copy = 0;
	size_t offset = 0, length;

	length = op1_len + op2_len;
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
	memcpy(Z_STRVAL_P(result) + offset + op1_len, op2, op2_len);
	Z_STRVAL_P(result)[length] = 0;
	zend_string_forget_hash_val(Z_STR_P(result));
	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_sssssss(zval *result, const char *op1, uint32_t op1_len, const char *op2, uint32_t op2_len, const char *op3, uint32_t op3_len, const char *op4, uint32_t op4_len, const char *op5, uint32_t op5_len, const char *op6, uint32_t op6_len, const char *op7, uint32_t op7_len, int self_var){

	zval result_copy;
	int use_copy = 0;
	size_t offset = 0, length;

	length = op1_len + op2_len + op3_len + op4_len + op5_len + op6_len + op7_len;
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
	memcpy(Z_STRVAL_P(result) + offset + op1_len, op2, op2_len);
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len, op3, op3_len);
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len + op3_len, op4, op4_len);
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len + op3_len + op4_len, op5, op5_len);
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len + op3_len + op4_len + op5_len, op6, op6_len);
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len + op3_len + op4_len + op5_len + op6_len, op7, op7_len);
	Z_STRVAL_P(result)[length] = 0;
	zend_string_forget_hash_val(Z_STR_P(result));
	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_ssv(zval *result, const char *op1, uint32_t op1_len, const char *op2, uint32_t op2_len, zval *op3, int self_var){

	zval result_copy, op3_copy;
	int use_copy = 0, use_copy3 = 0;
	size_t offset = 0, length;

	if (Z_TYPE_P(op3) != IS_STRING) {
	   use_copy3 = zend_make_printable_zval(op3, &op3_copy);
	   if (use_copy3) {
	       op3 = &op3_copy;
	   }
	}

	length = op1_len + op2_len + Z_STRLEN_P(op3);
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
	memcpy(Z_STRVAL_P(result) + offset + op1_len, op2, op2_len);
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len, Z_STRVAL_P(op3), Z_STRLEN_P(op3));
	Z_STRVAL_P(result)[length] = 0;
	zend_string_forget_hash_val(Z_STR_P(result));
	if (use_copy3) {
	   zval_dtor(op3);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_ssvs(zval *result, const char *op1, uint32_t op1_len, const char *op2, uint32_t op2_len, zval *op3, const char *op4, uint32_t op4_len, int self_var){

	zval result_copy, op3_copy;
	int use_copy = 0, use_copy3 = 0;
	size_t offset = 0, length;

	if (Z_TYPE_P(op3) != IS_STRING) {
	   use_copy3 = zend_make_printable_zval(op3, &op3_copy);
	   if (use_copy3) {
	       op3 = &op3_copy;
	   }
	}

	length = op1_len + op2_len + Z_STRLEN_P(op3) + op4_len;
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
	memcpy(Z_STRVAL_P(result) + offset + op1_len, op2, op2_len);
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len, Z_STRVAL_P(op3), Z_STRLEN_P(op3));
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len + Z_STRLEN_P(op3), op4, op4_len);
	Z_STRVAL_P(result)[length] = 0;
	zend_string_forget_hash_val(Z_STR_P(result));
	if (use_copy3) {
	   zval_dtor(op3);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_ssvsvssvs(zval *result, const char *op1, uint32_t op1_len, const char *op2, uint32_t op2_len, zval *op3, const char *op4, uint32_t op4_len, zval *op5, const char *op6, uint32_t op6_len, const char *op7, uint32_t op7_len, zval *op8, const char *op9, uint32_t op9_len, int self_var){

	zval result_copy, op3_copy, op5_copy, op8_copy;
	int use_copy = 0, use_copy3 = 0, use_copy5 = 0, use_copy8 = 0;
	size_t offset = 0, length;

	if (Z_TYPE_P(op3) != IS_STRING) {
	   use_copy3 = zend_make_printable_zval(op3, &op3_copy);
	   if (use_copy3) {
	       op3 = &op3_copy;
	   }
	}

	if (Z_TYPE_P(op5) != IS_STRING) {
	   use_copy5 = zend_make_printable_zval(op5, &op5_copy);
	   if (use_copy5) {
	       op5 = &op5_copy;
	   }
	}

	if (Z_TYPE_P(op8) != IS_STRING) {
	   use_copy8 = zend_make_printable_zval(op8, &op8_copy);
	   if (use_copy8) {
	       op8 = &op8_copy;
	   }
	}

	length = op1_len + op2_len + Z_STRLEN_P(op3) + op4_len + Z_STRLEN_P(op5) + op6_len + op7_len + Z_STRLEN_P(op8) + op9_len;
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
	memcpy(Z_STRVAL_P(result) + offset + op1_len, op2, op2_len);
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len, Z_STRVAL_P(op3), Z_STRLEN_P(op3));
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len + Z_STRLEN_P(op3), op4, op4_len);
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len + Z_STRLEN_P(op3) + op4_len, Z_STRVAL_P(op5), Z_STRLEN_P(op5));
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len + Z_STRLEN_P(op3) + op4_len + Z_STRLEN_P(op5), op6, op6_len);
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len + Z_STRLEN_P(op3) + op4_len + Z_STRLEN_P(op5) + op6_len, op7, op7_len);
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len + Z_STRLEN_P(op3) + op4_len + Z_STRLEN_P(op5) + op6_len + op7_len, Z_STRVAL_P(op8), Z_STRLEN_P(op8));
	memcpy(Z_STRVAL_P(result) + offset + op1_len + op2_len + Z_STRLEN_P(op3) + op4_len + Z_STRLEN_P(op5) + op6_len + op7_len + Z_STRLEN_P(op8), op9, op9_len);
	Z_STRVAL_P(result)[length] = 0;
	zend_string_forget_hash_val(Z_STR_P(result));
	if (use_copy3) {
	   zval_dtor(op3);
	}

	if (use_copy5) {
	   zval_dtor(op5);
	}

	if (use_copy8) {
	   zval_dtor(op8);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_sv(zval *result, const char *op1, uint32_t op1_len, zval *op2, int self_var){

	zval result_copy, op2_copy;
	int use_copy = 0, use_copy2 = 0;
	size_t offset = 0, length;

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

void zephir_concat_svs(zval *result, const char *op1, uint32_t op1_len, zval *op2, const char *op3, uint32_t op3_len, int self_var){

	zval result_copy, op2_copy;
	int use_copy = 0, use_copy2 = 0;
	size_t offset = 0, length;

	if (Z_TYPE_P(op2) != IS_STRING) {
	   use_copy2 = zend_make_printable_zval(op2, &op2_copy);
	   if (use_copy2) {
	       op2 = &op2_copy;
	   }
	}

	length = op1_len + Z_STRLEN_P(op2) + op3_len;
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
	memcpy(Z_STRVAL_P(result) + offset + op1_len + Z_STRLEN_P(op2), op3, op3_len);
	Z_STRVAL_P(result)[length] = 0;
	zend_string_forget_hash_val(Z_STR_P(result));
	if (use_copy2) {
	   zval_dtor(op2);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_svsv(zval *result, const char *op1, uint32_t op1_len, zval *op2, const char *op3, uint32_t op3_len, zval *op4, int self_var){

	zval result_copy, op2_copy, op4_copy;
	int use_copy = 0, use_copy2 = 0, use_copy4 = 0;
	size_t offset = 0, length;

	if (Z_TYPE_P(op2) != IS_STRING) {
	   use_copy2 = zend_make_printable_zval(op2, &op2_copy);
	   if (use_copy2) {
	       op2 = &op2_copy;
	   }
	}

	if (Z_TYPE_P(op4) != IS_STRING) {
	   use_copy4 = zend_make_printable_zval(op4, &op4_copy);
	   if (use_copy4) {
	       op4 = &op4_copy;
	   }
	}

	length = op1_len + Z_STRLEN_P(op2) + op3_len + Z_STRLEN_P(op4);
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
	memcpy(Z_STRVAL_P(result) + offset + op1_len + Z_STRLEN_P(op2), op3, op3_len);
	memcpy(Z_STRVAL_P(result) + offset + op1_len + Z_STRLEN_P(op2) + op3_len, Z_STRVAL_P(op4), Z_STRLEN_P(op4));
	Z_STRVAL_P(result)[length] = 0;
	zend_string_forget_hash_val(Z_STR_P(result));
	if (use_copy2) {
	   zval_dtor(op2);
	}

	if (use_copy4) {
	   zval_dtor(op4);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_vs(zval *result, zval *op1, const char *op2, uint32_t op2_len, int self_var){

	zval result_copy, op1_copy;
	int use_copy = 0, use_copy1 = 0;
	size_t offset = 0, length;

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
	size_t offset = 0, length;

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

void zephir_concat_vvs(zval *result, zval *op1, zval *op2, const char *op3, uint32_t op3_len, int self_var){

	zval result_copy, op1_copy, op2_copy;
	int use_copy = 0, use_copy1 = 0, use_copy2 = 0;
	size_t offset = 0, length;

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

	length = Z_STRLEN_P(op1) + Z_STRLEN_P(op2) + op3_len;
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
	memcpy(Z_STRVAL_P(result) + offset + Z_STRLEN_P(op1) + Z_STRLEN_P(op2), op3, op3_len);
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

void zephir_concat_vvv(zval *result, zval *op1, zval *op2, zval *op3, int self_var){

	zval result_copy, op1_copy, op2_copy, op3_copy;
	int use_copy = 0, use_copy1 = 0, use_copy2 = 0, use_copy3 = 0;
	size_t offset = 0, length;

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

	if (Z_TYPE_P(op3) != IS_STRING) {
	   use_copy3 = zend_make_printable_zval(op3, &op3_copy);
	   if (use_copy3) {
	       op3 = &op3_copy;
	   }
	}

	length = Z_STRLEN_P(op1) + Z_STRLEN_P(op2) + Z_STRLEN_P(op3);
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
	memcpy(Z_STRVAL_P(result) + offset + Z_STRLEN_P(op1) + Z_STRLEN_P(op2), Z_STRVAL_P(op3), Z_STRLEN_P(op3));
	Z_STRVAL_P(result)[length] = 0;
	zend_string_forget_hash_val(Z_STR_P(result));
	if (use_copy1) {
	   zval_dtor(op1);
	}

	if (use_copy2) {
	   zval_dtor(op2);
	}

	if (use_copy3) {
	   zval_dtor(op3);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_function(zval *result, zval *op1, zval *op2)
{
	zval tmp;
	SEPARATE_ZVAL(result);

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