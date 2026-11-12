
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
#include "kernel/operators.h"


/**
 * Properties a reader outside the class cannot reach, or not yet.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2831
 */
ZEPHIR_INIT_CLASS(Stub_Issue2831Base)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2831Base, stub, issue2831base, stub_issue2831base_method_entry, 0);

	zend_declare_property_string(stub_issue2831base_ce, SL("prot"), "prot", ZEND_ACC_PROTECTED);
	zend_declare_property_string(stub_issue2831base_ce, SL("priv"), "priv", ZEND_ACC_PRIVATE);
	{
		zval _zc0;
		ZVAL_UNDEF(&_zc0);
		zephir_declare_typed_property(stub_issue2831base_ce, SL("typed"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_LONG, NULL, 0);
	}

	return SUCCESS;
}

/**
 * A read of `this` that borrows the property instead of copying it.
 */
PHP_METHOD(Stub_Issue2831Base, readOwnTyped)
{
	zval value;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("typed", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_memory_observe(&value);
	zephir_read_property_cached(&value, this_ptr, _zephir_prop_0, 5, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&value);
}

PHP_METHOD(Stub_Issue2831Base, readOwnTypedInCondition)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("typed", 5, 1);
	}
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 5, PH_NOISY_CC | PH_READONLY);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	if (zephir_is_true(&_0)) {
		RETURN_STRING("yes");
	}
	RETURN_STRING("no");
}

/**
 * Another instance of the same class may be read from here.
 */
PHP_METHOD(Stub_Issue2831Base, readOtherProtected)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, _0;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("prot", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(o)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &o);
	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, o, _zephir_prop_0, 0, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_0);
}

