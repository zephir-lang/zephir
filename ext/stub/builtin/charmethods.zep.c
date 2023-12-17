
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
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/object.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Stub_BuiltIn_CharMethods)
{
	ZEPHIR_REGISTER_CLASS(Stub\\BuiltIn, CharMethods, stub, builtin_charmethods, stub_builtin_charmethods_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_BuiltIn_CharMethods, getHex)
{
	zval _0, _1;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_NULL(&_0);
	ZVAL_NULL(&_1);
	ZVAL_STRING(&_1, "a");
	zephir_string_to_hex(&_0, &_1);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_CharMethods, getHexForString)
{
	long _0;
	char ch = 0;
	zval *str_param = NULL, o, _1$$3, _2$$3, _3$$3;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZVAL_NULL(&o);
	ZVAL_STRING(&o, "");
	for (_0 = 0; _0 < Z_STRLEN_P(&str); _0++) {
		ch = ZEPHIR_STRING_OFFSET(&str, _0);
		ZEPHIR_INIT_NVAR(&_2$$3);
		ZEPHIR_INIT_NVAR(&_3$$3);
		ZVAL_STRINGL(&_3$$3, &ch, 1);
		zephir_string_to_hex(&_2$$3, &_3$$3);
		zephir_concat_self(&o, &_2$$3);
	}
	RETURN_CCTOR(&o);
}

