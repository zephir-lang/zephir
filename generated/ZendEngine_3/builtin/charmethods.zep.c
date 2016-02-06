
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

	zval _0, _1, _2;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "%X");
	ZVAL_LONG(&_1, 'a');
	ZEPHIR_CALL_FUNCTION(&_2, "sprintf", NULL, 4, &_0, &_1);
	zephir_check_call_status();
	RETURN_CCTOR(_2);

}

PHP_METHOD(Test_BuiltIn_CharMethods, getHexForString) {

	long _0;
	char ch = 0;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_5 = NULL;
	zval *str_param = NULL, o, _1$$3, _2$$3, _3$$3, _4$$3;
	zval str;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &str_param);

	zephir_get_strval(&str, str_param);


	ZEPHIR_INIT_VAR(&o);
	ZVAL_STRING(&o, "");
	for (_0 = 0; _0 < Z_STRLEN_P(&str); _0++) {
		ch = ZEPHIR_STRING_OFFSET(&str, _0);
		ZEPHIR_INIT_NVAR(&_2$$3);
		ZVAL_STRING(&_2$$3, "%X");
		ZVAL_LONG(&_3$$3, ch);
		ZEPHIR_CALL_FUNCTION(&_4$$3, "sprintf", &_5, 4, &_2$$3, &_3$$3);
		zephir_check_call_status();
		zephir_concat_self(&o, &_4$$3 TSRMLS_CC);
	}
	RETURN_CCTOR(o);

}

