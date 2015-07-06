
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
  +------------------------------------------------------------------------+
*/

#include <php.h>
#include "php_ext.h"

#include <Zend/zend_API.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_execute.h>
#include <Zend/zend_generators.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/hash.h"
#include "kernel/string.h"
#include "kernel/operators.h"
#include "kernel/exception.h"
#include "kernel/backtrace.h"

int zephir_call_func_aparams_fast(zval *return_value_ptr, zephir_fcall_cache_entry **cache_entry, zend_uint param_count, zval *params[])
{
	uint32_t i;
	zend_class_entry *calling_scope = NULL;
	zend_execute_data *call, dummy_execute_data;
	zval retval_local;
	zval *retval_ptr = return_value_ptr ? return_value_ptr : &retval_local;
	zend_class_entry *orig_scope;
	zend_function *func;

	if (return_value_ptr) {
		zval_ptr_dtor(return_value_ptr);
		ZVAL_UNDEF(return_value_ptr);
	} else {
		ZVAL_UNDEF(&retval_local);
	}

	if (!EG(active)) {
		return FAILURE; /* executor is already inactive */
	}

	if (EG(exception)) {
		return FAILURE; /* we would result in an instable executor otherwise */
	}

	orig_scope = EG(scope);

	/* Initialize execute_data */
	if (!EG(current_execute_data)) {
		/* This only happens when we're called outside any execute()'s
		 * It shouldn't be strictly necessary to NULL execute_data out,
		 * but it may make bugs easier to spot
		 */
		memset(&dummy_execute_data, 0, sizeof(zend_execute_data));
		EG(current_execute_data) = &dummy_execute_data;
	} else if (EG(current_execute_data)->func &&
	           ZEND_USER_CODE(EG(current_execute_data)->func->common.type) &&
	           EG(current_execute_data)->opline->opcode != ZEND_DO_FCALL &&
	           EG(current_execute_data)->opline->opcode != ZEND_DO_ICALL &&
	           EG(current_execute_data)->opline->opcode != ZEND_DO_UCALL &&
	           EG(current_execute_data)->opline->opcode != ZEND_DO_FCALL_BY_NAME) {
		/* Insert fake frame in case of include or magic calls */
		dummy_execute_data = *EG(current_execute_data);
		dummy_execute_data.prev_execute_data = EG(current_execute_data);
		dummy_execute_data.call = NULL;
		dummy_execute_data.opline = NULL;
		dummy_execute_data.func = NULL;
		EG(current_execute_data) = &dummy_execute_data;
	}

#ifndef ZEPHIR_RELEASE
	func = (*cache_entry)->f;
	++(*cache_entry)->times;
#else
	func = *cache_entry;
#endif

	calling_scope = NULL;
	call = zend_vm_stack_push_call_frame(ZEND_CALL_TOP_FUNCTION, func, param_count, NULL, NULL);

	for (i = 0; i < param_count; i++) {
		zval *param;
		zval *arg = params[i];

		if (ARG_SHOULD_BE_SENT_BY_REF(func, i + 1)) {
			if (!Z_ISREF_P(arg)) {
				if (!ARG_MAY_BE_SENT_BY_REF(func, i + 1)) {
					if (i) {
						/* hack to clean up the stack */
						ZEND_CALL_NUM_ARGS(call) = i;
						zend_vm_stack_free_args(call);
					}
					zend_vm_stack_free_call_frame(call);

					zend_error(E_WARNING, "Parameter %d to %s%s%s() expected to be a reference, value given",
						i+1,
						func->common.scope ? func->common.scope->name->val : "",
						func->common.scope ? "::" : "",
						func->common.function_name->val);
					if (EG(current_execute_data) == &dummy_execute_data) {
						EG(current_execute_data) = dummy_execute_data.prev_execute_data;
					}
					return FAILURE;
				}

				ZVAL_NEW_REF(arg, arg);
			}
			Z_ADDREF_P(arg);
		} else {
			if (Z_ISREF_P(arg) &&
			    !(func->common.fn_flags & ZEND_ACC_CALL_VIA_TRAMPOLINE)) {
				/* don't separate references for __call */
				arg = Z_REFVAL_P(arg);
			}
			if (Z_OPT_REFCOUNTED_P(arg)) {
				Z_ADDREF_P(arg);
			}
		}
		param = ZEND_CALL_ARG(call, i+1);
		ZVAL_COPY_VALUE(param, arg);
	}

	EG(scope) = calling_scope;
	Z_OBJ(call->This) = NULL;

	if (func->type == ZEND_USER_FUNCTION) {
		int call_via_handler = (func->common.fn_flags & ZEND_ACC_CALL_VIA_TRAMPOLINE) != 0;
		EG(scope) = func->common.scope;
		call->symbol_table = NULL;
		if (UNEXPECTED(func->op_array.fn_flags & ZEND_ACC_CLOSURE)) {
			ZEND_ASSERT(GC_TYPE(func->op_array.prototype) == IS_OBJECT);
			GC_REFCOUNT(func->op_array.prototype)++;
			ZEND_ADD_CALL_FLAG(call, ZEND_CALL_CLOSURE);
		}
		if (EXPECTED((func->op_array.fn_flags & ZEND_ACC_GENERATOR) == 0)) {
			zend_init_execute_data(call, &func->op_array, retval_ptr);
			zend_execute_ex(call);
		} else {
			zend_generator_create_zval(call, &func->op_array, retval_ptr);
		}
		if (call_via_handler) {
			/* We must re-initialize function again */
			*cache_entry = NULL;
		}
	} else if (func->type == ZEND_INTERNAL_FUNCTION) {
		int call_via_handler = (func->common.fn_flags & ZEND_ACC_CALL_VIA_TRAMPOLINE) != 0;
		if (func->common.scope) {
			EG(scope) = func->common.scope;
		}
		call->prev_execute_data = EG(current_execute_data);
		call->return_value = NULL; /* this is not a constructor call */
		EG(current_execute_data) = call;
		if (EXPECTED(zend_execute_internal == NULL)) {
			/* saves one function call if zend_execute_internal is not used */
			func->internal_function.handler(call, retval_ptr);
		} else {
			zend_execute_internal(call, retval_ptr);
		}
		EG(current_execute_data) = call->prev_execute_data;
		zend_vm_stack_free_args(call);

		/*  We shouldn't fix bad extensions here,
			because it can break proper ones (Bug #34045)
		if (!EX(function_state).function->common.return_reference)
		{
			INIT_PZVAL(f->retval);
		}*/
		if (EG(exception)) {
			zval_ptr_dtor(retval_ptr);
			ZVAL_UNDEF(retval_ptr);
		}

		if (call_via_handler) {
			/* We must re-initialize function again */
			*cache_entry = NULL;
		}
	} else { /* ZEND_OVERLOADED_FUNCTION */
		ZVAL_NULL(retval_ptr);

		zend_error(E_EXCEPTION | E_ERROR, "Cannot call overloaded function for non-object");
		zend_vm_stack_free_args(call);

		if (func->type == ZEND_OVERLOADED_FUNCTION_TEMPORARY) {
			zend_string_release(func->common.function_name);
		}
		efree(func);

		if (EG(exception)) {
			zval_ptr_dtor(retval_ptr);
			ZVAL_UNDEF(retval_ptr);
		}
	}

	EG(scope) = orig_scope;
	zend_vm_stack_free_call_frame(call);

	if (EG(current_execute_data) == &dummy_execute_data) {
		EG(current_execute_data) = dummy_execute_data.prev_execute_data;
	}

	if (EG(exception)) {
		zend_throw_exception_internal(NULL);
	}
	return SUCCESS;
}

static zend_bool zephir_is_info_dynamic_callable(zephir_fcall_info *info, zend_fcall_info_cache *fcc, zend_class_entry *ce_org, int strict_class)
{
	int call_via_handler = 0, retval = 0;
	zend_string *zs_lcname = zend_string_alloc(info->func_length, 0);
	zend_str_tolower_copy(zs_lcname->val, info->func_name, info->func_length);

	if (fcc->object && fcc->calling_scope == ce_org) {
		if (strict_class && ce_org->__call) {
			fcc->function_handler = zend_get_call_trampoline_func(ce_org, zs_lcname, 0);
			call_via_handler = 1;
			retval = 1;
		} else if (fcc->object->handlers->get_method) {
			fcc->function_handler = fcc->object->handlers->get_method(&fcc->object, zs_lcname, NULL);
			if (fcc->function_handler) {
				if (strict_class &&
					(!fcc->function_handler->common.scope ||
					 !instanceof_function(ce_org, fcc->function_handler->common.scope TSRMLS_CC))) {
					if ((fcc->function_handler->common.fn_flags & ZEND_ACC_CALL_VIA_HANDLER) != 0) {
						if (fcc->function_handler->type != ZEND_OVERLOADED_FUNCTION) {
							zend_string_release(fcc->function_handler->common.function_name);
						}
						zend_free_trampoline(fcc->function_handler);
					}
				} else {
					call_via_handler = (fcc->function_handler->common.fn_flags & ZEND_ACC_CALL_VIA_HANDLER) != 0;
					retval = 1;
				}
			}
		}
	} else if (fcc->calling_scope) {
		if (fcc->calling_scope->get_static_method) {
			fcc->function_handler = fcc->calling_scope->get_static_method(fcc->calling_scope, zs_lcname);
		} else {
			fcc->function_handler = zend_std_get_static_method(fcc->calling_scope, zs_lcname, NULL);
		}
		if (fcc->function_handler) {
			call_via_handler = (fcc->function_handler->common.fn_flags & ZEND_ACC_CALL_VIA_HANDLER) != 0;
			if (call_via_handler && !fcc->object && EG(current_execute_data) && Z_OBJ(EG(current_execute_data)->This) &&
			    instanceof_function(Z_OBJCE(EG(current_execute_data)->This), fcc->calling_scope)) {
				fcc->object = Z_OBJ(EG(current_execute_data)->This);
			}
			retval = 1;
		}
	}

	zend_string_free(zs_lcname);
	return retval;
}

static zend_bool zephir_is_info_callable_ex(zephir_fcall_info *info, zend_fcall_info_cache *fcc TSRMLS_DC)
{
	int retval = 0;
	zend_class_entry *ce_org = fcc->calling_scope;
	zend_fcall_info_cache fcc_local;

	if (fcc == NULL) {
		fcc = &fcc_local;
	}

	fcc->initialized = 0;
	fcc->calling_scope = NULL;
	fcc->called_scope = NULL;
	fcc->function_handler = NULL;
	fcc->object = NULL;

	switch (info->type) {

		case ZEPHIR_FCALL_TYPE_FUNC:

			if ((fcc->function_handler = zend_hash_str_find_ptr(EG(function_table), info->func_name, info->func_length)) != NULL) {
				if (fcc == &fcc_local &&
					fcc->function_handler &&
					((fcc->function_handler->type == ZEND_INTERNAL_FUNCTION &&
					  (fcc->function_handler->common.fn_flags & ZEND_ACC_CALL_VIA_HANDLER)) ||
					 fcc->function_handler->type == ZEND_OVERLOADED_FUNCTION_TEMPORARY ||
					 fcc->function_handler->type == ZEND_OVERLOADED_FUNCTION)) {
					if (fcc->function_handler->type != ZEND_OVERLOADED_FUNCTION) {
						efree((char*)fcc->function_handler->common.function_name);
					}
					zend_string_release(fcc->function_handler->common.function_name);
				}
				fcc->initialized = 1;
				return 1;
			}
			break;

		case ZEPHIR_FCALL_TYPE_ZVAL_METHOD:
			if (!EG(objects_store).object_buckets || !IS_OBJ_VALID(EG(objects_store).object_buckets[Z_OBJ_HANDLE_P(info->object_ptr)])) {
				return 0;
			}
			fcc->calling_scope = Z_OBJCE_P(info->object_ptr); /* TBFixed: what if it's overloaded? */
			fcc->object = Z_OBJ_P(info->object_ptr);

			if (!ce_org) {
				ce_org = fcc->calling_scope;
			}

			if ((fcc->function_handler = zend_hash_str_find_ptr(&info->ce->function_table, info->func_name, info->func_length + 1)) != NULL) {
				if (fcc == &fcc_local &&
					fcc->function_handler &&
					((fcc->function_handler->type == ZEND_INTERNAL_FUNCTION &&
					  (fcc->function_handler->common.fn_flags & ZEND_ACC_CALL_VIA_HANDLER)) ||
					 fcc->function_handler->type == ZEND_OVERLOADED_FUNCTION_TEMPORARY ||
					 fcc->function_handler->type == ZEND_OVERLOADED_FUNCTION)) {
					if (fcc->function_handler->type != ZEND_OVERLOADED_FUNCTION) {
						zend_string_release(fcc->function_handler->common.function_name);
					}
					zend_free_trampoline(fcc->function_handler);
				}
				retval = 1;
			}

			if (!retval) {
				retval = zephir_is_info_dynamic_callable(info, fcc, ce_org, 0 TSRMLS_CC);
			}
			break;

		case ZEPHIR_FCALL_TYPE_CLASS_SELF_METHOD:

			if (!EG(scope)) {
				return 0; // cannot access self:: when no class scope is active
			}

			fcc->called_scope = EG(current_execute_data)->called_scope;
			if (!fcc->object) {
				fcc->object = Z_OBJ(EG(current_execute_data)->This);
			}
			if ((fcc->function_handler = zend_hash_str_find_ptr(&fcc->calling_scope->function_table, info->func_name, info->func_length)) != NULL) {
				if (fcc == &fcc_local &&
					fcc->function_handler &&
					((fcc->function_handler->type == ZEND_INTERNAL_FUNCTION &&
					  (fcc->function_handler->common.fn_flags & ZEND_ACC_CALL_VIA_HANDLER)) ||
					 fcc->function_handler->type == ZEND_OVERLOADED_FUNCTION_TEMPORARY ||
					 fcc->function_handler->type == ZEND_OVERLOADED_FUNCTION)) {
					if (fcc->function_handler->type != ZEND_OVERLOADED_FUNCTION) {
						zend_string_release(fcc->function_handler->common.function_name);
					}
					zend_free_trampoline(fcc->function_handler);
				}
				retval = 1;
			}

			if (!retval) {
				retval = zephir_is_info_dynamic_callable(info, fcc, ce_org, 0 TSRMLS_CC);
			}
			break;

		case ZEPHIR_FCALL_TYPE_CLASS_PARENT_METHOD:

			if (!EG(scope)) {
				return 0; // cannot access parent:: when no class scope is active
			}

			if (!EG(scope)->parent) {
				return 0; // cannot access parent:: when current class scope has no parent
			}

			fcc->calling_scope = EG(scope)->parent;
			fcc->called_scope = EG(current_execute_data)->called_scope;
			if (!fcc->object) {
				fcc->object = Z_OBJ(EG(current_execute_data)->This);
			}

			if ((fcc->function_handler = zend_hash_str_find_ptr(&fcc->calling_scope->function_table, info->func_name, info->func_length)) != NULL) {
				if (fcc == &fcc_local &&
					fcc->function_handler &&
					((fcc->function_handler->type == ZEND_INTERNAL_FUNCTION &&
					  (fcc->function_handler->common.fn_flags & ZEND_ACC_CALL_VIA_HANDLER)) ||
					 fcc->function_handler->type == ZEND_OVERLOADED_FUNCTION_TEMPORARY ||
					 fcc->function_handler->type == ZEND_OVERLOADED_FUNCTION)) {
					if (fcc->function_handler->type != ZEND_OVERLOADED_FUNCTION) {
						zend_string_release(fcc->function_handler->common.function_name);
					}
					zend_free_trampoline(fcc->function_handler);
				}
				retval = 1;
			}

			if (!retval) {
				retval = zephir_is_info_dynamic_callable(info, fcc, ce_org, 1 TSRMLS_CC);
			}
			break;

		case ZEPHIR_FCALL_TYPE_CLASS_STATIC_METHOD:

			if (!EG(current_execute_data) || !EG(current_execute_data)->called_scope) {
				return 0; // cannot access static:: when no class scope is active
			}

			fcc->called_scope = EG(current_execute_data)->called_scope;
			if (!fcc->object) {
				fcc->object = Z_OBJ(EG(current_execute_data)->This);
			}
			fcc->calling_scope = fcc->called_scope;

			if ((fcc->function_handler = zend_hash_str_find_ptr(&fcc->calling_scope->function_table, info->func_name, info->func_length)) != NULL) {
				if (fcc == &fcc_local &&
					fcc->function_handler &&
					((fcc->function_handler->type == ZEND_INTERNAL_FUNCTION &&
					  (fcc->function_handler->common.fn_flags & ZEND_ACC_CALL_VIA_HANDLER)) ||
					 fcc->function_handler->type == ZEND_OVERLOADED_FUNCTION_TEMPORARY ||
					 fcc->function_handler->type == ZEND_OVERLOADED_FUNCTION)) {
					if (fcc->function_handler->type != ZEND_OVERLOADED_FUNCTION) {
						zend_string_release(fcc->function_handler->common.function_name);
					}
					zend_free_trampoline(fcc->function_handler);
				}
				retval = 1;
			}

			if (!retval) {
				retval = zephir_is_info_dynamic_callable(info, fcc, ce_org, 1 TSRMLS_CC);
			}
			break;

		case ZEPHIR_FCALL_TYPE_CE_METHOD:
			{
				zend_class_entry *scope = EG(current_execute_data) ? EG(current_execute_data)->func->common.scope : NULL;;

				fcc->calling_scope = info->ce;
				if (scope && !fcc->object && EG(current_execute_data) && Z_OBJ(EG(current_execute_data)->This) &&
		    		instanceof_function(Z_OBJCE(EG(current_execute_data)->This), scope) &&
				    instanceof_function(scope, fcc->calling_scope)) {
					fcc->object = Z_OBJ(EG(current_execute_data)->This);
					fcc->called_scope = Z_OBJCE(EG(current_execute_data)->This);
				} else {
					fcc->called_scope = fcc->object ? fcc->object->ce : fcc->calling_scope;
				}

				if ((fcc->function_handler = zend_hash_str_find_ptr(&fcc->calling_scope->function_table, info->func_name, info->func_length)) != NULL) {
					if (fcc == &fcc_local &&
						fcc->function_handler &&
						((fcc->function_handler->type == ZEND_INTERNAL_FUNCTION &&
						  (fcc->function_handler->common.fn_flags & ZEND_ACC_CALL_VIA_HANDLER)) ||
						 fcc->function_handler->type == ZEND_OVERLOADED_FUNCTION_TEMPORARY ||
						 fcc->function_handler->type == ZEND_OVERLOADED_FUNCTION)) {
						if (fcc->function_handler->type != ZEND_OVERLOADED_FUNCTION) {
							zend_string_release(fcc->function_handler->common.function_name);
						}
						zend_free_trampoline(fcc->function_handler);
					}
					retval = 1;
				}

				if (!retval) {
					retval = zephir_is_info_dynamic_callable(info, fcc, ce_org, 1 TSRMLS_CC);
				}
			}
			break;
	}

	if (fcc->object) {
		fcc->called_scope = fcc->object->ce;
	}
	if (retval) {
		fcc->initialized = 1;
	}

	return retval;
}

int zephir_call_function_opt(zend_fcall_info *fci, zend_fcall_info_cache *fci_cache, zephir_fcall_info *info, zval *params[])
{
	uint32_t i;
	zend_class_entry *calling_scope = NULL;
	zend_execute_data *call, dummy_execute_data;
	zend_fcall_info_cache fci_cache_local;
	zend_function *func;
	zend_class_entry *orig_scope;

	ZVAL_UNDEF(fci->retval);

	if (!EG(active)) {
		return FAILURE; /* executor is already inactive */
	}

	if (EG(exception)) {
		return FAILURE; /* we would result in an instable executor otherwise */
	}

	switch (fci->size) {
		case sizeof(zend_fcall_info):
			break; /* nothing to do currently */
		default:
			zend_error_noreturn(E_CORE_ERROR, "Corrupted fcall_info provided to zend_call_function()");
			break;
	}

	orig_scope = EG(scope);

	/* Initialize execute_data */
	if (!EG(current_execute_data)) {
		/* This only happens when we're called outside any execute()'s
		 * It shouldn't be strictly necessary to NULL execute_data out,
		 * but it may make bugs easier to spot
		 */
		memset(&dummy_execute_data, 0, sizeof(zend_execute_data));
		EG(current_execute_data) = &dummy_execute_data;
	} else if (EG(current_execute_data)->func &&
	           ZEND_USER_CODE(EG(current_execute_data)->func->common.type) &&
	           EG(current_execute_data)->opline->opcode != ZEND_DO_FCALL &&
	           EG(current_execute_data)->opline->opcode != ZEND_DO_ICALL &&
	           EG(current_execute_data)->opline->opcode != ZEND_DO_UCALL &&
	           EG(current_execute_data)->opline->opcode != ZEND_DO_FCALL_BY_NAME) {
		/* Insert fake frame in case of include or magic calls */
		dummy_execute_data = *EG(current_execute_data);
		dummy_execute_data.prev_execute_data = EG(current_execute_data);
		dummy_execute_data.call = NULL;
		dummy_execute_data.opline = NULL;
		dummy_execute_data.func = NULL;
		EG(current_execute_data) = &dummy_execute_data;
	}

	if (!fci_cache || !fci_cache->initialized) {
		char *error = NULL;

		if (!fci_cache) {
			fci_cache = &fci_cache_local;
		}

		if (!info) {
			/*char *callable_name;
			if (!zephir_is_callable_ex(&fci->function_name, fci->object, IS_CALLABLE_CHECK_SILENT, &callable_name, fci_cache, &error)) {
				if (error) {
					zend_error(E_WARNING, "Invalid callback %s, %s", callable_name, error);
					efree(error);
				}
				if (callable_name) {
					efree(callable_name);
				}
				if (EG(current_execute_data) == &dummy_execute_data) {
					EG(current_execute_data) = dummy_execute_data.prev_execute_data;
				}
				return FAILURE;
			} else {
				if (error) {
					zend_error(E_STRICT, "%s", error);
					efree(error);
				}
			}
			efree(callable_name);*/
			zend_error(E_ERROR, "Zephir: ZE3: Calling without info seems to be required but not implemented yet");
		} else {
			if (!zephir_is_info_callable_ex(info, fci_cache)) {
				return FAILURE;
			}
		}
	}

	func = fci_cache->function_handler;
	call = zend_vm_stack_push_call_frame(ZEND_CALL_TOP_FUNCTION,
		func, fci->param_count, fci_cache->called_scope, fci_cache->object);
	calling_scope = fci_cache->calling_scope;
	fci->object = fci_cache->object;
	if (fci->object &&
	    (!EG(objects_store).object_buckets ||
	     !IS_OBJ_VALID(EG(objects_store).object_buckets[fci->object->handle]))) {
		if (EG(current_execute_data) == &dummy_execute_data) {
			EG(current_execute_data) = dummy_execute_data.prev_execute_data;
		}
		return FAILURE;
	}

	if (func->common.fn_flags & (ZEND_ACC_ABSTRACT|ZEND_ACC_DEPRECATED)) {
		if (func->common.fn_flags & ZEND_ACC_ABSTRACT) {
			zend_error(E_EXCEPTION | E_ERROR, "Cannot call abstract method %s::%s()", func->common.scope->name->val, func->common.function_name->val);
			return FAILURE;
		}
		if (func->common.fn_flags & ZEND_ACC_DEPRECATED) {
 			zend_error(E_DEPRECATED, "Function %s%s%s() is deprecated",
				func->common.scope ? func->common.scope->name->val : "",
				func->common.scope ? "::" : "",
				func->common.function_name->val);
		}
	}

	for (i=0; i<fci->param_count; i++) {
		zval *param;
		zval *arg = params[i];

		if (ARG_SHOULD_BE_SENT_BY_REF(func, i + 1)) {
			if (UNEXPECTED(!Z_ISREF_P(arg))) {
				if (fci->no_separation &&
					!ARG_MAY_BE_SENT_BY_REF(func, i + 1)) {
					if (i) {
						/* hack to clean up the stack */
						ZEND_CALL_NUM_ARGS(call) = i;
						zend_vm_stack_free_args(call);
					}
					zend_vm_stack_free_call_frame(call);

					zend_error(E_WARNING, "Parameter %d to %s%s%s() expected to be a reference, value given",
						i+1,
						func->common.scope ? func->common.scope->name->val : "",
						func->common.scope ? "::" : "",
						func->common.function_name->val);
					if (EG(current_execute_data) == &dummy_execute_data) {
						EG(current_execute_data) = dummy_execute_data.prev_execute_data;
					}
					return FAILURE;
				}

				ZVAL_NEW_REF(arg, arg);
			}
			Z_ADDREF_P(arg);
		} else {
			if (Z_ISREF_P(arg) &&
			    !(func->common.fn_flags & ZEND_ACC_CALL_VIA_TRAMPOLINE)) {
				/* don't separate references for __call */
				arg = Z_REFVAL_P(arg);
			}
			if (Z_OPT_REFCOUNTED_P(arg)) {
				Z_ADDREF_P(arg);
			}
		}
		param = ZEND_CALL_ARG(call, i+1);
		ZVAL_COPY_VALUE(param, arg);
	}

	EG(scope) = calling_scope;
	if (func->common.fn_flags & ZEND_ACC_STATIC) {
		fci->object = NULL;
	}
	Z_OBJ(call->This) = fci->object;

	if (func->type == ZEND_USER_FUNCTION) {
		int call_via_handler = (func->common.fn_flags & ZEND_ACC_CALL_VIA_TRAMPOLINE) != 0;
		EG(scope) = func->common.scope;
		call->symbol_table = fci->symbol_table;
		if (UNEXPECTED(func->op_array.fn_flags & ZEND_ACC_CLOSURE)) {
			ZEND_ASSERT(GC_TYPE(func->op_array.prototype) == IS_OBJECT);
			GC_REFCOUNT(func->op_array.prototype)++;
			ZEND_ADD_CALL_FLAG(call, ZEND_CALL_CLOSURE);
		}
		if (EXPECTED((func->op_array.fn_flags & ZEND_ACC_GENERATOR) == 0)) {
			zend_init_execute_data(call, &func->op_array, fci->retval);
			zend_execute_ex(call);
		} else {
			zend_generator_create_zval(call, &func->op_array, fci->retval);
		}
		if (call_via_handler) {
			/* We must re-initialize function again */
			fci_cache->initialized = 0;
		}
	} else if (func->type == ZEND_INTERNAL_FUNCTION) {
		int call_via_handler = (func->common.fn_flags & ZEND_ACC_CALL_VIA_TRAMPOLINE) != 0;
		ZVAL_NULL(fci->retval);
		if (func->common.scope) {
			EG(scope) = func->common.scope;
		}
		call->prev_execute_data = EG(current_execute_data);
		call->return_value = NULL; /* this is not a constructor call */
		EG(current_execute_data) = call;
		if (EXPECTED(zend_execute_internal == NULL)) {
			/* saves one function call if zend_execute_internal is not used */
			func->internal_function.handler(call, fci->retval);
		} else {
			zend_execute_internal(call, fci->retval);
		}
		EG(current_execute_data) = call->prev_execute_data;
		zend_vm_stack_free_args(call);

		/*  We shouldn't fix bad extensions here,
			because it can break proper ones (Bug #34045)
		if (!EX(function_state).function->common.return_reference)
		{
			INIT_PZVAL(f->retval);
		}*/
		if (EG(exception)) {
			zval_ptr_dtor(fci->retval);
			ZVAL_UNDEF(fci->retval);
		}

		if (call_via_handler) {
			/* We must re-initialize function again */
			fci_cache->initialized = 0;
		}
	} else { /* ZEND_OVERLOADED_FUNCTION */
		ZVAL_NULL(fci->retval);

		/* Not sure what should be done here if it's a static method */
		if (fci->object) {
			call->prev_execute_data = EG(current_execute_data);
			EG(current_execute_data) = call;
			fci->object->handlers->call_method(func->common.function_name, fci->object, call, fci->retval);
			EG(current_execute_data) = call->prev_execute_data;
		} else {
			zend_error(E_EXCEPTION | E_ERROR, "Cannot call overloaded function for non-object");
		}

		zend_vm_stack_free_args(call);

		if (func->type == ZEND_OVERLOADED_FUNCTION_TEMPORARY) {
			zend_string_release(func->common.function_name);
		}
		efree(func);

		if (EG(exception)) {
			zval_ptr_dtor(fci->retval);
			ZVAL_UNDEF(fci->retval);
		}
	}

	EG(scope) = orig_scope;
	zend_vm_stack_free_call_frame(call);

	if (EG(current_execute_data) == &dummy_execute_data) {
		EG(current_execute_data) = dummy_execute_data.prev_execute_data;
	}

	if (EG(exception)) {
		zend_throw_exception_internal(NULL);
	}
	return SUCCESS;
}
