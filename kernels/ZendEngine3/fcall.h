
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

#ifndef ZEPHIR_KERNEL_FCALL_H
#define ZEPHIR_KERNEL_FCALL_H

#include "php_ext.h"
#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/extended/fcall.h"

#include <Zend/zend_hash.h>
#include <Zend/zend.h>

typedef enum _zephir_call_type {
	zephir_fcall_parent,
	zephir_fcall_self,
	zephir_fcall_static,
	zephir_fcall_ce,
	zephir_fcall_method,
	zephir_fcall_function
} zephir_call_type;

/**
 * @addtogroup callfuncs Calling Functions
 * @{
 */
#if defined(_MSC_VER)
#define ZEPHIR_PASS_CALL_PARAMS(x) x + 1
#define ZEPHIR_CALL_NUM_PARAMS(x) ((sizeof(x) - sizeof(x[0]))/sizeof(x[0]))
#define ZEPHIR_FETCH_VA_ARGS NULL,
#else
#define ZEPHIR_PASS_CALL_PARAMS(x) x
#define ZEPHIR_CALL_NUM_PARAMS(x) sizeof(x)/sizeof(zval*)
#define ZEPHIR_FETCH_VA_ARGS
#endif

#define ZEPHIR_CALL_FUNCTION(return_value_ptr, func_name, cache, cache_slot, ...) \
	do { \
		zephir_fcall_cache_entry **cache_entry_ = cache; \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		if (__builtin_constant_p(func_name)) { \
			if (cache_entry_ && *cache_entry_) { \
				ZEPHIR_LAST_CALL_STATUS = zephir_call_func_aparams_fast(return_value_ptr, cache, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
			} else { \
				ZEPHIR_LAST_CALL_STATUS = zephir_call_func_aparams(return_value_ptr, func_name, sizeof(func_name)-1, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
			} \
		} else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_func_aparams(return_value_ptr, func_name, strlen(func_name), cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
	} while (0)

#define ZEPHIR_CALL_ZVAL_FUNCTION(return_value_ptr, func_name, cache, cache_slot, ...) \
	do { \
        	zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
        	ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
        	ZEPHIR_LAST_CALL_STATUS = zephir_call_zval_func_aparams(return_value_ptr, func_name, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
	} while (0)

#define ZEPHIR_SET_THIS(val) \
	if (val) { ZVAL_COPY_VALUE(&EG(current_execute_data)->This, val); } \
	else { ZVAL_NULL(&EG(current_execute_data)->This); } \

#define ZEPHIR_RETURN_CALL_INTERNAL_METHOD_P0(object, method) \
	do { \
		zval *old_this_ptr = this_ptr; \
		ZEPHIR_SET_THIS(object); \
		method(0, return_value, object, 1); \
		ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
		ZEPHIR_SET_THIS(old_this_ptr); \
	} while (0)

#define ZEPHIR_RETURN_CALL_INTERNAL_METHOD_P1(object, method, p1) \
	do { \
		zval *old_this_ptr = this_ptr; \
		ZEPHIR_SET_THIS(object); \
		Z_TRY_ADDREF_P(p1); \
		method(0, return_value, object, 1, p1); \
		ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
		Z_TRY_DELREF_P(p1); \
		ZEPHIR_SET_THIS(old_this_ptr); \
	} while (0)


#define ZEPHIR_RETURN_CALL_INTERNAL_METHOD_P2(object, method, p1, p2) \
	do { \
		zend_object *old_this_ptr = Z_OBJ_P(this_ptr); \
		Z_OBJ(EG(current_execute_data)->This) = Z_OBJ_P(object); \
		Z_TRY_ADDREF_P(p1); \
		Z_TRY_ADDREF_P(p2); \
		method(0, return_value, object, 1, p1, p2); \
		Z_TRY_DELREF_P(p1); \
		Z_TRY_DELREF_P(p2); \
		ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
		Z_OBJ(EG(current_execute_data)->This) = old_this_ptr; \
	} while (0)

#define ZEPHIR_RETURN_CALL_INTERNAL_METHOD_P3(object, method, p1, p2, p3) \
	do { \
		zend_object *old_this_ptr = Z_OBJ_P(this_ptr); \
		Z_OBJ(EG(current_execute_data)->This) = Z_OBJ_P(object); \
		Z_TRY_ADDREF_P(p1); \
		Z_TRY_ADDREF_P(p2); \
		Z_TRY_ADDREF_P(p3); \
		method(0, return_value, object, 1, p1, p2, p3); \
		Z_TRY_DELREF_P(p1); \
		Z_TRY_DELREF_P(p2); \
		Z_TRY_DELREF_P(p3); \
		ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
		Z_OBJ(EG(current_execute_data)->This) = old_this_ptr; \
	} while (0)

#define ZEPHIR_RETURN_CALL_INTERNAL_METHOD_P4(object, method, p1, p2, p3, p4) \
	do { \
		zend_object *old_this_ptr = Z_OBJ_P(this_ptr); \
		Z_OBJ(EG(current_execute_data)->This) = Z_OBJ_P(object); \
		Z_TRY_ADDREF_P(p1); \
		Z_TRY_ADDREF_P(p2); \
		Z_TRY_ADDREF_P(p3); \
		Z_TRY_ADDREF_P(p4); \
		method(0, return_value, object, 1, p1, p2, p3); \
		Z_TRY_DELREF_P(p1); \
		Z_TRY_DELREF_P(p2); \
		Z_TRY_DELREF_P(p3); \
		Z_TRY_DELREF_P(p4); \
		ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
		Z_OBJ(EG(current_execute_data)->This) = old_this_ptr; \
	} while (0)

/**
  * Call a internal method using a local return value ptr, since the return value isn't used
  */
#define ZEPHIR_CALL_INTERNAL_METHOD_NORETURN_P0(object, method) \
	do { \
		zval *old_this_ptr = this_ptr; \
		zval rv; \
		zval *rvp = &rv; \
		ZVAL_UNDEF(&rv); \
		ZEPHIR_SET_THIS(object); \
		method(0, rvp, object, 0); \
		ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
		ZEPHIR_SET_THIS(old_this_ptr); \
		zval_ptr_dtor(rvp); \
	} while (0)

#define ZEPHIR_CALL_INTERNAL_METHOD_NORETURN_P1(object, method, p1) \
	do { \
		zval *old_this_ptr = this_ptr; \
		zval rv; \
		zval *rvp = &rv; \
		ZVAL_UNDEF(rvp); \
		ZEPHIR_SET_THIS(object); \
		Z_TRY_ADDREF_P(p1); \
		method(0, rvp, object, 0, p1); \
		ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
		Z_TRY_DELREF_P(p1); \
		ZEPHIR_SET_THIS(old_this_ptr); \
		zval_ptr_dtor(rvp); \
	} while (0)

#define ZEPHIR_CALL_INTERNAL_METHOD_NORETURN_P2(object, method, p1, p2) \
	do { \
		zend_object *old_this_ptr = Z_OBJ_P(this_ptr); \
		zval rv; \
		zval *rvp = &rv; \
		ZVAL_UNDEF(rvp); \
		Z_OBJ(EG(current_execute_data)->This) = Z_OBJ_P(object); \
		Z_TRY_ADDREF_P(p1); \
		Z_TRY_ADDREF_P(p2); \
		method(0, rvp, object, 0, p1, p2); \
		Z_TRY_DELREF_P(p1); \
		Z_TRY_DELREF_P(p2); \
		ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
		Z_OBJ(EG(current_execute_data)->This) = old_this_ptr; \
		zval_ptr_dtor(rvp); \
	} while (0)

#define ZEPHIR_CALL_INTERNAL_METHOD_NORETURN_P3(object, method, p1, p2, p3) \
	do { \
		zend_object *old_this_ptr = Z_OBJ_P(this_ptr); \
		zval rv; \
		zval *rvp = &rv; \
		ZVAL_UNDEF(rvp); \
		Z_OBJ(EG(current_execute_data)->This) = Z_OBJ_P(object); \
		Z_TRY_ADDREF_P(p1); \
		Z_TRY_ADDREF_P(p2); \
		Z_TRY_ADDREF_P(p3); \
		method(0, rvp, object, 0, p1, p2, p3); \
		Z_TRY_DELREF_P(p1); \
		Z_TRY_DELREF_P(p2); \
		Z_TRY_DELREF_P(p3); \
		ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
		Z_OBJ(EG(current_execute_data)->This) = old_this_ptr; \
		zval_ptr_dtor(rvp); \
	} while (0)

#define ZEPHIR_CALL_INTERNAL_METHOD_P0(return_value_ptr, object, method) \
	do { \
		zval *old_this_ptr = this_ptr; \
		ZEPHIR_SET_THIS(object); \
		ZEPHIR_INIT_NVAR(return_value_ptr); \
		method(0, return_value_ptr, object, 1); \
		ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
		ZEPHIR_SET_THIS(old_this_ptr); \
	} while (0)

#define ZEPHIR_CALL_INTERNAL_METHOD_P1(return_value_ptr, object, method, p1) \
	do { \
		zend_object *old_this_ptr = Z_OBJ_P(this_ptr); \
		Z_OBJ(EG(current_execute_data)->This) = Z_OBJ_P(object); \
		ZEPHIR_INIT_NVAR(return_value_ptr); \
		Z_TRY_ADDREF_P(p1); \
		method(0, return_value_ptr, object, 1, p1); \
		Z_TRY_DELREF_P(p1); \
		ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
		Z_OBJ(EG(current_execute_data)->This) = old_this_ptr; \
	} while (0)

#define ZEPHIR_CALL_INTERNAL_METHOD_P2(return_value_ptr, object, method, p1, p2) \
	do { \
		zend_object *old_this_ptr = Z_OBJ_P(this_ptr); \
		Z_OBJ(EG(current_execute_data)->This) = Z_OBJ_P(object); \
		ZEPHIR_INIT_NVAR(return_value_ptr); \
		Z_TRY_ADDREF_P(p1); \
		Z_TRY_ADDREF_P(p2); \
		method(0, return_value_ptr, object, 1, p1, p2); \
		Z_TRY_DELREF_P(p1); \
		Z_TRY_DELREF_P(p2); \
		ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
		Z_OBJ(EG(current_execute_data)->This) = old_this_ptr; \
	} while (0)

#define ZEPHIR_CALL_INTERNAL_METHOD_P3(return_value_ptr, object, method, p1, p2, p3) \
	do { \
		zend_object *old_this_ptr = Z_OBJ_P(this_ptr); \
		Z_OBJ(EG(current_execute_data)->This) = Z_OBJ_P(object); \
		ZEPHIR_INIT_NVAR(return_value_ptr); \
		Z_TRY_ADDREF_P(p1); \
		Z_TRY_ADDREF_P(p2); \
		Z_TRY_ADDREF_P(p3); \
		method(0, return_value_ptr, object, 1, p1, p2, p3); \
		Z_TRY_DELREF_P(p1); \
		Z_TRY_DELREF_P(p2); \
		Z_TRY_DELREF_P(p3); \
		ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
		Z_OBJ(EG(current_execute_data)->This) = old_this_ptr; \
	} while (0)

#define ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(func_name, cache, cache_slot, ...) \
	do { \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		ZEPHIR_LAST_CALL_STATUS = zephir_return_call_zval_function(return_value, func_name, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
	} while (0)

#define ZEPHIR_RETURN_CALL_FUNCTION(func_name, cache, cache_slot, ...) \
	do { \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		if (__builtin_constant_p(func_name)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_function(return_value, func_name, sizeof(func_name)-1, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_function(return_value, func_name, strlen(func_name), cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
	} while (0)

#define ZEPHIR_CALL_METHOD(return_value_ptr, object, method, cache, cache_slot, ...) \
	do { \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL, zephir_fcall_method, object, method, sizeof(method)-1, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL, zephir_fcall_method, object, method, strlen(method), cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_METHOD_ZVAL(object, method, cache, cache_slot, ...) \
	do { \
		char *method_name; \
		int method_len; \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		if (Z_TYPE_P(method) == IS_STRING) { \
			method_len = Z_STRLEN_P(method); \
			method_name = zend_str_tolower_dup(Z_STRVAL_P(method), method_len); \
		} else { \
			method_len = 0; \
			method_name = zend_str_tolower_dup("", 0); \
		} \
		ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL, zephir_fcall_method, object, method_name, method_len, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		efree(method_name); \
	} while (0)

#define ZEPHIR_CALL_METHOD_ZVAL(return_value_ptr, object, method, cache, cache_slot, ...) \
	do { \
		char *method_name; \
		int method_len; \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		if (Z_TYPE_P(method) == IS_STRING) { \
			method_len = Z_STRLEN_P(method); \
			method_name = zend_str_tolower_dup(Z_STRVAL_P(method), method_len); \
		} else { \
			method_len = 0; \
			method_name = zend_str_tolower_dup("", 0); \
		} \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL, zephir_fcall_method, object, method_name, method_len, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		efree(method_name); \
	} while (0)

#define ZEPHIR_CALL_PARENT(return_value_ptr, class_entry, this_ptr, method, cache, cache_slot, ...) \
	do { \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, class_entry, zephir_fcall_parent, this_ptr, method, sizeof(method)-1, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, class_entry, zephir_fcall_parent, this_ptr, method, strlen(method), cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_METHOD(object, method, cache, cache_slot, ...) \
	do { \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL, zephir_fcall_method, object, method, sizeof(method)-1, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL, zephir_fcall_method, object, method, strlen(method), cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_STATIC(method, cache, cache_slot, ...) \
	do { \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, NULL, zephir_fcall_static, NULL, method, sizeof(method)-1, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, NULL, zephir_fcall_static, NULL, method, strlen(method), cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_PARENT(class_entry, this_ptr, method, cache, cache_slot, ...) \
	do { \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, class_entry, zephir_fcall_parent, this_ptr, method, sizeof(method)-1, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, class_entry, zephir_fcall_parent, this_ptr, method, strlen(method), cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
	} while (0)

#define ZEPHIR_CALL_SELF(return_value_ptr, method, cache, cache_slot, ...) \
	do { \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, NULL, zephir_fcall_self, NULL, method, sizeof(method)-1, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, NULL, zephir_fcall_self, NULL, method, strlen(method), cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_SELF(method, cache, cache_slot, ...) \
	do { \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, NULL, zephir_fcall_self, NULL, method, sizeof(method)-1, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, NULL, zephir_fcall_self, NULL, method, strlen(method), cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
	} while (0)

#define ZEPHIR_CALL_STATIC(return_value_ptr, method, cache, cache_slot, ...) \
	do { \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, NULL, zephir_fcall_static, NULL, method, sizeof(method)-1, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, NULL, zephir_fcall_static, NULL, method, strlen(method), cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
	} while (0)

#define ZEPHIR_CALL_CE_STATIC(return_value_ptr, class_entry, method, cache, cache_slot, ...) \
	do { \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, class_entry, zephir_fcall_ce, NULL, method, sizeof(method)-1, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, class_entry, zephir_fcall_ce, NULL, method, strlen(method), cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_CE_STATIC(class_entry, method, cache, cache_slot, ...) \
	do { \
		zval *params_[] = {ZEPHIR_FETCH_VA_ARGS __VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, class_entry, zephir_fcall_ce, NULL, method, sizeof(method)-1, cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, class_entry, zephir_fcall_ce, NULL, method, strlen(method), cache, cache_slot, ZEPHIR_CALL_NUM_PARAMS(params_), ZEPHIR_PASS_CALL_PARAMS(params_)); \
		} \
	} while (0)

int zephir_call_func_aparams(zval *return_value_ptr, const char *func_name, uint func_length,
	zephir_fcall_cache_entry **cache_entry, int cache_slot,
	uint param_count, zval **params);
int zephir_call_func_aparams_fast(zval *return_value_ptr, zephir_fcall_cache_entry **cache_entry, uint param_count, zval **params);

int zephir_call_zval_func_aparams(zval *return_value_ptr, zval *func_name,
	zephir_fcall_cache_entry **cache_entry, int cache_slot,
	uint param_count, zval **params) ZEPHIR_ATTR_WARN_UNUSED_RESULT;

int zephir_call_class_method_aparams(zval *return_value_ptr,
	zend_class_entry *ce,
	zephir_call_type type,
	zval *object,
	const char *method_name,
	uint method_len,
	zephir_fcall_cache_entry **cache_entry,
	int cache_slot,
	uint param_count,
	zval **params) ZEPHIR_ATTR_WARN_UNUSED_RESULT;

ZEPHIR_ATTR_WARN_UNUSED_RESULT static inline int zephir_return_call_function(zval *return_value,
	const char *func, uint func_len, zephir_fcall_cache_entry **cache_entry, int cache_slot, uint param_count, zval **params)
{
	zval rv, *rvp = return_value ? return_value : &rv;
	int status;

	if (return_value) {
		zval_ptr_dtor(return_value);
		ZVAL_UNDEF(return_value);
	}

	status = zephir_call_func_aparams(rvp, func, func_len, cache_entry, cache_slot, param_count, params);

	if (status == FAILURE) {
		if (return_value && EG(exception)) {
			ZVAL_NULL(return_value);
		}

		return FAILURE;
	}

	if (!return_value) {
		zval_ptr_dtor(&rv);
	}

	return SUCCESS;
}

ZEPHIR_ATTR_WARN_UNUSED_RESULT static inline int zephir_return_call_zval_function(zval *return_value,
	zval *func, zephir_fcall_cache_entry **cache_entry, int cache_slot, uint param_count, zval **params)
{
	zval rv, *rvp = return_value ? return_value : &rv;
	int status;

	if (return_value) {
		zval_ptr_dtor(return_value);
		ZVAL_UNDEF(return_value);
	}

	status = zephir_call_zval_func_aparams(rvp, func, cache_entry, cache_slot, param_count, params);

	if (status == FAILURE) {
		if (return_value && EG(exception)) {
			ZVAL_NULL(return_value);
		}

		return FAILURE;
	}

	if (!return_value) {
		zval_ptr_dtor(&rv);
	}

	return SUCCESS;
}

ZEPHIR_ATTR_WARN_UNUSED_RESULT static inline int zephir_return_call_class_method(zval *return_value,
	zend_class_entry *ce, zephir_call_type type, zval *object,
	const char *method_name, uint method_len,
	zephir_fcall_cache_entry **cache_entry, int cache_slot,
	uint param_count, zval **params)
{
	zval rv, *rvp = return_value ? return_value : &rv;
	int status;

	ZVAL_UNDEF(&rv);

	if (return_value) {
		zval_ptr_dtor(return_value);
		ZVAL_UNDEF(return_value);
	}

	status = zephir_call_class_method_aparams(rvp, ce, type, object, method_name, method_len, cache_entry, cache_slot, param_count, params);

	if (status == FAILURE) {
		if (return_value && EG(exception)) {
			ZVAL_NULL(return_value);
		}

		return FAILURE;
	}

	if (!return_value) {
		zval_ptr_dtor(&rv);
	}

	return SUCCESS;
}

int zephir_has_constructor_ce(const zend_class_entry *ce) ZEPHIR_ATTR_PURE ZEPHIR_ATTR_NONNULL;

ZEPHIR_ATTR_WARN_UNUSED_RESULT ZEPHIR_ATTR_NONNULL static inline int zephir_has_constructor(const zval *object)
{
	return Z_TYPE_P(object) == IS_OBJECT ? zephir_has_constructor_ce(Z_OBJCE_P(object)) : 0;
}

#define zephir_check_call_status() \
	do \
		if (ZEPHIR_LAST_CALL_STATUS == FAILURE) { \
			ZEPHIR_MM_RESTORE(); \
			return; \
	} \
	while(0)

#define zephir_check_call_status_or_jump(label) \
	if (ZEPHIR_LAST_CALL_STATUS == FAILURE) { \
		if (EG(exception)) { \
			goto label; \
		} else { \
			ZEPHIR_MM_RESTORE(); \
			return; \
		} \
	}

#ifdef ZEPHIR_RELEASE
#define ZEPHIR_TEMP_PARAM_COPY 0
#define zephir_check_temp_parameter(param) do { if (Z_REFCOUNT(param) > 1) zval_copy_ctor(&param); else ZVAL_NULL(&param); } while(0)
#else
#define ZEPHIR_TEMP_PARAM_COPY 1
#define zephir_check_temp_parameter(param)
#endif

void zephir_eval_php(zval *str, zval *retval_ptr, char *context);

#endif /* ZEPHIR_KERNEL_FCALL_H */
