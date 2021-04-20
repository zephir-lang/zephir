
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
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Stub_Properties_StaticProtectedProperties)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Properties, StaticProtectedProperties, stub, properties_staticprotectedproperties, stub_properties_staticprotectedproperties_method_entry, 0);

	/**
	 * This is a protected property with no initial value
	 */
	zend_declare_property_null(stub_properties_staticprotectedproperties_ce, SL("someNull"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	/**
	 * This is a protected property with initial null value
	 */
	zend_declare_property_null(stub_properties_staticprotectedproperties_ce, SL("someNullInitial"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	/**
	 * This is a protected property with initial boolean false
	 */
	zend_declare_property_bool(stub_properties_staticprotectedproperties_ce, SL("someFalse"), 0, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	/**
	 * This is a protected property with initial boolean true
	 */
	zend_declare_property_bool(stub_properties_staticprotectedproperties_ce, SL("someTrue"), 1, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	/**
	 * This is a protected property with an initial integer value
	 */
	zend_declare_property_long(stub_properties_staticprotectedproperties_ce, SL("someInteger"), 10, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	/**
	 * This is a protected property with an initial double value
	 */
	zend_declare_property_double(stub_properties_staticprotectedproperties_ce, SL("someDouble"), 10.25, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	/**
	 * This is a protected property with an initial string value
	 */
	zend_declare_property_string(stub_properties_staticprotectedproperties_ce, SL("someString"), "test", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(stub_properties_staticprotectedproperties_ce, SL("someVar"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Properties_StaticProtectedProperties, setSomeVar)
{
	zval *someVar, someVar_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&someVar_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(someVar)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(1, 0, &someVar);


	zephir_update_property_zval(this_ptr, ZEND_STRL("someVar"), someVar);
	RETURN_THISW();
}

PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeVar)
{
	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "someVar");
}

PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeNull)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);



	zephir_read_static_property_ce(&_0, stub_properties_staticprotectedproperties_ce, SL("someNull"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

PHP_METHOD(Stub_Properties_StaticProtectedProperties, setSomeNull)
{
	zval *param, param_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&param_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(param)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(1, 0, &param);


	zephir_update_static_property_ce(stub_properties_staticprotectedproperties_ce, ZEND_STRL("someNull"), param);
}

PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeNullInitial)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);



	zephir_read_static_property_ce(&_0, stub_properties_staticprotectedproperties_ce, SL("someNullInitial"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeFalse)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);



	zephir_read_static_property_ce(&_0, stub_properties_staticprotectedproperties_ce, SL("someFalse"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeTrue)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);



	zephir_read_static_property_ce(&_0, stub_properties_staticprotectedproperties_ce, SL("someTrue"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeInteger)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);



	zephir_read_static_property_ce(&_0, stub_properties_staticprotectedproperties_ce, SL("someInteger"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeDouble)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);



	zephir_read_static_property_ce(&_0, stub_properties_staticprotectedproperties_ce, SL("someDouble"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeString)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);



	zephir_read_static_property_ce(&_0, stub_properties_staticprotectedproperties_ce, SL("someString"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1536
 */
PHP_METHOD(Stub_Properties_StaticProtectedProperties, compareStaticNull)
{
	zval someNull, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&someNull);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(&_0);
	zephir_read_static_property_ce(&_0, stub_properties_staticprotectedproperties_ce, SL("someNull"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(&someNull, &_0);
	if (Z_TYPE_P(&someNull) == IS_NULL) {
		ZEPHIR_INIT_NVAR(&someNull);
		ZVAL_BOOL(&someNull, 1);
		RETURN_CCTOR(&someNull);
	}
	RETURN_CCTOR(&someNull);
}

