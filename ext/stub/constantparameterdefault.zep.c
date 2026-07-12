
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
#include "kernel/operators.h"


/**
 * Follow-up to https://github.com/zephir-lang/zephir/pull/2566
 *
 * A class constant used as a parameter default (`self::CONST`) must be folded
 * into the arg_info default value so that it is readable through Reflection,
 * the same way the property declaration and the method body already fold it.
 */
ZEPHIR_INIT_CLASS(Stub_ConstantParameterDefault)
{
	ZEPHIR_REGISTER_CLASS(Stub, ConstantParameterDefault, stub, constantparameterdefault, stub_constantparameterdefault_method_entry, 0);

	zend_declare_property_null(stub_constantparameterdefault_ce, SL("cipher"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_constantparameterdefault_ce, SL("rounds"), ZEND_ACC_PROTECTED);
	zephir_declare_class_constant_string(stub_constantparameterdefault_ce, SL("DEFAULT_CIPHER"), "aes-256-cfb");

	zephir_declare_class_constant_long(stub_constantparameterdefault_ce, SL("DEFAULT_ROUNDS"), 12);

	return SUCCESS;
}

PHP_METHOD(Stub_ConstantParameterDefault, __construct)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long rounds;
	zval *cipher_param = NULL, *rounds_param = NULL, _0;
	zval cipher;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&cipher);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("cipher", 6, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("rounds", 6, 1);
	}

	ZEND_PARSE_PARAMETERS_START(0, 2)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(cipher_param)
		Z_PARAM_LONG(rounds)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 2, &cipher_param, &rounds_param);
	if (!cipher_param) {
		ZEPHIR_INIT_VAR(&cipher);
		ZVAL_STRING(&cipher, "aes-256-cfb");
	} else {
		zephir_get_strval(&cipher, cipher_param);
	}
	if (!rounds_param) {
		rounds = 12;
	} else {
		}
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 26, &cipher);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, rounds);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_1, 27, &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_ConstantParameterDefault, getCipher)
{

	RETURN_MEMBER_TYPED(getThis(), "cipher", IS_STRING);
}

PHP_METHOD(Stub_ConstantParameterDefault, getRounds)
{

	RETURN_MEMBER_TYPED(getThis(), "rounds", IS_LONG);
}

