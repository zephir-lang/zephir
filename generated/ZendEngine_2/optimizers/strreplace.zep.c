
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
#include "kernel/string.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Test_Optimizers_StrReplace) {

	ZEPHIR_REGISTER_CLASS(Test\\Optimizers, StrReplace, test, optimizers_strreplace, test_optimizers_strreplace_method_entry, 0);

	return SUCCESS;

}

/**
 * @link https://github.com/phalcon/zephir/issues/1055
 */
PHP_METHOD(Test_Optimizers_StrReplace, issue1055) {

	zval *_1 = NULL, _3;
	zval *_0, *_2;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "aa", 1);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_VAR(_2);
	zephir_create_array(_2, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_STRING(_1, "aaa", 1);
	zephir_array_fast_append(_2, _1);
	ZEPHIR_SINIT_VAR(_3);
	ZVAL_STRING(&_3, "bla bla aa aaa askks", 0);
	ZEPHIR_INIT_NVAR(_1);
	zephir_fast_str_replace(&_1, _0, _2, &_3 TSRMLS_CC);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_Optimizers_StrReplace, issue1087) {

	zval *filtered = NULL, *sanitize = NULL, *_0 = NULL, _1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(filtered);
	ZVAL_STRING(filtered, "test_test", 1);
	ZEPHIR_INIT_VAR(sanitize);
	zephir_create_array(sanitize, 8, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "/", 1);
	zephir_array_fast_append(sanitize, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "'", 1);
	zephir_array_fast_append(sanitize, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "\"", 1);
	zephir_array_fast_append(sanitize, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "?", 1);
	zephir_array_fast_append(sanitize, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "!", 1);
	zephir_array_fast_append(sanitize, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "%", 1);
	zephir_array_fast_append(sanitize, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "$", 1);
	zephir_array_fast_append(sanitize, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "&", 1);
	zephir_array_fast_append(sanitize, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_STRING(&_1, "", 0);
	zephir_fast_str_replace(&_0, sanitize, &_1, filtered TSRMLS_CC);
	ZEPHIR_CPY_WRT(filtered, _0);
	RETURN_CCTOR(filtered);

}

PHP_METHOD(Test_Optimizers_StrReplace, issue732A) {

	zval *subject;
	zval *_0 = NULL;
	zval *search, *replace;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(search);
	zephir_create_array(search, 5, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "A", 1);
	zephir_array_fast_append(search, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "B", 1);
	zephir_array_fast_append(search, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "C", 1);
	zephir_array_fast_append(search, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "D", 1);
	zephir_array_fast_append(search, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "E", 1);
	zephir_array_fast_append(search, _0);
	ZEPHIR_INIT_VAR(replace);
	zephir_create_array(replace, 5, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "B", 1);
	zephir_array_fast_append(replace, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "C", 1);
	zephir_array_fast_append(replace, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "D", 1);
	zephir_array_fast_append(replace, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "E", 1);
	zephir_array_fast_append(replace, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "F", 1);
	zephir_array_fast_append(replace, _0);
	ZEPHIR_INIT_VAR(subject);
	ZVAL_STRING(subject, "A", 1);
	ZEPHIR_INIT_NVAR(_0);
	zephir_fast_str_replace(&_0, search, replace, subject TSRMLS_CC);
	RETURN_CCTOR(_0);

}

PHP_METHOD(Test_Optimizers_StrReplace, issue732B) {

	zval *subject;
	zval *_0 = NULL;
	zval *patterns, *replacements;
	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(patterns);
	zephir_create_array(patterns, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "/quick/", 1);
	zephir_array_fast_append(patterns, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "/brown/", 1);
	zephir_array_fast_append(patterns, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "/fox/", 1);
	zephir_array_fast_append(patterns, _0);
	ZEPHIR_INIT_VAR(replacements);
	zephir_create_array(replacements, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "bear", 1);
	zephir_array_fast_append(replacements, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "black", 1);
	zephir_array_fast_append(replacements, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "slow", 1);
	zephir_array_fast_append(replacements, _0);
	ZEPHIR_INIT_VAR(subject);
	ZVAL_STRING(subject, "The quick brown fox jumped over the lazy dog.", 1);
	ZEPHIR_RETURN_CALL_FUNCTION("preg_replace", NULL, 51, patterns, replacements, subject);
	zephir_check_call_status();
	RETURN_MM();

}

