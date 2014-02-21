
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
#include "kernel/fcall.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_BuiltIn_CharMethods) {

	ZEPHIR_REGISTER_CLASS(Test\\BuiltIn, CharMethods, test, builtin_charmethods, test_builtin_charmethods_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_BuiltIn_CharMethods, getHex) {

	zval _0, _1, *_2;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "%X", 0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_LONG(&_1, 'a');
	ZEPHIR_INIT_VAR(_2);
	zephir_call_func_p2(_2, "sprintf", &_0, &_1);
	RETURN_CCTOR(_2);

}

PHP_METHOD(Test_BuiltIn_CharMethods, getHexForString) {

	zend_function *_5 = NULL;
	int _0;
	char ch;
	zval *str_param = NULL, *o, *_1 = NULL, _2 = zval_used_for_init, _3 = zval_used_for_init, *_4 = NULL;
	zval *str = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &str_param);

	zephir_get_strval(str, str_param);
	ZEPHIR_INIT_VAR(o);
	ZVAL_STRING(o, "", 1);


	for (_0 = 0; _0 < Z_STRLEN_P(str); _0++) {
		ch = ZEPHIR_STRING_OFFSET(str, _0);
		ZEPHIR_INIT_NVAR(_1);
		ZEPHIR_SINIT_NVAR(_2);
		ZVAL_STRING(&_2, "%X", 0);
		ZEPHIR_SINIT_NVAR(_3);
		ZVAL_LONG(&_3, ch);
		ZEPHIR_INIT_NVAR(_4);
		ZEPHIR_CALL_INTERNAL_FUNCTION(_4, &_4, "sprintf", &_5, 2, &_2, &_3);
		zephir_concat_self(&o, _4 TSRMLS_CC);
	}
	RETURN_CCTOR(o);

}

