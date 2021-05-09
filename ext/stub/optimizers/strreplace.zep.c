
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
#include "kernel/object.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_Optimizers_StrReplace)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Optimizers, StrReplace, stub, optimizers_strreplace, stub_optimizers_strreplace_method_entry, 0);

	return SUCCESS;
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/1055
 */
PHP_METHOD(Stub_Optimizers_StrReplace, issue1055)
{
	zval _1, _3;
	zval _0, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "aa");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "aaa");
	zephir_array_fast_append(&_2, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "bla bla aa aaa askks");
	ZEPHIR_INIT_VAR(&_3);
	zephir_fast_str_replace(&_3, &_0, &_2, &_1);
	RETURN_CCTOR(&_3);
}

PHP_METHOD(Stub_Optimizers_StrReplace, issue1087)
{
	zval filtered, sanitize, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&filtered);
	ZVAL_UNDEF(&sanitize);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&filtered);
	ZVAL_STRING(&filtered, "test_test");
	ZEPHIR_INIT_VAR(&sanitize);
	zephir_create_array(&sanitize, 8, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "/");
	zephir_array_fast_append(&sanitize, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "'");
	zephir_array_fast_append(&sanitize, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "\"");
	zephir_array_fast_append(&sanitize, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "?");
	zephir_array_fast_append(&sanitize, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "!");
	zephir_array_fast_append(&sanitize, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "%");
	zephir_array_fast_append(&sanitize, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "$");
	zephir_array_fast_append(&sanitize, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "&");
	zephir_array_fast_append(&sanitize, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "");
	zephir_fast_str_replace(&_0, &sanitize, &_1, &filtered);
	ZEPHIR_CPY_WRT(&filtered, &_0);
	RETURN_CCTOR(&filtered);
}

PHP_METHOD(Stub_Optimizers_StrReplace, issue732A)
{
	zval subject;
	zval _0;
	zval search, replace;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&search);
	ZVAL_UNDEF(&replace);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&subject);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&search);
	zephir_create_array(&search, 5, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "A");
	zephir_array_fast_append(&search, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "B");
	zephir_array_fast_append(&search, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "C");
	zephir_array_fast_append(&search, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "D");
	zephir_array_fast_append(&search, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "E");
	zephir_array_fast_append(&search, &_0);
	ZEPHIR_INIT_VAR(&replace);
	zephir_create_array(&replace, 5, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "B");
	zephir_array_fast_append(&replace, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "C");
	zephir_array_fast_append(&replace, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "D");
	zephir_array_fast_append(&replace, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "E");
	zephir_array_fast_append(&replace, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "F");
	zephir_array_fast_append(&replace, &_0);
	ZEPHIR_INIT_VAR(&subject);
	ZVAL_STRING(&subject, "A");
	ZEPHIR_INIT_NVAR(&_0);
	zephir_fast_str_replace(&_0, &search, &replace, &subject);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Optimizers_StrReplace, issue732B)
{
	zval subject;
	zval _0;
	zval patterns, replacements;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&patterns);
	ZVAL_UNDEF(&replacements);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&subject);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&patterns);
	zephir_create_array(&patterns, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "/quick/");
	zephir_array_fast_append(&patterns, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "/brown/");
	zephir_array_fast_append(&patterns, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "/fox/");
	zephir_array_fast_append(&patterns, &_0);
	ZEPHIR_INIT_VAR(&replacements);
	zephir_create_array(&replacements, 3, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "bear");
	zephir_array_fast_append(&replacements, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "black");
	zephir_array_fast_append(&replacements, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "slow");
	zephir_array_fast_append(&replacements, &_0);
	ZEPHIR_INIT_VAR(&subject);
	ZVAL_STRING(&subject, "The quick brown fox jumped over the lazy dog.");
	ZEPHIR_RETURN_CALL_FUNCTION("preg_replace", NULL, 72, &patterns, &replacements, &subject);
	zephir_check_call_status();
	RETURN_MM();
}

