
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
 * Property shortcuts take their accessor types from the declared property
 * type, falling back to a `@var` docblock that may name a class.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2761
 */
ZEPHIR_INIT_CLASS(Stub_Issue2761_Accessors)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Issue2761, Accessors, stub, issue2761_accessors, stub_issue2761_accessors_method_entry, 0);

	{
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "", sizeof("") - 1);
		zephir_declare_typed_property(stub_issue2761_accessors_ce, SL("name"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_STRING, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_UNDEF(&_zc0);
		zephir_declare_typed_property(stub_issue2761_accessors_ce, SL("thing"), &_zc0, ZEND_ACC_PROTECTED, 0, SL("Stub\\Issue2761\\Collaborator"));
	}

	{
		zval _zc0;
		ZVAL_NULL(&_zc0);
		zephir_declare_typed_property(stub_issue2761_accessors_ce, SL("maybe"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_NULL, SL("Stub\\Issue2761\\Collaborator"));
	}

	{
		zval _zc0;
		ZVAL_NULL(&_zc0);
		zephir_declare_typed_property(stub_issue2761_accessors_ce, SL("count"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_LONG|MAY_BE_NULL, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_UNDEF(&_zc0);
		zephir_declare_typed_property(stub_issue2761_accessors_ce, SL("pending"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_LONG, NULL, 0);
	}

	/** @var \Stub\Issue2761\Collaborator */
	zend_declare_property_null(stub_issue2761_accessors_ce, SL("docQualified"), ZEND_ACC_PROTECTED);
	/** @var Collaborator */
	zend_declare_property_null(stub_issue2761_accessors_ce, SL("docRelative"), ZEND_ACC_PROTECTED);
	/** @var Collaborator|null */
	zend_declare_property_null(stub_issue2761_accessors_ce, SL("docNullable"), ZEND_ACC_PROTECTED);
	/** @var Collaborator */
	{
		zval _zc0;
		ZVAL_LONG(&_zc0, 0);
		zephir_declare_typed_property(stub_issue2761_accessors_ce, SL("declaredWins"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_LONG, NULL, 0);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2761_Accessors, getName)
{

	RETURN_MEMBER_VERIFIED(getThis(), "name");
}

PHP_METHOD(Stub_Issue2761_Accessors, setName)
{
	zval name_zv;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("name", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&name_zv, name);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 95, &name_zv);
	RETURN_THISW();
}

PHP_METHOD(Stub_Issue2761_Accessors, getThing)
{

	RETURN_MEMBER_VERIFIED(getThis(), "thing");
}

PHP_METHOD(Stub_Issue2761_Accessors, setThing)
{
	zval *thing, thing_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&thing_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("thing", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(thing, stub_issue2761_collaborator_ce)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &thing);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 96, thing);
	RETURN_THISW();
}

PHP_METHOD(Stub_Issue2761_Accessors, getMaybe)
{

	RETURN_MEMBER_VERIFIED(getThis(), "maybe");
}

PHP_METHOD(Stub_Issue2761_Accessors, setMaybe)
{
	zval *maybe, maybe_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&maybe_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("maybe", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(maybe)
	ZEND_PARSE_PARAMETERS_END();
	if (maybe && UNEXPECTED(!zephir_verify_union_arg(maybe, 1))) {
		RETURN_THROWS();
	}
	zephir_fetch_params_without_memory_grow(1, 0, &maybe);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 97, maybe);
	RETURN_THISW();
}

PHP_METHOD(Stub_Issue2761_Accessors, getCount)
{

	RETURN_MEMBER_VERIFIED(getThis(), "count");
}

PHP_METHOD(Stub_Issue2761_Accessors, setCount)
{
	zval *count, count_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&count_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("count", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(count)
	ZEND_PARSE_PARAMETERS_END();
	if (count && UNEXPECTED(!zephir_verify_union_arg(count, 1))) {
		RETURN_THROWS();
	}
	zephir_fetch_params_without_memory_grow(1, 0, &count);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 98, count);
	RETURN_THISW();
}

PHP_METHOD(Stub_Issue2761_Accessors, getPending)
{

	RETURN_MEMBER_VERIFIED(getThis(), "pending");
}

/**
 */
PHP_METHOD(Stub_Issue2761_Accessors, getDocQualified)
{

	RETURN_MEMBER_VERIFIED(getThis(), "docQualified");
}

/**
 */
PHP_METHOD(Stub_Issue2761_Accessors, setDocQualified)
{
	zval *docQualified, docQualified_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&docQualified_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("docQualified", 12, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(docQualified, stub_issue2761_collaborator_ce)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &docQualified);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 99, docQualified);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2761_Accessors, getDocRelative)
{

	RETURN_MEMBER_VERIFIED(getThis(), "docRelative");
}

/**
 */
PHP_METHOD(Stub_Issue2761_Accessors, getDocNullable)
{

	RETURN_MEMBER_VERIFIED(getThis(), "docNullable");
}

/**
 */
PHP_METHOD(Stub_Issue2761_Accessors, setDocNullable)
{
	zval *docNullable, docNullable_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&docNullable_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("docNullable", 11, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(docNullable)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &docNullable);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 100, docNullable);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2761_Accessors, getDeclaredWins)
{

	RETURN_MEMBER_VERIFIED(getThis(), "declaredWins");
}

/**
 */
PHP_METHOD(Stub_Issue2761_Accessors, setDeclaredWins)
{
	zval *declaredWins_param = NULL, _0;
	zend_long declaredWins;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("declaredWins", 12, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(declaredWins)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &declaredWins_param);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, declaredWins);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 101, &_0);
	RETURN_THISW();
}

