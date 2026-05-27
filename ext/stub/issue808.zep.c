
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
#include "kernel/object.h"
#include "kernel/array.h"


/**
 * Regression coverage for https://github.com/zephir-lang/zephir/issues/808
 *
 * unset(obj->{variable}) was not supported and threw:
 *   CompilerException: Cannot use expression type: property-dynamic-access in "unset"
 *
 * Fix: UnsetStatement must resolve the dynamic property name from the symbol
 * table and emit zephir_unset_property_zval() instead of rejecting the node.
 */
ZEPHIR_INIT_CLASS(Stub_Issue808)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue808, stub, issue808, stub_issue808_method_entry, 0);

	return SUCCESS;
}

/**
 * Unset a stdClass property by a variable holding the property name.
 * After the unset the property must no longer exist (isset returns false).
 */
PHP_METHOD(Stub_Issue808, testDynamicUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *propertyName, propertyName_sub, obj, _0;

	ZVAL_UNDEF(&propertyName_sub);
	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(propertyName)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &propertyName);
	ZEPHIR_INIT_VAR(&obj);
	object_init(&obj);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "value");
	zephir_update_property_zval_zval(&obj, propertyName, &_0);
	zephir_unset_property_zval(&obj, propertyName);
	RETURN_MM_BOOL(zephir_isset_property_value_zval(&obj, propertyName));
}

/**
 * Only the targeted property is removed; others survive.
 */
PHP_METHOD(Stub_Issue808, testDynamicUnsetSelective)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *toRemove, toRemove_sub, obj, _0;

	ZVAL_UNDEF(&toRemove_sub);
	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(toRemove)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &toRemove);
	ZEPHIR_INIT_VAR(&obj);
	object_init(&obj);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "keep_value");
	zephir_update_property_zval(&obj, ZEND_STRL("keep"), &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "remove_value");
	zephir_update_property_zval(&obj, ZEND_STRL("remove"), &_0);
	zephir_unset_property_zval(&obj, toRemove);
	RETURN_CCTOR(&obj);
}

/**
 * Unset using a string literal in braces: unset(obj->{"name"}).
 * This exercises property-string-access in UnsetStatement.
 * The post-unset check uses regular property-access (not brace syntax)
 * because isset(obj->{"name"}) has a separate known limitation.
 */
PHP_METHOD(Stub_Issue808, testDynamicUnsetStringKey)
{
	zval obj, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&obj);
	object_init(&obj);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "value");
	zephir_update_property_zval(&obj, ZEND_STRL("name"), &_0);
	zephir_unset_property(&obj, "name");
	static zend_string *_zephir_isset_0 = NULL;
	if (UNEXPECTED(!_zephir_isset_0)) {
		_zephir_isset_0 = zend_string_init("name", 4, 1);
	}
	RETURN_MM_BOOL(zephir_isset_property_value_fast(&obj, _zephir_isset_0));
}

/**
 * Unsetting a property that does not exist on the object must be a
 * silent no-op — no error, no warning — and must leave any other
 * properties intact. This mirrors PHP's own behaviour:
 *   unset($obj->nonExistent); // silent
 */
PHP_METHOD(Stub_Issue808, testDynamicUnsetNonExistentProperty)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *propertyName, propertyName_sub, obj, _0;

	ZVAL_UNDEF(&propertyName_sub);
	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(propertyName)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &propertyName);
	ZEPHIR_INIT_VAR(&obj);
	object_init(&obj);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "keep_value");
	zephir_update_property_zval(&obj, ZEND_STRL("keep"), &_0);
	zephir_unset_property_zval(&obj, propertyName);
	RETURN_CCTOR(&obj);
}

