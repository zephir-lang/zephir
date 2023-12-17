
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
#include "kernel/object.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Stub_Constants)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Constants, stub, constants, stub_constantsparent_ce, stub_constants_method_entry, 0);

	zend_declare_property_string(stub_constants_ce, SL("propWsVarsAssigned"), "$SOME/CSRF/KEY$", ZEND_ACC_PROTECTED);
	zend_declare_property_string(stub_constants_ce, SL("propWsVarsGet"), "$SOME/CSRF/KEY$", ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_constants_ce, SL("propertyC1"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(stub_constants_ce, SL("propertyC2"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(stub_constants_ce, SL("propertyC3"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_long(stub_constants_ce, SL("propertyC4"), 10, ZEND_ACC_PROTECTED);
	zend_declare_property_double(stub_constants_ce, SL("propertyC5"), 10.25, ZEND_ACC_PROTECTED);
	zend_declare_property_string(stub_constants_ce, SL("propertyC6"), "test", ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_constants_ce, SL("propertyC7"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_constants_ce, SL("propertyC8"), ZEND_ACC_PROTECTED);
	/**
	 * @var \Phalcon\Cache\FrontendInterface
	 */
	zend_declare_property_string(stub_constants_ce, SL("propertyC9"), "some-value", ZEND_ACC_PROTECTED);
	zephir_declare_class_constant_null(stub_constants_ce, SL("C1"));

	zephir_declare_class_constant_bool(stub_constants_ce, SL("C2"), 0);

	zephir_declare_class_constant_bool(stub_constants_ce, SL("C3"), 1);

	zephir_declare_class_constant_long(stub_constants_ce, SL("C4"), 10);

	zephir_declare_class_constant_double(stub_constants_ce, SL("C5"), 10.25);

	zephir_declare_class_constant_double(stub_constants_ce, SL("C5_SELF"), 10.25);

	zephir_declare_class_constant_double(stub_constants_ce, SL("C5_BY_NAME"), 10.25);

	zephir_declare_class_constant_string(stub_constants_ce, SL("C6"), "test");

	zephir_declare_class_constant_string(stub_constants_ce, SL("className"), "Stub\\Constants");

	zephir_declare_class_constant_long(stub_constants_ce, SL("STD_PROP_LIST"), 1);

	/** Test Issue 1571 */
	zephir_declare_class_constant_string(stub_constants_ce, SL("DEFAULT_PATH_DELIMITER"), ".");

	zephir_declare_class_constant_string(stub_constants_ce, SL("PROPERTY_WITH_VARS"), "$SOME/CSRF/KEY$");

	/**
	 * Test property addSlashes for constants
	 */
	zephir_declare_class_constant_string(stub_constants_ce, SL("ANNOTATION_REGEX"), "/@(\\w+)(?:\\s*(?:\\(\\s*)?(.*?)(?:\\s*\\))?)??\\s*(?:\n|\\*\\/)/");

	zephir_declare_class_constant_string(stub_constants_ce, SL("PARAMETER_REGEX"), "/(\\w+)\\s*=\\s*(\\[[^\\]]*\\]|\"[^\"]*\"|[^,)]*)\\s*(?:,|$)/");

	return SUCCESS;
}

PHP_METHOD(Stub_Constants, getPropWsVarsGet)
{

	RETURN_MEMBER(getThis(), "propWsVarsGet");
}

PHP_METHOD(Stub_Constants, getPropertyC1)
{

	RETURN_MEMBER(getThis(), "propertyC1");
}

PHP_METHOD(Stub_Constants, getPropertyC2)
{

	RETURN_MEMBER(getThis(), "propertyC2");
}

PHP_METHOD(Stub_Constants, getPropertyC3)
{

	RETURN_MEMBER(getThis(), "propertyC3");
}

PHP_METHOD(Stub_Constants, getPropertyC4)
{

	RETURN_MEMBER(getThis(), "propertyC4");
}

PHP_METHOD(Stub_Constants, getPropertyC5)
{

	RETURN_MEMBER(getThis(), "propertyC5");
}

PHP_METHOD(Stub_Constants, getPropertyC6)
{

	RETURN_MEMBER(getThis(), "propertyC6");
}

PHP_METHOD(Stub_Constants, getPropertyC7)
{

	RETURN_MEMBER(getThis(), "propertyC7");
}

PHP_METHOD(Stub_Constants, getPropertyC8)
{

	RETURN_MEMBER(getThis(), "propertyC8");
}

/**
 */
PHP_METHOD(Stub_Constants, getPropertyC9)
{

	RETURN_MEMBER(getThis(), "propertyC9");
}

PHP_METHOD(Stub_Constants, testReadConstant)
{

	RETURN_LONG(3);
}

PHP_METHOD(Stub_Constants, testReadClassConstant1)
{

	RETURN_LONG(10);
}

PHP_METHOD(Stub_Constants, testReadClassConstant2)
{

	RETURN_LONG(10);
}

PHP_METHOD(Stub_Constants, testReadClassConstant3)
{

	RETURN_LONG(10);
}

PHP_METHOD(Stub_Constants, testPHPVersionEnvConstant)
{

	ZEPHIR_GET_CONSTANT(return_value, "PHP_VERSION");
	return;
}

PHP_METHOD(Stub_Constants, testClassMagicConstant)
{

	RETURN_STRING("Stub\\Constants");
}

PHP_METHOD(Stub_Constants, testMethodMagicConstant)
{

	RETURN_STRING("Constants:testMethodMagicConstant");
}

PHP_METHOD(Stub_Constants, testFunctionMagicConstant)
{

	RETURN_STRING("testFunctionMagicConstant");
}

PHP_METHOD(Stub_Constants, testNamespaceMagicConstant)
{

	RETURN_STRING("Stub");
}

PHP_METHOD(Stub_Constants, testDirConstant)
{

	RETURN_NULL();
}

PHP_METHOD(Stub_Constants, testPHPVersionEnvConstantInExpValue)
{
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_NULL(&a);
	ZEPHIR_GET_CONSTANT(&a, "PHP_VERSION");
	RETURN_CCTOR(&a);
}

/**
 * Test Delimiters as String Constants
 *
 * @link https://github.com/zephir-lang/zephir/issues/1571
 */
PHP_METHOD(Stub_Constants, testStringDelimiterConstantDoubleQuoted)
{
	zval delimiter;

	ZVAL_UNDEF(&delimiter);
	ZVAL_NULL(&delimiter);
	ZVAL_STRING(&delimiter, ".");
	RETURN_CCTOR(&delimiter);
}

PHP_METHOD(Stub_Constants, testStringConstantWithVars)
{
	zval property;

	ZVAL_UNDEF(&property);
	ZVAL_NULL(&property);
	ZVAL_STRING(&property, "$SOME/CSRF/KEY$");
	RETURN_CCTOR(&property);
}

PHP_METHOD(Stub_Constants, testStringPropertyWithVarsAssigned)
{

	RETURN_MEMBER(getThis(), "propWsVarsAssigned");
}

PHP_METHOD(Stub_Constants, testStringPropertyWithVarsGet)
{

	RETURN_MEMBER(getThis(), "propWsVarsGet");
}

