
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


/**
 * A getter that returns `this->prop` verifies the value against the return
 * type it advertises, as PHP does for a hand-written method.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2391
 */
ZEPHIR_INIT_CLASS(Stub_Issue2391_Accessors)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Issue2391, Accessors, stub, issue2391_accessors, stub_issue2391_accessors_method_entry, 0);

	/** @var bool */
	zend_declare_property_null(stub_issue2391_accessors_ce, SL("flag"), ZEND_ACC_PROTECTED);
	/** @var \Stub\Issue2391\Collaborator */
	zend_declare_property_null(stub_issue2391_accessors_ce, SL("thing"), ZEND_ACC_PROTECTED);
	/** @var object */
	zend_declare_property_null(stub_issue2391_accessors_ce, SL("obj"), ZEND_ACC_PROTECTED);
	/** @var Collaborator|null */
	zend_declare_property_null(stub_issue2391_accessors_ce, SL("maybe"), ZEND_ACC_PROTECTED);
	/** @var int|string */
	zend_declare_property_null(stub_issue2391_accessors_ce, SL("key"), ZEND_ACC_PROTECTED);
	/** @var array|false */
	zend_declare_property_null(stub_issue2391_accessors_ce, SL("listOrFalse"), ZEND_ACC_PROTECTED);
	/** @var double */
	zend_declare_property_null(stub_issue2391_accessors_ce, SL("ratio"), ZEND_ACC_PROTECTED);
	/** @var string */
	zend_declare_property_null(stub_issue2391_accessors_ce, SL("name"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_issue2391_accessors_ce, SL("label"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

/**
 */
PHP_METHOD(Stub_Issue2391_Accessors, getFlag)
{

	RETURN_MEMBER_VERIFIED(getThis(), "flag");
}

/**
 */
PHP_METHOD(Stub_Issue2391_Accessors, getThing)
{

	RETURN_MEMBER_VERIFIED(getThis(), "thing");
}

/**
 */
PHP_METHOD(Stub_Issue2391_Accessors, getObj)
{

	RETURN_MEMBER_VERIFIED(getThis(), "obj");
}

/**
 */
PHP_METHOD(Stub_Issue2391_Accessors, getMaybe)
{

	RETURN_MEMBER_VERIFIED(getThis(), "maybe");
}

/**
 */
PHP_METHOD(Stub_Issue2391_Accessors, getKey)
{

	RETURN_MEMBER_VERIFIED(getThis(), "key");
}

/**
 */
PHP_METHOD(Stub_Issue2391_Accessors, getListOrFalse)
{

	RETURN_MEMBER_VERIFIED(getThis(), "listOrFalse");
}

/**
 */
PHP_METHOD(Stub_Issue2391_Accessors, getRatio)
{

	RETURN_MEMBER_VERIFIED(getThis(), "ratio");
}

/**
 */
PHP_METHOD(Stub_Issue2391_Accessors, getName)
{

	RETURN_MEMBER_VERIFIED(getThis(), "name");
}

PHP_METHOD(Stub_Issue2391_Accessors, __toString)
{

	RETURN_MEMBER_VERIFIED(getThis(), "label");
}

/**
 * Stores any value, so a test can put a wrong-typed one behind a getter.
 */
PHP_METHOD(Stub_Issue2391_Accessors, poke)
{
	zval property_zv, *value, value_sub;
	zend_string *property = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property_zv);
	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(property)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	value = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&property_zv, property);
	zephir_update_property_zval_zval(this_ptr, &property_zv, value);
	RETURN_THISW();
}

PHP_METHOD(Stub_Issue2391_Accessors, flagByHand)
{

	RETURN_MEMBER_VERIFIED(getThis(), "flag");
}

PHP_METHOD(Stub_Issue2391_Accessors, thingByHand)
{

	RETURN_MEMBER_VERIFIED(getThis(), "thing");
}

PHP_METHOD(Stub_Issue2391_Accessors, maybeByHand)
{

	RETURN_MEMBER_VERIFIED(getThis(), "maybe");
}

PHP_METHOD(Stub_Issue2391_Accessors, objByHand)
{

	RETURN_MEMBER_VERIFIED(getThis(), "obj");
}

