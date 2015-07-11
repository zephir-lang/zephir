
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

#define ZEPHIR_REGISTER_CLASS_EX(ns, class_name, lower_ns, lcname, parent_ce, methods, flags) \
	{ \
		zend_class_entry ce; \
		if (!parent_ce) { \
			fprintf(stderr, "Can't register class %s::%s with null parent\n", #ns, #class_name); \
			return FAILURE; \
		} \
		memset(&ce, 0, sizeof(zend_class_entry)); \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, methods); \
		lower_ns## _ ##lcname## _ce = zend_register_internal_class_ex(&ce, parent_ce); \
		if (!lower_ns## _ ##lcname## _ce) { \
			fprintf(stderr, "Zephir Error: Class to extend '%s' was not found when registering class '%s'\n", (parent_ce ? parent_ce->name : "(null)"), ZEND_NS_NAME(#ns, #class_name)); \
			return FAILURE; \
		} \
		lower_ns## _ ##lcname## _ce->ce_flags |= flags;  \
	}

/** Return zval with always ctor */
#define RETURN_CTOR(var) { \
		RETVAL_ZVAL(&var, 1, 0); \
	} \
	ZEPHIR_MM_RESTORE(); \
	return;

/** Return zval with always ctor, without restoring the memory stack */
#define RETURN_CTORW(var) { \
		RETVAL_ZVAL(&var, 1, 0); \
	} \
	return;

/** Return zval checking if it's needed to ctor */
#define RETURN_CCTOR(var) { \
	ZVAL_DUP(return_value, &var); \
	ZEPHIR_MM_RESTORE(); \
	return; \
}

/** Return zval checking if it's needed to ctor, without restoring the memory stack  */
#define RETURN_CCTORW(var) { \
		ZVAL_DUP(return_value, &var); \
		return; \
}

#define RETURN_LCTORW(var) RETURN_CCTORW(var);

/**
 * Returns a zval in an object member
 */
#define RETURN_MEMBER(object, member_name) \
	zephir_return_property(return_value, object, SL(member_name)); \
	return;

/** Return without change return_value */
#define RETURN_MM()                 { ZEPHIR_MM_RESTORE(); return; }

/**
 * Returns a zval in an object member
 */
#define RETURN_MM_MEMBER(object, member_name) \
  zephir_return_property(return_value, object, SL(member_name)); \
  RETURN_MM();

#define RETURN_ON_FAILURE(what) \
	do { \
		if (what == FAILURE) { \
			return; \
		} \
	} while (0)

#define RETURN_MM_ON_FAILURE(what) \
	do { \
		if (what == FAILURE) { \
			ZEPHIR_MM_RESTORE(); \
			return; \
		} \
	} while (0)

/** Return null restoring memory frame */
#define RETURN_MM_NULL()            { RETVAL_NULL(); ZEPHIR_MM_RESTORE(); return; }

/* Globals functions */
int zephir_get_global(zval *arr, const char *global, unsigned int global_length);

/* Count */
void zephir_fast_count(zval *result, zval *array);
int zephir_fast_count_ev(zval *array);
int zephir_fast_count_int(zval *value);

/* Utils functions */
static inline int zephir_maybe_separate_zval(zval* z)
{
	if (!Z_REFCOUNTED_P(z) || (Z_REFCOUNT_P(z) > 1 && !Z_ISREF_P(z))) {
		if (!Z_IMMUTABLE_P(z)) {
			Z_DELREF_P(z);
		}
		zval_copy_ctor_func(z);
		return 1;
	}

	return 0;
}

#define ZEPHIR_SET_SYMBOL(symbol_table, name, value) { \
	Z_TRY_ADDREF_P(value); \
	zend_hash_str_update(symbol_table, name, sizeof(name) - 1, value); \
}

int zephir_is_iterable_ex(zval *arr, int duplicate);

/** Check if an array is iterable or not */
#define zephir_is_iterable(var, duplicate, file, line) \
	if (!var || !zephir_is_iterable_ex(var, duplicate)) { \
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(zend_exception_get_default(), "The argument is not initialized or iterable()", file, line); \
		ZEPHIR_MM_RESTORE(); \
		return; \
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

#define ZEPHIR_GET_CONSTANT(return_value, const_name) do { \
	zval *_constant_ptr = zend_get_constant_str(SL(const_name)); \
	if (_constant_ptr == NULL) { \
		ZEPHIR_MM_RESTORE(); \
		return; \
	} \
	ZVAL_COPY(&return_value, _constant_ptr); \
} while(0)

#ifndef ZEPHIR_RELEASE
#define ZEPHIR_DEBUG_PARAMS , const char *file, int line
#define ZEPHIR_DEBUG_PARAMS_DUMMY , "", 0
#else
#define ZEPHIR_DEBUG_PARAMS , const char *file, int line
#define ZEPHIR_DEBUG_PARAMS_DUMMY , "", 0
#endif

#endif /* ZEPHIR_KERNEL_MAIN_H */
