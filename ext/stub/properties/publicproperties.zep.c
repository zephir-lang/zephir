
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


ZEPHIR_INIT_CLASS(Stub_Properties_PublicProperties)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Properties, PublicProperties, stub, properties_publicproperties, stub_properties_publicproperties_method_entry, 0);

	/**
	 * This is a public property with no initial value
	 */
	zend_declare_property_null(stub_properties_publicproperties_ce, SL("someNull"), ZEND_ACC_PUBLIC);
	/**
	 * This is a public property with initial null value
	 */
	zend_declare_property_null(stub_properties_publicproperties_ce, SL("someNullInitial"), ZEND_ACC_PUBLIC);
	/**
	 * This is a public property with initial boolean false
	 */
	zend_declare_property_bool(stub_properties_publicproperties_ce, SL("someFalse"), 0, ZEND_ACC_PUBLIC);
	/**
	 * This is a public property with initial boolean true
	 */
	zend_declare_property_bool(stub_properties_publicproperties_ce, SL("someTrue"), 1, ZEND_ACC_PUBLIC);
	/**
	 * This is a public property with an initial integer value
	 */
	zend_declare_property_long(stub_properties_publicproperties_ce, SL("someInteger"), 10, ZEND_ACC_PUBLIC);
	/**
	 * This is a public property with an initial double value
	 */
	zend_declare_property_double(stub_properties_publicproperties_ce, SL("someDouble"), 10.25, ZEND_ACC_PUBLIC);
	/**
	 * This is a public property with an initial string value
	 */
	zend_declare_property_string(stub_properties_publicproperties_ce, SL("someString"), "test", ZEND_ACC_PUBLIC);
	/**
	 * This is a public property with an initial empty-array value
	 */
	{
		zval _zc0;
		array_init_size(&_zc0, 1);
		zephir_declare_property_array(stub_properties_publicproperties_ce, SL("someEmptyArray"), &_zc0, ZEND_ACC_PUBLIC);
	}

	/**
	 * This is a public property with an initial array value
	 */
	{
		zval _zc0;
		array_init_size(&_zc0, 5);
		add_next_index_long(&_zc0, 1);
		add_next_index_long(&_zc0, 2);
		add_next_index_long(&_zc0, 3);
		add_next_index_long(&_zc0, 4);
		zephir_declare_property_array(stub_properties_publicproperties_ce, SL("someArray"), &_zc0, ZEND_ACC_PUBLIC);
	}

	/**
	 * It's needed to test ide stubs
	 * @link https://github.com/zephir-lang/zephir/issues/731
	 */
	{
		zval _zc0;
		array_init_size(&_zc0, 4);
		add_assoc_stringl_ex(&_zc0, SL("a"), SL("A"));
		add_assoc_stringl_ex(&_zc0, SL("b"), SL("B"));
		add_assoc_stringl_ex(&_zc0, SL("c"), SL("C"));
		zephir_declare_property_array(stub_properties_publicproperties_ce, SL("someArrayWithSpecifyKeys"), &_zc0, ZEND_ACC_PROTECTED);
	}

	/**
	 * It's needed to test ide stubs
	 * @link https://github.com/zephir-lang/zephir/issues/731
	 */
	{
		zval _zc0;
		array_init_size(&_zc0, 5);
		zval _zc1;
		array_init_size(&_zc1, 2);
		add_assoc_stringl_ex(&_zc1, SL("class"), SL("alert alert-success"));
		add_assoc_zval_ex(&_zc0, SL("success"), &_zc1);
		zval _zc2;
		array_init_size(&_zc2, 2);
		add_assoc_stringl_ex(&_zc2, SL("class"), SL("alert alert-info"));
		add_assoc_zval_ex(&_zc0, SL("info"), &_zc2);
		zval _zc3;
		array_init_size(&_zc3, 2);
		add_assoc_stringl_ex(&_zc3, SL("class"), SL("alert alert-warning"));
		add_assoc_zval_ex(&_zc0, SL("warning"), &_zc3);
		zval _zc4;
		array_init_size(&_zc4, 2);
		add_assoc_stringl_ex(&_zc4, SL("class"), SL("alert alert-danger"));
		add_assoc_zval_ex(&_zc0, SL("danger"), &_zc4);
		zephir_declare_property_array(stub_properties_publicproperties_ce, SL("someArrayWithSubArray"), &_zc0, ZEND_ACC_PROTECTED);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 1);
		zephir_declare_property_array(stub_properties_publicproperties_ce, SL("someGetterSetterArray"), &_zc0, ZEND_ACC_PROTECTED);
	}

	zend_declare_property_long(stub_properties_publicproperties_ce, SL("testPropertyFromSelfConstantValue"), 123, ZEND_ACC_PUBLIC);
	zend_declare_property_long(stub_properties_publicproperties_ce, SL("testPropertyFromClassNameConstantValue"), 123, ZEND_ACC_PUBLIC);
	zephir_declare_class_constant_long(stub_properties_publicproperties_ce, SL("TEST_CONST"), 123);

	return SUCCESS;
}

PHP_METHOD(Stub_Properties_PublicProperties, setSomeGetterSetterArray)
{
	zval *someGetterSetterArray, someGetterSetterArray_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&someGetterSetterArray_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("someGetterSetterArray", 21, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(someGetterSetterArray)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &someGetterSetterArray);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 6, someGetterSetterArray);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_THISW();
}

PHP_METHOD(Stub_Properties_PublicProperties, getSomeGetterSetterArray)
{

	RETURN_MEMBER(getThis(), "someGetterSetterArray");
}

PHP_METHOD(Stub_Properties_PublicProperties, test394Issue)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("someTrue", 8, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 7, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_0);
}

