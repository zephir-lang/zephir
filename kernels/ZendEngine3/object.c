
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

#include <Zend/zend_closures.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/hash.h"
#include "kernel/array.h"
#include "kernel/operators.h"

zend_class_entry *zephir_fetch_class_str_ex(char *class_name, size_t length, int fetch_type)
{
	zend_class_entry *retval;
	zend_string *str = zend_string_init(class_name, length, 0);
	retval = zend_fetch_class(str, fetch_type);
	zend_string_release(str);
	return retval;
}

static inline zend_class_entry *zephir_lookup_class_ce(zend_class_entry *ce, const char *property_name, unsigned int property_length)
{
	zend_class_entry *original_ce = ce;

	while (ce) {
		if (zend_hash_str_exists(&ce->properties_info, property_name, property_length)) {
			return ce;
		}
		ce = ce->parent;
	}
	return original_ce;
}


/**
 * Reads a property from an object
 */
int zephir_read_property(zval *result, zval *object, const char *property_name, zend_uint property_length, int flags)
{
	zval property;
	zend_class_entry *ce, *old_scope;
	zval tmp;
	zval *res;

	ZVAL_UNDEF(&tmp);

	if (Z_TYPE_P(object) != IS_OBJECT) {

		if ((flags & PH_NOISY) == PH_NOISY) {
			php_error_docref(NULL, E_NOTICE, "Trying to get property \"%s\" of non-object", property_name);
		}

		zval_ptr_dtor(result);
		ZVAL_NULL(result);
		return FAILURE;
	}

	ce = Z_OBJCE_P(object);
	if (ce->parent) {
		ce = zephir_lookup_class_ce(ce, property_name, property_length);
	}

	old_scope = EG(scope);
	EG(scope) = ce;

	if (!Z_OBJ_HT_P(object)->read_property) {
		const char *class_name;

		class_name = Z_OBJ_P(object) ? ZSTR_VAL(Z_OBJCE_P(object)->name) : "";
		zend_error(E_CORE_ERROR, "Property %s of class %s cannot be read", property_name, class_name);
	}

	ZVAL_STRINGL(&property, property_name, property_length);

	res = Z_OBJ_HT_P(object)->read_property(object, &property, flags ? BP_VAR_IS : BP_VAR_R, NULL, &tmp);
	if ((flags & PH_READONLY) == PH_READONLY) {
		ZVAL_COPY_VALUE(result, res);
	} else {
		ZVAL_COPY(result, res);
	}

	zval_ptr_dtor(&property);

	EG(scope) = old_scope;
	return SUCCESS;
}

/**
 * Checks whether obj is an object and updates property with another zval
 */
int zephir_update_property_zval(zval *object, const char *property_name, unsigned int property_length, zval *value)
{
	zend_class_entry *ce, *old_scope;
	zval property;

	old_scope = EG(scope);
	if (Z_TYPE_P(object) != IS_OBJECT) {
		php_error_docref(NULL, E_WARNING, "Attempt to assign property of non-object");
		return FAILURE;
	}

	ce = Z_OBJCE_P(object);
	if (ce->parent) {
		ce = zephir_lookup_class_ce(ce, property_name, property_length);
	}

	EG(scope) = ce;

	if (!Z_OBJ_HT_P(object)->write_property) {
		const char *class_name;

		class_name = Z_OBJ_P(object) ? ZSTR_VAL(Z_OBJCE_P(object)->name) : "";
		zend_error(E_CORE_ERROR, "Property %s of class %s cannot be updated", property_name, class_name);
	}

	ZVAL_STRINGL(&property, property_name, property_length);

	Z_OBJ_HT_P(object)->write_property(object, &property, value, 0);
	zval_ptr_dtor(&property);

	EG(scope) = old_scope;
	return SUCCESS;
}


/**
 * Updates an array property
 */
int zephir_update_property_array(zval *object, const char *property, zend_uint property_length, const zval *index, zval *value)
{
	zval tmp;
	int separated = 0;

	if (Z_TYPE_P(object) == IS_OBJECT) {
		zephir_read_property(&tmp, object, property, property_length, PH_NOISY | PH_READONLY);

		/** Separation only when refcount > 1 */
		if (Z_REFCOUNTED(tmp) && Z_REFCOUNT(tmp) > 1) {
			if (!Z_ISREF(tmp)) {
				zval new_zv;
				ZVAL_DUP(&new_zv, &tmp);
				ZVAL_COPY_VALUE(&tmp, &new_zv);
				separated = 1;
			}
		}

		/** Convert the value to array if not is an array */
		if (Z_TYPE(tmp) != IS_ARRAY) {
			if (separated) {
				convert_to_array(&tmp);
			} else {
				array_init(&tmp);
				separated = 1;
			}
		}

		Z_TRY_ADDREF_P(value);

		if (Z_TYPE_P(index) == IS_STRING) {
			zend_symtable_str_update(Z_ARRVAL(tmp), Z_STRVAL_P(index), Z_STRLEN_P(index), value);
		} else if (Z_TYPE_P(index) == IS_LONG) {
			zend_hash_index_update(Z_ARRVAL(tmp), Z_LVAL_P(index), value);
		} else if (Z_TYPE_P(index) == IS_NULL) {
			zend_hash_next_index_insert(Z_ARRVAL(tmp), value);
		}

		if (separated) {
			zephir_update_property_zval(object, property, property_length, &tmp);
		}
	}

	return SUCCESS;
}

/**
 * Multiple array-offset update
 */
int zephir_update_property_array_multi(zval *object, const char *property, zend_uint property_length, zval *value, const char *types, int types_length, int types_count, ...)
{
	va_list ap;
	zval tmp_arr;
	int separated = 0;

	if (Z_TYPE_P(object) == IS_OBJECT) {
		zephir_read_property(&tmp_arr, object, property, property_length, PH_NOISY | PH_READONLY);

		/** Separation only when refcount > 1 */
		if (Z_REFCOUNTED(tmp_arr) && Z_REFCOUNT(tmp_arr) > 1) {
			Z_DELREF(tmp_arr);
			if (!Z_ISREF(tmp_arr)) {
				zval new_zv;
				ZVAL_DUP(&new_zv, &tmp_arr);
				ZVAL_COPY_VALUE(&tmp_arr, &new_zv);
				separated = 1;
			}
		}

		/** Convert the value to array if not is an array */
		if (Z_TYPE(tmp_arr) != IS_ARRAY) {
			if (separated) {
				convert_to_array(&tmp_arr);
			} else {
				array_init(&tmp_arr);
				separated = 1;
			}
		}

		va_start(ap, types_count);
		zephir_array_update_multi_ex(&tmp_arr, value, types, types_length, types_count, ap);
		va_end(ap);

		if (separated) {
			zephir_update_property_zval(object, property, property_length, &tmp_arr);
		}
	}

	return SUCCESS;
}

int zephir_read_static_property_ce(zval *result, zend_class_entry *ce, const char *property, int len, int flags)
{
	zval *tmp = zend_read_static_property(ce, property, len, (zend_bool) ZEND_FETCH_CLASS_SILENT);

	zval_ptr_dtor(result);
	ZVAL_NULL(result);
	if (tmp)
	{
		if ((flags & PH_READONLY) == PH_READONLY) {
			ZVAL_COPY_VALUE(result, tmp);
		} else {
			ZVAL_COPY(result, tmp);
		}
		return SUCCESS;
	}
	return FAILURE;
}

static zval *zephir_std_get_static_property(zend_class_entry *ce, const char *property_name, int property_name_len, zend_bool silent, zend_property_info **param_property_info)
{
	zend_property_info *property_info;
	zval *ret;

	if (param_property_info == NULL)
	{
		property_info = zend_hash_str_find_ptr(&ce->properties_info, property_name, property_name_len);
		if (UNEXPECTED(property_info == NULL)) {
			goto undeclared_property;
		}

		/*if (UNEXPECTED(!zend_verify_property_access(property_info, ce))) {
			if (!silent) {
				zend_throw_error(NULL, "Cannot access %s property %s::$%s", zend_visibility_string(property_info->flags), ZSTR_VAL(ce->name), property_name);
			}
			return NULL;
		}*/

		if (UNEXPECTED((property_info->flags & ZEND_ACC_STATIC) == 0)) {
			goto undeclared_property;
		}

		if (UNEXPECTED(!(ce->ce_flags & ZEND_ACC_CONSTANTS_UPDATED))) {
			if (UNEXPECTED(zend_update_class_constants(ce)) != SUCCESS) {
				return NULL;
			}
		}
		ret = CE_STATIC_MEMBERS(ce) + property_info->offset;

		if (param_property_info) {
			*param_property_info = property_info;
		}
	} else {
		property_info = *param_property_info;
	}

	/* check if static properties were destoyed */
	if (UNEXPECTED(CE_STATIC_MEMBERS(ce) == NULL)) {
undeclared_property:
		if (!silent) {
			zend_throw_error(NULL, "Access to undeclared static property: %s::$%s", ZSTR_VAL(ce->name), property_name);
		}
		ret = NULL;
	}

	return ret;
}

static int zephir_update_static_property_ex(zend_class_entry *scope, const char *name, int name_length, zval *value, zend_property_info **property_info)
{
	zval *property;
	zend_zephir_globals_def *zephir_globals_ptr = ZEPHIR_VGLOBAL;
	zend_class_entry *old_scope = EG(scope);

	EG(scope) = scope;
	property = zephir_std_get_static_property(scope, name, name_length, 0, property_info);
	EG(scope) = old_scope;

	if (!property) {
		return FAILURE;
	} else {
		if (Z_REFCOUNTED_P(value) && Z_ISREF_P(value)) {
			SEPARATE_ZVAL(value);
		}
		ZVAL_COPY(property, value);
		return SUCCESS;
	}
}

int zephir_update_static_property_ce(zend_class_entry *ce, const char *name, int len, zval *value)
{
	assert(ce != NULL);
	return zephir_update_static_property_ex(ce, name, len, value, NULL);
}

/*
 * Multiple array-offset update
 */
int zephir_update_static_property_array_multi_ce(zend_class_entry *ce, const char *property, zend_uint property_length, zval *value, const char *types, int types_length, int types_count, ...)
{
	va_list ap;
	zval tmp_arr;
	int separated = 0;

	ZVAL_UNDEF(&tmp_arr);

	zephir_read_static_property_ce(&tmp_arr, ce, property, property_length, PH_NOISY | PH_READONLY);
	if (Z_TYPE(tmp_arr) <= IS_NULL) {
		array_init(&tmp_arr);
		separated = 1;
	}

	/** Separation only when refcount > 1 */
	if (Z_REFCOUNTED(tmp_arr) && Z_REFCOUNT(tmp_arr) > 1) {
		if (!Z_ISREF(tmp_arr)) {
			zval new_zv;
			ZVAL_DUP(&new_zv, &tmp_arr);
			ZVAL_COPY_VALUE(&tmp_arr, &new_zv);
			separated = 1;
		}
	}

	/** Convert the value to array if not is an array */
	if (Z_TYPE(tmp_arr) != IS_ARRAY) {
		if (separated) {
			convert_to_array(&tmp_arr);
		} else {
			array_init(&tmp_arr);
			separated = 1;
		}
	}

	va_start(ap, types_count);
	zephir_array_update_multi_ex(&tmp_arr, value, types, types_length, types_count, ap);
	va_end(ap);

	if (separated) {
		zephir_update_static_property_ce(ce, property, property_length, &tmp_arr);
	}

	return SUCCESS;
}
