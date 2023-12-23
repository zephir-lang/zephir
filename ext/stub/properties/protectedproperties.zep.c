
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


/**
 * Stub\Properties\ProtectedProperties
 */
ZEPHIR_INIT_CLASS(Stub_Properties_ProtectedProperties)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Properties, ProtectedProperties, stub, properties_protectedproperties, stub_properties_protectedproperties_method_entry, 0);

	/**
	 * This is a protected property with no initial value
	 */
	zend_declare_property_null(stub_properties_protectedproperties_ce, SL("someNull"), ZEND_ACC_PROTECTED);
	/**
	 * This is a protected property with initial null value
	 */
	zend_declare_property_null(stub_properties_protectedproperties_ce, SL("someNullInitial"), ZEND_ACC_PROTECTED);
	/**
	 * This is a protected property with initial boolean false
	 */
	zend_declare_property_bool(stub_properties_protectedproperties_ce, SL("someFalse"), 0, ZEND_ACC_PROTECTED);
	/**
	 * This is a protected property with initial boolean true
	 */
	zend_declare_property_bool(stub_properties_protectedproperties_ce, SL("someTrue"), 1, ZEND_ACC_PROTECTED);
	/**
	 * This is a protected property with an initial integer value
	 */
	zend_declare_property_long(stub_properties_protectedproperties_ce, SL("someInteger"), 10, ZEND_ACC_PROTECTED);
	/**
	 * This is a protected property with an initial double value
	 */
	zend_declare_property_double(stub_properties_protectedproperties_ce, SL("someDouble"), 10.25, ZEND_ACC_PROTECTED);
	/**
	 * This is a protected property with an initial string value
	 */
	zend_declare_property_string(stub_properties_protectedproperties_ce, SL("someString"), "test", ZEND_ACC_PROTECTED);
	/**
	 * @var null|mixed
	 */
	zend_declare_property_null(stub_properties_protectedproperties_ce, SL("someVar"), ZEND_ACC_PROTECTED);
	/**
	 * This is a property to test default value on extends
	 * @var array
	 */
	zend_declare_property_null(stub_properties_protectedproperties_ce, SL("someArrayVar"), ZEND_ACC_PROTECTED);
	stub_properties_protectedproperties_ce->create_object = zephir_init_properties_Stub_Properties_ProtectedProperties;

	return SUCCESS;
}

/**
 */
PHP_METHOD(Stub_Properties_ProtectedProperties, setSomeVar)
{
	zval *someVar, someVar_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&someVar_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(someVar)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &someVar);
	zephir_update_property_zval(this_ptr, ZEND_STRL("someVar"), someVar);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeVar)
{

	RETURN_MEMBER(getThis(), "someVar");
}

/**
 * This is a property to test default value on extends
 */
PHP_METHOD(Stub_Properties_ProtectedProperties, setSomeArrayVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *someArrayVar_param = NULL;
	zval someArrayVar;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&someArrayVar);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(someArrayVar)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &someArrayVar_param);
	zephir_get_arrval(&someArrayVar, someArrayVar_param);
	zephir_update_property_zval(this_ptr, ZEND_STRL("someArrayVar"), &someArrayVar);
	RETURN_THIS();
}

/**
 * This is a property to test default value on extends
 */
PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeArrayVar)
{

	RETURN_MEMBER(getThis(), "someArrayVar");
}

/**
 * @return null|mixed
 */
PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeNull)
{

	RETURN_MEMBER(getThis(), "someNull");
}

/**
 * @return void
 */
PHP_METHOD(Stub_Properties_ProtectedProperties, setSomeNull)
{
	zval *param, param_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&param_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &param);
	zephir_update_property_zval(this_ptr, ZEND_STRL("someNull"), param);
}

/**
 * @return null
 */
PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeNullInitial)
{

	RETURN_MEMBER(getThis(), "someNullInitial");
}

/**
 * @return bool
 */
PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeFalse)
{

	RETURN_MEMBER(getThis(), "someFalse");
}

/**
 * @return bool
 */
PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeTrue)
{

	RETURN_MEMBER(getThis(), "someTrue");
}

/**
 * @return int
 */
PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeInteger)
{

	RETURN_MEMBER(getThis(), "someInteger");
}

/**
 * @return float
 */
PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeDouble)
{

	RETURN_MEMBER(getThis(), "someDouble");
}

/**
 * @return string
 */
PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeString)
{

	RETURN_MEMBER(getThis(), "someString");
}

zend_object *zephir_init_properties_Stub_Properties_ProtectedProperties(zend_class_entry *class_type)
{
		zval _0, _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1$$3);
	

		ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
		zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("someArrayVar"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			array_init(&_1$$3);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("someArrayVar"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

