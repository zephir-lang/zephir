
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"


/**
 * Readonly class properties (issue #2614). A readonly property must be typed,
 * has no default value and is initialized exactly once (here, in the
 * constructor). The engine enforces write-once on PHP 8.1+.
 */
ZEPHIR_INIT_CLASS(Stub_Properties_ReadonlyProperties)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Properties, ReadonlyProperties, stub, properties_readonlyproperties, stub_properties_readonlyproperties_method_entry, 0);

	{
		zval _zc0;
		ZVAL_UNDEF(&_zc0);
		zephir_declare_typed_property(stub_properties_readonlyproperties_ce, SL("id"), &_zc0, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, MAY_BE_LONG, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_UNDEF(&_zc0);
		zephir_declare_typed_property(stub_properties_readonlyproperties_ce, SL("label"), &_zc0, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, MAY_BE_STRING, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_UNDEF(&_zc0);
		zephir_declare_typed_property(stub_properties_readonlyproperties_ce, SL("tags"), &_zc0, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, MAY_BE_ARRAY, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_UNDEF(&_zc0);
		zephir_declare_typed_property(stub_properties_readonlyproperties_ce, SL("flag"), &_zc0, ZEND_ACC_PROTECTED|ZEND_ACC_READONLY, MAY_BE_BOOL, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_UNDEF(&_zc0);
		zephir_declare_typed_property(stub_properties_readonlyproperties_ce, SL("friend"), &_zc0, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, 0, SL("Stub\\Properties\\PublicProperties"));
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Properties_ReadonlyProperties, __construct)
{
	zval _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval __$true, __$false, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	static zend_string *_zephir_prop_2 = NULL;
	static zend_string *_zephir_prop_3 = NULL;
	static zend_string *_zephir_prop_4 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("id", 2, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("label", 5, 1);
	}
	if (UNEXPECTED(!_zephir_prop_2)) {
		_zephir_prop_2 = zend_string_init("tags", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_3)) {
		_zephir_prop_3 = zend_string_init("flag", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_4)) {
		_zephir_prop_4 = zend_string_init("friend", 6, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 42);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 84, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_1, 85, &_1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "a");
	zephir_array_fast_append(&_2, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "b");
	zephir_array_fast_append(&_2, &_1);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_2, 86, &_2);
	if (1) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_3, 87, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_3, 87, &__$false);
	}
	ZEPHIR_INIT_NVAR(&_1);
	object_init_ex(&_1, stub_properties_publicproperties_ce);
	if (zephir_has_constructor(&_1)) {
		ZEPHIR_CALL_METHOD(NULL, &_1, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	zephir_update_property_zval_cached(this_ptr, _zephir_prop_4, 88, &_1);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Properties_ReadonlyProperties, getFlag)
{

	RETURN_MEMBER(getThis(), "flag");
}

