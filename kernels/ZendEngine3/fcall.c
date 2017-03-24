
/*
  +------------------------------------------------------------------------+
  | Zephir Language                                                        |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2016 Zephir Team (http://www.zephir-lang.com)       |
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

#include <php.h>
#include "php_ext.h"

#include <Zend/zend_API.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_execute.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/hash.h"
#include "kernel/operators.h"
#include "kernel/exception.h"
#include "kernel/backtrace.h"
#include "kernel/variables.h"

static const unsigned char tolower_map[256] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

int zephir_has_constructor_ce(const zend_class_entry *ce)
{
	while (ce) {
		if (ce->constructor != NULL) {
			return 1;
		}
		ce = ce->parent;
	}
	return 0;
}

/**
 * Creates a unique key to cache the current method/function call address for the current scope
 */
static ulong zephir_make_fcall_key(char **result, size_t *length, const zend_class_entry *obj_ce, zephir_call_type type, zval *function_name)
{
	const zend_class_entry *calling_scope;
	char *buf = NULL, *c;
	size_t l = 0, len = 0;
	const size_t ppzce_size = sizeof(zend_class_entry**);
	ulong hash = 5381;

#if PHP_VERSION_ID >= 70100
	calling_scope = zend_get_executed_scope();
#else
	calling_scope = EG(scope);
#endif
	*result = NULL;
	*length = 0;

	if (calling_scope && type == zephir_fcall_parent) {
		calling_scope = calling_scope->parent;
		if (UNEXPECTED(!calling_scope)) {
			return 0;
		}
	}
	else if (type == zephir_fcall_static) {
#if PHP_VERSION_ID >= 70100
		calling_scope = zend_get_called_scope(EG(current_execute_data));
#else
		calling_scope = EG(current_execute_data)->called_scope;
#endif
		if (UNEXPECTED(!calling_scope)) {
			return 0;
		}
	}

	if (
		    calling_scope
		 && obj_ce
		 && calling_scope != obj_ce
		 && !instanceof_function(obj_ce, calling_scope)
		 && !instanceof_function(calling_scope, obj_ce)
	) {
		calling_scope = NULL;
	}

	if (Z_TYPE_P(function_name) == IS_STRING) {
		l   = (size_t)(Z_STRLEN_P(function_name)) + 1;
		c   = Z_STRVAL_P(function_name);
		len = 2 * ppzce_size + l + 1;
		buf = emalloc(len);

		memcpy(buf,                  c,               l);
		memcpy(buf + l,              &calling_scope,  ppzce_size);
		memcpy(buf + l + ppzce_size, &obj_ce,         ppzce_size);
		buf[len - 1] = '\0';
	}
	else if (Z_TYPE_P(function_name) == IS_ARRAY) {
		zval *method;
		HashTable *function_hash = Z_ARRVAL_P(function_name);
		if (
			    function_hash->nNumOfElements == 2
			 && ((method = zend_hash_index_find(function_hash, 1)) != NULL)
			 && Z_TYPE_P(method) == IS_STRING
		) {
			l   = (size_t)(Z_STRLEN_P(method)) + 1;
			c   = Z_STRVAL_P(method);
			len = 2 * ppzce_size + l + 1;
			buf = emalloc(len);

			memcpy(buf,                  c,               l);
			memcpy(buf + l,              &calling_scope,  ppzce_size);
			memcpy(buf + l + ppzce_size, &obj_ce,         ppzce_size);
			buf[len - 1] = '\0';
		}
	}

	if (EXPECTED(buf != NULL)) {
		size_t i;

		for (i = 0; i < l; ++i) {
			char c = buf[i];
			c = tolower_map[(unsigned char)c];
			buf[i] = c;
			hash   = (hash << 5) + hash + c;
		}

		for (i = l; i < len; ++i) {
			char c = buf[i];
			hash = (hash << 5) + hash + c;
		}
	}

	*result = buf;
	*length = len;
	return hash;
}

/**
 * Calls a function/method in the PHP userland
 */
int zephir_call_user_function(zval *object_pp, zend_class_entry *obj_ce, zephir_call_type type,
	zval *function_name, zval *retval_ptr, zephir_fcall_cache_entry **cache_entry, int cache_slot, zend_uint param_count,
	zval *params[])
{
	zval local_retval_ptr;
	int status;
	zend_fcall_info fci;
	zend_fcall_info_cache fcic;
	zend_zephir_globals_def *zephir_globals_ptr = ZEPHIR_VGLOBAL;
	char *fcall_key = NULL;
	ulong fcall_key_hash;
	size_t fcall_key_len;
	zephir_fcall_cache_entry *temp_cache_entry = NULL;
	zend_class_entry *old_scope;
	int reload_cache = 1, i;

#if PHP_VERSION_ID < 70100
	old_scope = EG(scope);
#endif

	assert(obj_ce || !object_pp);
	ZVAL_UNDEF(&local_retval_ptr);

	if (retval_ptr) {
		zval_ptr_dtor(retval_ptr);
		ZVAL_UNDEF(retval_ptr);
	}

	++zephir_globals_ptr->recursive_lock;

	if (UNEXPECTED(zephir_globals_ptr->recursive_lock > 2048)) {
		zend_error(E_ERROR, "Maximum recursion depth exceeded");
		return FAILURE;
	}

	if (obj_ce) {
#if PHP_VERSION_ID < 70100
		EG(scope) = obj_ce;
#endif
	}

	if ((!cache_entry || !*cache_entry) && zephir_globals_ptr->cache_enabled) {
		if (cache_slot > 0) {
			if (zephir_globals_ptr->scache[cache_slot]) {
				reload_cache = 0;
				temp_cache_entry = zephir_globals_ptr->scache[cache_slot];
				if (cache_entry) {
					*cache_entry = temp_cache_entry;
				}
			}
		}

		if (reload_cache) {
			fcall_key_hash = zephir_make_fcall_key(&fcall_key, &fcall_key_len, (object_pp && type != zephir_fcall_ce ? Z_OBJCE_P(object_pp) : obj_ce), type, function_name);
		}
	}

	fci.size           = sizeof(fci);
#if PHP_VERSION_ID < 70100
	fci.function_table = obj_ce ? &obj_ce->function_table : EG(function_table);
	fci.symbol_table   = NULL;
#endif
	fci.object         = object_pp ? Z_OBJ_P(object_pp) : NULL;
	ZVAL_COPY_VALUE(&fci.function_name, function_name);
	fci.retval         = retval_ptr ? retval_ptr : &local_retval_ptr;
	fci.param_count    = param_count;
	fci.params         = NULL;
	/* fci.params: Passed as separate parameter to prevent the need to convert zval ** to zval * */
	fci.no_separation  = 1;

	if (cache_entry && *cache_entry) {
	/* We have a cache record, initialize scope */
		fcic.initialized   = 1;
		fcic.calling_scope = obj_ce;

		if (object_pp) {
			fcic.called_scope = Z_OBJCE_P(object_pp);
		}
		else {
			zend_class_entry *called_scope;
#if PHP_VERSION_ID >= 70100
			called_scope = zend_get_called_scope(EG(current_execute_data));
#else
			called_scope = EG(current_execute_data)->called_scope;
#endif

			if (obj_ce && (!called_scope || instanceof_function(called_scope, obj_ce))) {
				fcic.called_scope = obj_ce;
			}
			else {
				fcic.called_scope = called_scope;
			}
		}

		fcic.object = object_pp ? Z_OBJ_P(object_pp) : NULL;

#ifndef ZEPHIR_RELEASE
		fcic.function_handler = (*cache_entry)->f;
		++(*cache_entry)->times;
#else
		fcic.function_handler = *cache_entry;
#endif
	}
	else if ((cache_entry && !*cache_entry) || zephir_globals_ptr->cache_enabled) {
	/* The caller is interested in caching OR we have the call cache enabled */
		zend_string *callable_name = NULL;
		char *error = NULL;

		zend_is_callable_ex(&fci.function_name, fci.object, IS_CALLABLE_CHECK_SILENT, &callable_name, &fcic, &error);
		if (error)         { efree(error);                       }
		if (callable_name) { zend_string_release(callable_name); }
	}
	else {
		fcic.initialized = 0;
	}

	zval p[fci.param_count];
	for (i=0; i<fci.param_count; ++i) {
		ZVAL_COPY_VALUE(&p[i], params[i]);
	}

	fci.params = p;

#if 0
//	fcic.initialized = 0;
	zval tmp;
	zephir_var_export_ex(&tmp, function_name);
	if (obj_ce) {
		fprintf(stderr, "obj_ce: %s\n", ZSTR_VAL(obj_ce->name));
	}
	fprintf(stderr, "> %s called: %p, calling: %p, obj: %p, h: %p, type=%d\n", Z_STRVAL(tmp), fcic.called_scope, fcic.calling_scope, fcic.object, fcic.function_handler, (int)type);
#endif
	status = zend_call_function(&fci, &fcic);
#if 0
	fprintf(stderr, "< called: %p, calling: %p, obj: %p, h: %p\n", fcic.called_scope, fcic.calling_scope, fcic.object, fcic.function_handler);
#endif

#if PHP_VERSION_ID < 70100
	EG(scope) = old_scope;
#endif

	/* Skip caching IF:
	 * call failed OR there was an exception (to be safe) OR cache key is not defined OR
	 * fcall cache was deinitialized OR we have a slot cache
	 */
	if (EXPECTED(status != FAILURE) && !EG(exception) && fcall_key && fcic.initialized && !temp_cache_entry) {
		int add_failed = 0;
#ifndef ZEPHIR_RELEASE
		zephir_fcall_cache_entry *cache_entry_temp = malloc(sizeof(zephir_fcall_cache_entry));
		cache_entry_temp->f     = fcic.function_handler;
		cache_entry_temp->times = 0;
#else
		zephir_fcall_cache_entry *cache_entry_temp = fcic.function_handler;
#endif

		if (zephir_globals_ptr->cache_enabled) {
			/** TODO: maybe construct zend_string (we do have a valid fcall_key) to avoid hash recalculation */
			if (NULL == zend_hash_str_add_ptr(zephir_globals_ptr->fcache, fcall_key, fcall_key_len, cache_entry_temp)) {
				add_failed = 1;
			}
		}

		if (cache_entry) {
			if (cache_slot > 0) {
				*cache_entry = cache_entry_temp;
				zephir_globals_ptr->scache[cache_slot] = *cache_entry;
			}
		}
		else if (add_failed) {
#ifndef ZEPHIR_RELEASE
			free(cache_entry_temp);
#endif
		}
	}

	if (fcall_key) {
		efree(fcall_key);
	}

	if (!retval_ptr) {
		zval_ptr_dtor(&local_retval_ptr);
	}

	--zephir_globals_ptr->recursive_lock;
	return status;
}

int zephir_call_func_aparams(zval *return_value_ptr, const char *func_name, uint func_length,
	zephir_fcall_cache_entry **cache_entry, int cache_slot,
	uint param_count, zval **params)
{
	int status;
	zval rv, *rvp = return_value_ptr ? return_value_ptr : &rv;

	ZVAL_UNDEF(&rv);

#ifndef ZEPHIR_RELEASE
	if (return_value_ptr != NULL && Z_TYPE_P(return_value_ptr) > IS_NULL) {
		fprintf(stderr, "%s: *return_value_ptr must be NULL\n", __func__);
		zephir_print_backtrace();
		abort();
	}
#endif

	zval f;
	ZVAL_STRINGL(&f, func_name, func_length);
	status = zephir_call_user_function(NULL, NULL, zephir_fcall_function, &f, rvp, cache_entry, cache_slot, param_count, params);
	zval_ptr_dtor(&f);

	if (status == FAILURE && !EG(exception)) {
		zephir_throw_exception_format(spl_ce_RuntimeException, "Call to undefined function %s()", func_name);
		if (return_value_ptr) {
			zval_ptr_dtor(return_value_ptr);
			ZVAL_UNDEF(return_value_ptr);
		}
	} else {
		if (EG(exception)) {
			status = FAILURE;
			if (return_value_ptr) {
				zval_ptr_dtor(return_value_ptr);
				ZVAL_UNDEF(return_value_ptr);
			}
		}
	}

	if (!return_value_ptr) {
		zval_ptr_dtor(&rv);
	}

	return status;
}

int zephir_call_zval_func_aparams(zval *return_value_ptr, zval *func_name,
	zephir_fcall_cache_entry **cache_entry, int cache_slot,
	uint param_count, zval **params)
{
	int status;
	zval rv, *rvp = return_value_ptr ? return_value_ptr : &rv;

	ZVAL_UNDEF(&rv);

#ifndef ZEPHIR_RELEASE
	if (return_value_ptr != NULL && Z_TYPE_P(return_value_ptr) > IS_NULL) {
		fprintf(stderr, "%s: *return_value_ptr must be NULL\n", __func__);
		zephir_print_backtrace();
		abort();
	}
#endif

	status = zephir_call_user_function(NULL, NULL, zephir_fcall_function, func_name, rvp, cache_entry, cache_slot, param_count, params);

	if (status == FAILURE && !EG(exception)) {
		zephir_throw_exception_format(spl_ce_RuntimeException, "Call to undefined function %s()", Z_TYPE_P(func_name) ? Z_STRVAL_P(func_name) : "undefined");
		if (return_value_ptr) {
			zval_ptr_dtor(return_value_ptr);
			ZVAL_NULL(return_value_ptr);
		}
	} else {
		if (EG(exception)) {
			status = FAILURE;
			if (return_value_ptr) {
				zval_ptr_dtor(return_value_ptr);
				ZVAL_NULL(return_value_ptr);
			}
		}
	}

	if (!return_value_ptr) {
		zval_ptr_dtor(&rv);
	}

	return status;
}

int zephir_call_class_method_aparams(zval *return_value_ptr, zend_class_entry *ce, zephir_call_type type, zval *object,
	const char *method_name, uint method_len,
	zephir_fcall_cache_entry **cache_entry, int cache_slot,
	uint param_count, zval **params)
{
	zval rv, *rvp = return_value_ptr ? return_value_ptr : &rv;
	zval *fn = NULL;
	int status;

	ZVAL_UNDEF(&rv);

#ifndef ZEPHIR_RELEASE
	if (return_value_ptr != NULL && Z_TYPE_P(return_value_ptr) > IS_NULL) {
		fprintf(stderr, "%s: *return_value_ptr must be NULL\n", __func__);
		zephir_print_backtrace();
		abort();
	}
#endif

	if (object) {
		if (Z_TYPE_P(object) != IS_OBJECT) {
			zephir_throw_exception_format(spl_ce_RuntimeException TSRMLS_CC, "Trying to call method %s on a non-object", method_name);
			if (return_value_ptr) {
				zval_ptr_dtor(return_value_ptr);
				ZVAL_UNDEF(return_value_ptr);
			}
			return FAILURE;
		}
	}

	zval method;
	array_init_size(&method, 2);

	switch (type) {

		case zephir_fcall_parent:
			add_next_index_string(&method, "parent");
			break;

		case zephir_fcall_self:
			add_next_index_string(&method, "self");
			break;

		case zephir_fcall_static:
			add_next_index_string(&method, "static");
			break;

		case zephir_fcall_ce:
			add_next_index_string(&method, ZSTR_VAL(ce->name));
			break;

		default:
			assert(object);
			Z_TRY_ADDREF_P(object);
			add_next_index_zval(&method, object);
			break;

	}

	add_next_index_stringl(&method, method_name, method_len);

	status = zephir_call_user_function(object, ce, type, &method, rvp, cache_entry, cache_slot, param_count, params);
	zval_ptr_dtor(&method);

	if (status == FAILURE && !EG(exception)) {
		// @TODO: Cleanup ZendEngine2 Backend
		switch (type) {
			case zephir_fcall_parent:
				zephir_throw_exception_format(spl_ce_RuntimeException, "Call to undefined method parent::%s()", method_name);
				break;

			case zephir_fcall_self:
				zephir_throw_exception_format(spl_ce_RuntimeException, "Call to undefined method self::%s()", method_name);
				break;

			case zephir_fcall_static:
				zephir_throw_exception_format(spl_ce_RuntimeException, "Call to undefined method static::%s()", method_name);
				break;

			case zephir_fcall_ce:
				zephir_throw_exception_format(spl_ce_RuntimeException, "Call to undefined method %s::%s()", ce->name, method_name);
				break;

			case zephir_fcall_method:
				zephir_throw_exception_format(spl_ce_RuntimeException, "Call to undefined method %s::%s()", ce->name, method_name);
				break;

			default:
				zephir_throw_exception_format(spl_ce_RuntimeException, "Call to undefined method ?::%s()", method_name);
		}

		if (return_value_ptr) {
			zval_ptr_dtor(return_value_ptr);
			ZVAL_UNDEF(return_value_ptr);
		}
	} else {
		if (EG(exception)) {
			status = FAILURE;
			if (return_value_ptr) {
				zval_ptr_dtor(return_value_ptr);
				ZVAL_UNDEF(return_value_ptr);
			}
		}
	}

	if (!return_value_ptr) {
		zval_ptr_dtor(&rv);
	}

	return status;
}

/**
 * Replaces call_user_func_array avoiding function lookup
 * This function does not return FAILURE if an exception has ocurred
 */
int zephir_call_user_func_array_noex(zval *return_value, zval *handler, zval *params)
{
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;
	char *is_callable_error = NULL;
	int status = FAILURE;

	if (params && Z_TYPE_P(params) != IS_ARRAY) {
		ZVAL_NULL(return_value);
		php_error_docref(NULL, E_WARNING, "Invalid arguments supplied for zephir_call_user_func_array_noex()");
		return FAILURE;
	}

	zend_fcall_info_init(handler, 0, &fci, &fci_cache, NULL, &is_callable_error);

	if (is_callable_error) {
		zend_error(E_WARNING, "%s", is_callable_error);
		efree(is_callable_error);
	} else {
		status = SUCCESS;
	}

	if (status == SUCCESS) {
		zend_fcall_info_args(&fci, params);

		fci.retval = return_value;
		zend_call_function(&fci, &fci_cache);

		zend_fcall_info_args_clear(&fci, 1);
	}

	if (EG(exception)) {
		status = SUCCESS;
	}

	return status;
}

/**
 * If a retval_ptr is specified, PHP's implementation of zend_eval_stringl
 * simply prepends a "return " which causes only the first statement to be executed
 */
void zephir_eval_php(zval *str, zval *retval_ptr, char *context)
{
	zval local_retval;
	zend_op_array *new_op_array = NULL;
	uint32_t original_compiler_options;

	ZVAL_UNDEF(&local_retval);

	original_compiler_options = CG(compiler_options);
	CG(compiler_options) = ZEND_COMPILE_DEFAULT_FOR_EVAL;
	new_op_array = zend_compile_string(str, context);
	CG(compiler_options) = original_compiler_options;

	if (new_op_array)
	{
		EG(no_extensions) = 1;
		zend_try {
			zend_execute(new_op_array, &local_retval);
		} zend_catch {
			destroy_op_array(new_op_array);
			efree_size(new_op_array, sizeof(zend_op_array));
			zend_bailout();
		} zend_end_try();
		EG(no_extensions) = 0;

		if (Z_TYPE(local_retval) != IS_UNDEF) {
			if (retval_ptr) {
				ZVAL_COPY_VALUE(retval_ptr, &local_retval);
			} else {
				zval_ptr_dtor(&local_retval);
			}
		} else if (retval_ptr) {
			ZVAL_NULL(retval_ptr);
		}

		destroy_op_array(new_op_array);
		efree_size(new_op_array, sizeof(zend_op_array));
	}
}
