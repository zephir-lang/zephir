
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
#include "kernel/operators.h"
#include "kernel/memory.h"


/**
 * PHPDoc `@var` types map onto Zephir types, and any other one leaves the
 * accessors untyped instead of failing the build.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2391
 */
ZEPHIR_INIT_CLASS(Stub_Issue2391_DocblockTypes)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Issue2391, DocblockTypes, stub, issue2391_docblocktypes, stub_issue2391_docblocktypes_method_entry, 0);

	/** @var float */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("floatValue"), ZEND_ACC_PROTECTED);
	/** @var integer */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("integerValue"), ZEND_ACC_PROTECTED);
	/** @var boolean */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("booleanValue"), ZEND_ACC_PROTECTED);
	/** @var ?int */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("nullableInt"), ZEND_ACC_PROTECTED);
	/** @var ?Collaborator */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("nullableThing"), ZEND_ACC_PROTECTED);
	/** @var string[] */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("strings"), ZEND_ACC_PROTECTED);
	/** @var array<int, string> */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("generic"), ZEND_ACC_PROTECTED);
	/** @var Collaborator[] */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("things"), ZEND_ACC_PROTECTED);
	/** @var numeric */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("numericValue"), ZEND_ACC_PROTECTED);
	/** @var scalar */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("scalarValue"), ZEND_ACC_PROTECTED);
	/** @var self */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("selfValue"), ZEND_ACC_PROTECTED);
	/** @var iterable */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("iterableValue"), ZEND_ACC_PROTECTED);
	/** @var void */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("voidValue"), ZEND_ACC_PROTECTED);
	/** @var true */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("trueValue"), ZEND_ACC_PROTECTED);
	/** @var null */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("nullValue"), ZEND_ACC_PROTECTED);
	/** @var numeric|null */
	zend_declare_property_null(stub_issue2391_docblocktypes_ce, SL("numericOrNull"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getFloatValue)
{

	RETURN_MEMBER_VERIFIED(getThis(), "floatValue");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setFloatValue)
{
	zval *floatValue_param = NULL, _0;
	double floatValue;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("floatValue", 10, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(floatValue)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &floatValue_param);
	ZVAL_UNDEF(&_0);
	ZVAL_DOUBLE(&_0, floatValue);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 62, &_0);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getIntegerValue)
{

	RETURN_MEMBER_VERIFIED(getThis(), "integerValue");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setIntegerValue)
{
	zval *integerValue_param = NULL, _0;
	zend_long integerValue;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("integerValue", 12, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(integerValue)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &integerValue_param);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, integerValue);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 63, &_0);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getBooleanValue)
{

	RETURN_MEMBER_VERIFIED(getThis(), "booleanValue");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setBooleanValue)
{
	zval *booleanValue_param = NULL, __$true, __$false;
	zend_bool booleanValue;
	zval *this_ptr = getThis();

	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("booleanValue", 12, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(booleanValue)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &booleanValue_param);
	if (booleanValue) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 64, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 64, &__$false);
	}
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getNullableInt)
{

	RETURN_MEMBER_VERIFIED(getThis(), "nullableInt");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setNullableInt)
{
	zval *nullableInt, nullableInt_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&nullableInt_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("nullableInt", 11, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(nullableInt)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &nullableInt);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 65, nullableInt);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getNullableThing)
{

	RETURN_MEMBER_VERIFIED(getThis(), "nullableThing");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setNullableThing)
{
	zval *nullableThing, nullableThing_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&nullableThing_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("nullableThing", 13, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(nullableThing)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &nullableThing);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 66, nullableThing);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getStrings)
{

	RETURN_MEMBER_VERIFIED(getThis(), "strings");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setStrings)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *strings_param = NULL;
	zval strings;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&strings);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("strings", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(strings, strings_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &strings_param);
	zephir_get_arrval(&strings, strings_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 67, &strings);
	RETURN_THIS();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getGeneric)
{

	RETURN_MEMBER_VERIFIED(getThis(), "generic");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setGeneric)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *generic_param = NULL;
	zval generic;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&generic);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("generic", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(generic, generic_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &generic_param);
	zephir_get_arrval(&generic, generic_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 68, &generic);
	RETURN_THIS();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getThings)
{

	RETURN_MEMBER_VERIFIED(getThis(), "things");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setThings)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *things_param = NULL;
	zval things;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&things);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("things", 6, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(things, things_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &things_param);
	zephir_get_arrval(&things, things_param);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 69, &things);
	RETURN_THIS();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getNumericValue)
{

	RETURN_MEMBER(getThis(), "numericValue");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setNumericValue)
{
	zval *numericValue, numericValue_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&numericValue_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("numericValue", 12, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(numericValue)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &numericValue);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 70, numericValue);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getScalarValue)
{

	RETURN_MEMBER(getThis(), "scalarValue");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setScalarValue)
{
	zval *scalarValue, scalarValue_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&scalarValue_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("scalarValue", 11, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(scalarValue)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &scalarValue);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 71, scalarValue);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getSelfValue)
{

	RETURN_MEMBER(getThis(), "selfValue");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setSelfValue)
{
	zval *selfValue, selfValue_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&selfValue_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("selfValue", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(selfValue)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &selfValue);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 72, selfValue);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getIterableValue)
{

	RETURN_MEMBER(getThis(), "iterableValue");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setIterableValue)
{
	zval *iterableValue, iterableValue_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&iterableValue_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("iterableValue", 13, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(iterableValue)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &iterableValue);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 73, iterableValue);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getVoidValue)
{

	RETURN_MEMBER(getThis(), "voidValue");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setVoidValue)
{
	zval *voidValue, voidValue_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&voidValue_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("voidValue", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(voidValue)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &voidValue);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 74, voidValue);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getTrueValue)
{

	RETURN_MEMBER(getThis(), "trueValue");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setTrueValue)
{
	zval *trueValue, trueValue_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&trueValue_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("trueValue", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(trueValue)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &trueValue);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 75, trueValue);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getNullValue)
{

	RETURN_MEMBER(getThis(), "nullValue");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setNullValue)
{
	zval *nullValue, nullValue_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&nullValue_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("nullValue", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(nullValue)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &nullValue);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, nullValue);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, getNumericOrNull)
{

	RETURN_MEMBER(getThis(), "numericOrNull");
}

/**
 */
PHP_METHOD(Stub_Issue2391_DocblockTypes, setNumericOrNull)
{
	zval *numericOrNull, numericOrNull_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&numericOrNull_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("numericOrNull", 13, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(numericOrNull)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &numericOrNull);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, numericOrNull);
	RETURN_THISW();
}

