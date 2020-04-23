
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
#include "kernel/string.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Test_Strings) {

	ZEPHIR_REGISTER_CLASS(Test, Strings, test, strings, test_strings_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Strings, camelize) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *str_param = NULL, *delimiter = NULL, delimiter_sub, __$null;
	zval str;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&delimiter_sub);
	ZVAL_NULL(&__$null);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &str_param, &delimiter);

	zephir_get_strval(&str, str_param);
	if (!delimiter) {
		delimiter = &delimiter_sub;
		delimiter = &__$null;
	}


	zephir_camelize(return_value, &str, delimiter );
	RETURN_MM();

}

PHP_METHOD(Test_Strings, uncamelize) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *str_param = NULL, *delimiter = NULL, delimiter_sub, __$null;
	zval str;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&delimiter_sub);
	ZVAL_NULL(&__$null);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &str_param, &delimiter);

	zephir_get_strval(&str, str_param);
	if (!delimiter) {
		delimiter = &delimiter_sub;
		delimiter = &__$null;
	}


	zephir_uncamelize(return_value, &str, delimiter );
	RETURN_MM();

}

PHP_METHOD(Test_Strings, testTrim) {

	zval *str, str_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &str);



	zephir_fast_trim(return_value, str, NULL , ZEPHIR_TRIM_BOTH);
	return;

}

PHP_METHOD(Test_Strings, testRtrim) {

	zval *str, str_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &str);



	zephir_fast_trim(return_value, str, NULL , ZEPHIR_TRIM_RIGHT);
	return;

}

PHP_METHOD(Test_Strings, testLtrim) {

	zval *str, str_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &str);



	zephir_fast_trim(return_value, str, NULL , ZEPHIR_TRIM_LEFT);
	return;

}

PHP_METHOD(Test_Strings, testTrim2Params) {

	zval *str, str_sub, *charlist, charlist_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);
	ZVAL_UNDEF(&charlist_sub);

	zephir_fetch_params_without_memory_grow(2, 0, &str, &charlist);



	zephir_fast_trim(return_value, str, charlist, ZEPHIR_TRIM_BOTH);
	return;

}

PHP_METHOD(Test_Strings, testRtrim2Params) {

	zval *str, str_sub, *charlist, charlist_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);
	ZVAL_UNDEF(&charlist_sub);

	zephir_fetch_params_without_memory_grow(2, 0, &str, &charlist);



	zephir_fast_trim(return_value, str, charlist, ZEPHIR_TRIM_RIGHT);
	return;

}

PHP_METHOD(Test_Strings, testLtrim2Params) {

	zval *str, str_sub, *charlist, charlist_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);
	ZVAL_UNDEF(&charlist_sub);

	zephir_fetch_params_without_memory_grow(2, 0, &str, &charlist);



	zephir_fast_trim(return_value, str, charlist, ZEPHIR_TRIM_LEFT);
	return;

}

PHP_METHOD(Test_Strings, testImplode) {

	zval *glue, glue_sub, *pieces, pieces_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&glue_sub);
	ZVAL_UNDEF(&pieces_sub);

	zephir_fetch_params_without_memory_grow(2, 0, &glue, &pieces);



	zephir_fast_join(return_value, glue, pieces);
	return;

}

PHP_METHOD(Test_Strings, testStrpos) {

	zval *haystack, haystack_sub, *needle, needle_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&haystack_sub);
	ZVAL_UNDEF(&needle_sub);

	zephir_fetch_params_without_memory_grow(2, 0, &haystack, &needle);



	zephir_fast_strpos(return_value, haystack, needle, 0 );
	return;

}

PHP_METHOD(Test_Strings, testStrposOffset) {

	zend_long offset;
	zval *haystack, haystack_sub, *needle, needle_sub, *offset_param = NULL, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&haystack_sub);
	ZVAL_UNDEF(&needle_sub);
	ZVAL_UNDEF(&_0);

	zephir_fetch_params_without_memory_grow(3, 0, &haystack, &needle, &offset_param);

	offset = zephir_get_intval(offset_param);


	ZVAL_LONG(&_0, offset);
	zephir_fast_strpos(return_value, haystack, needle, zephir_get_intval(&_0) );
	return;

}

PHP_METHOD(Test_Strings, testExplode) {

	zval *delimiter, delimiter_sub, *str, str_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&delimiter_sub);
	ZVAL_UNDEF(&str_sub);

	zephir_fetch_params_without_memory_grow(2, 0, &delimiter, &str);



	zephir_fast_explode(return_value, delimiter, str, LONG_MAX);
	return;

}

PHP_METHOD(Test_Strings, testExplodeStr) {

	zval *str, str_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &str);



	zephir_fast_explode_str(return_value, SL(","), str, LONG_MAX);
	return;

}

PHP_METHOD(Test_Strings, testExplodeLimit) {

	zend_long limit;
	zval *str, str_sub, *limit_param = NULL, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);
	ZVAL_UNDEF(&_0);

	zephir_fetch_params_without_memory_grow(2, 0, &str, &limit_param);

	limit = zephir_get_intval(limit_param);


	ZVAL_LONG(&_0, limit);
	zephir_fast_explode_str(return_value, SL(","), str, zephir_get_intval(&_0) );
	return;

}

PHP_METHOD(Test_Strings, testSubstr) {

	zend_long from, len;
	zval *str, str_sub, *from_param = NULL, *len_param = NULL, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	zephir_fetch_params_without_memory_grow(3, 0, &str, &from_param, &len_param);

	from = zephir_get_intval(from_param);
	len = zephir_get_intval(len_param);


	ZVAL_LONG(&_0, from);
	ZVAL_LONG(&_1, len);
	zephir_substr(return_value, str, zephir_get_intval(&_0), zephir_get_intval(&_1), 0);
	return;

}

PHP_METHOD(Test_Strings, testSubstr2) {

	zend_long from;
	zval *str, str_sub, *from_param = NULL, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);
	ZVAL_UNDEF(&_0);

	zephir_fetch_params_without_memory_grow(2, 0, &str, &from_param);

	from = zephir_get_intval(from_param);


	ZVAL_LONG(&_0, from);
	zephir_substr(return_value, str, zephir_get_intval(&_0), 0, ZEPHIR_SUBSTR_NO_LENGTH);
	return;

}

PHP_METHOD(Test_Strings, testSubstr3) {

	zval *str, str_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);
	ZVAL_UNDEF(&_0);

	zephir_fetch_params_without_memory_grow(1, 0, &str);



	ZVAL_LONG(&_0, -1);
	zephir_substr(return_value, str, -1 , 0, ZEPHIR_SUBSTR_NO_LENGTH);
	return;

}

PHP_METHOD(Test_Strings, testSubstr4) {

	zval *str, str_sub, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	zephir_fetch_params_without_memory_grow(1, 0, &str);



	ZVAL_LONG(&_0, 0);
	ZVAL_LONG(&_1, -1);
	zephir_substr(return_value, str, 0 , -1 , 0);
	return;

}

PHP_METHOD(Test_Strings, testAddslashes) {

	zval *str, str_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &str);



	zephir_addslashes(return_value, str);
	return;

}

PHP_METHOD(Test_Strings, testStripslashes) {

	zval *str, str_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &str);



	zephir_stripslashes(return_value, str);
	return;

}

PHP_METHOD(Test_Strings, testStripcslashes) {

	zval *str, str_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &str);



	zephir_stripcslashes(return_value, str);
	return;

}

PHP_METHOD(Test_Strings, testHashEquals) {

	zval *str1, str1_sub, *str2, str2_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str1_sub);
	ZVAL_UNDEF(&str2_sub);

	zephir_fetch_params_without_memory_grow(2, 0, &str1, &str2);



	RETURN_BOOL(zephir_hash_equals(str1, str2));

}

PHP_METHOD(Test_Strings, testHardcodedMultilineString) {

	zval *this_ptr = getThis();


	RETURN_STRING("\n            Hello world\n        ");

}

PHP_METHOD(Test_Strings, testEchoMultilineString) {

	zval *this_ptr = getThis();


	php_printf("%s", "\n            Hello world\n        ");

}

PHP_METHOD(Test_Strings, testTrimMultilineString) {

	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "\n            Hello world\n        ");
	zephir_fast_trim(return_value, &_0, NULL , ZEPHIR_TRIM_BOTH);
	RETURN_MM();

}

PHP_METHOD(Test_Strings, testWellEscapedMultilineString) {

	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "\n            \\\"\\}\\$hello\\$\\\"\'\n        ");
	zephir_fast_trim(return_value, &_0, NULL , ZEPHIR_TRIM_BOTH);
	RETURN_MM();

}

PHP_METHOD(Test_Strings, testInternedString1) {

	zval *this_ptr = getThis();


	RETURN_STRING("hello");

}

PHP_METHOD(Test_Strings, testInternedString2) {

	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, " hello ");
	zephir_fast_trim(&_0, &_1, NULL , ZEPHIR_TRIM_BOTH);
	RETURN_CCTOR(&_0);

}

PHP_METHOD(Test_Strings, strToHex) {

	unsigned char _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, i;
	zephir_fcall_cache_entry *_4 = NULL, *_6 = NULL;
	zval *value_param = NULL, _0, _2$$3, _3$$3, _5$$3;
	zval value, ret;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value);
	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_5$$3);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &value_param);

	zephir_get_strval(&value, value_param);


	i = 0;
	ZEPHIR_INIT_VAR(&ret);
	ZVAL_STRING(&ret, "");
	while (1) {
		if (!(i < zephir_fast_strlen_ev(&value))) {
			break;
		}
		_1$$3 = ZEPHIR_STRING_OFFSET(&value, i);
		ZEPHIR_INIT_NVAR(&_2$$3);
		ZVAL_STRINGL(&_2$$3, &_1$$3, 1);
		ZEPHIR_CALL_FUNCTION(&_3$$3, "ord", &_4, 97, &_2$$3);
		zephir_check_call_status();
		ZEPHIR_CALL_FUNCTION(&_5$$3, "dechex", &_6, 10, &_3$$3);
		zephir_check_call_status();
		zephir_concat_self(&ret, &_5$$3);
		i++;
	}
	RETURN_CTOR(&ret);

}

PHP_METHOD(Test_Strings, issue1267) {

	zval _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_5 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *value = NULL, value_sub, x, _0, _2, _3, _4, _6, _7;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &value);

	ZEPHIR_SEPARATE_PARAM(value);


	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 3, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "\\");
	zephir_array_fast_append(&_1, &_2);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_STRING(&_2, "\"");
	zephir_array_fast_append(&_1, &_2);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_STRING(&_2, "'");
	zephir_array_fast_append(&_1, &_2);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_STRING(&_2, "");
	zephir_fast_str_replace(&_0, &_1, &_2, value);
	ZEPHIR_CPY_WRT(value, &_0);
	ZVAL_LONG(&_3, 513);
	ZEPHIR_CALL_FUNCTION(&_4, "filter_var", NULL, 98, value, &_3);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(value, &_4);
	ZEPHIR_INIT_NVAR(&_0);
	ZEPHIR_CALL_FUNCTION(&_4, "strip_tags", &_5, 99, value);
	zephir_check_call_status();
	zephir_stripslashes(&_0, &_4);
	ZEPHIR_INIT_VAR(&x);
	zephir_fast_trim(&x, &_0, NULL , ZEPHIR_TRIM_BOTH);
	ZEPHIR_INIT_VAR(&_6);
	ZEPHIR_CALL_FUNCTION(&_7, "strip_tags", &_5, 99, value);
	zephir_check_call_status();
	zephir_stripcslashes(&_6, &_7);
	zephir_fast_trim(return_value, &_6, NULL , ZEPHIR_TRIM_BOTH);
	RETURN_MM();

}

