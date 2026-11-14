
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/operators.h"
#include "kernel/object.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/2670
 *
 * PHP 8.6 support: kernel paths whose engine API moved in 8.6, each
 * compared against plain PHP by the test.
 */
ZEPHIR_INIT_CLASS(Stub_Issue2670)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2670, stub, issue2670, stub_issue2670_method_entry, 0);

	return SUCCESS;
}

/**
 * Runs unique_key() `count` times inside the extension, so a string
 * leaked per call shows up in one memory_get_usage() window.
 */
PHP_METHOD(Stub_Issue2670, uniqueKeyLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long count, i;
	zval *prefix, prefix_sub, *value, value_sub, *count_param = NULL, key;

	ZVAL_UNDEF(&prefix_sub);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&key);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(prefix)
		Z_PARAM_ZVAL(value)
		Z_PARAM_LONG(count)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &prefix, &value, &count_param);
	ZEPHIR_INIT_VAR(&key);
	ZVAL_NULL(&key);
	i = 0;
	while (1) {
		if (!(i < count)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&key);
		zephir_unique_key(&key, prefix, value);
		i++;
	}
	RETURN_CCTOR(&key);
}

PHP_METHOD(Stub_Issue2670, emptyValue)
{
	zend_bool _0;
	zval *obj, obj_sub;

	ZVAL_UNDEF(&obj_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("value", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(obj)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &obj);
	_0 = zephir_isempty_property_fast(obj, _zephir_prop_0);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_Issue2670, issetValue)
{
	zend_bool _0;
	zval *obj, obj_sub;

	ZVAL_UNDEF(&obj_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("value", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(obj)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &obj);
	_0 = zephir_isset_property_value_fast(obj, _zephir_prop_0);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

