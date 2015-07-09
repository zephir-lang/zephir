
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

#ifndef ZEPHIR_KERNEL_MAIN_H
#define ZEPHIR_KERNEL_MAIN_H

#include <Zend/zend_interfaces.h>
#include <ext/spl/spl_exceptions.h>
#include <ext/spl/spl_iterators.h>

/** Main macros */
#define PH_DEBUG 0

#define PH_NOISY 256
#define PH_SILENT 1024
#define PH_READONLY 4096

#define PH_NOISY_CC PH_NOISY
#define PH_SILENT_CC PH_SILENT

#define PH_SEPARATE 256
#define PH_COPY 1024
#define PH_CTOR 4096

#ifndef zend_uint
 #define zend_uint uint
#endif

#define SL(str) ZEND_STRL(str)
#define SS(str) ZEND_STRS(str)
#define ISL(str) (zephir_interned_##str), (sizeof(#str)-1)
#define ISS(str) (zephir_interned_##str), (sizeof(#str))

#include <Zend/zend_constants.h>
#include "kernel/exception.h"

/** class/interface registering */
#define ZEPHIR_REGISTER_CLASS(ns, class_name, lower_ns, name, methods, flags) \
	{ \
		zend_class_entry ce; \
		memset(&ce, 0, sizeof(zend_class_entry)); \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, methods); \
		lower_ns## _ ##name## _ce = zend_register_internal_class(&ce); \
		lower_ns## _ ##name## _ce->ce_flags |= flags;  \
	}

/** Return zval checking if it's needed to ctor */
#define RETURN_CCTOR(var) { \
	ZVAL_DUP(return_value, &var); \
	ZEPHIR_MM_RESTORE(); \
	return; \
}

/** Return null restoring memory frame */
#define RETURN_MM_NULL()            { RETVAL_NULL(); ZEPHIR_MM_RESTORE(); return; }

/* Globals functions */
int zephir_get_global(zval *arr, const char *global, unsigned int global_length);

/* Utils functions */
static inline int zephir_maybe_separate_zval(zval* z)
{
	if (Z_REFCOUNTED_P(z) && Z_REFCOUNT_P(z) > 1 && !Z_ISREF_P(z)) {
		if (!Z_IMMUTABLE_P(z)) {
			Z_DELREF_P(z);
		}
		zval_copy_ctor_func(z);
		return 1;
	}

	return 0;
}

#define ZEPHIR_SET_SYMBOL(symbol_table, name, value) { \
	zend_hash_str_update(symbol_table, name, sizeof(name) - 1, value); \
}

/* Fetch Parameters */
int zephir_fetch_parameters(int num_args, int required_args, int optional_args, ...);

/** Low overhead parse/fetch parameters */
#define zephir_fetch_params(memory_grow, required_params, optional_params, ...) \
	if (zephir_fetch_parameters(ZEND_NUM_ARGS(), required_params, optional_params, __VA_ARGS__) == FAILURE) { \
		if (memory_grow) { \
			RETURN_MM_NULL(); \
		} else { \
			RETURN_NULL(); \
		} \
	}

#define ZEPHIR_CREATE_OBJECT(obj, class_type) \
	{ \
		zend_object *object = zend_objects_new(class_type); \
		ZVAL_UNDEF(obj); \
		ZVAL_OBJ(obj, object); \
		object_properties_init(object, class_type); \
	}

#ifndef ZEPHIR_RELEASE
#define ZEPHIR_DEBUG_PARAMS , const char *file, int line
#define ZEPHIR_DEBUG_PARAMS_DUMMY , "", 0
#else
#define ZEPHIR_DEBUG_PARAMS , const char *file, int line
#define ZEPHIR_DEBUG_PARAMS_DUMMY , "", 0
#endif

#endif /* ZEPHIR_KERNEL_MAIN_H */
