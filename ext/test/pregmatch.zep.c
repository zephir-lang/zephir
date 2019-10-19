
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
#include "kernel/string.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Test_Pregmatch) {

	ZEPHIR_REGISTER_CLASS(Test, Pregmatch, test, pregmatch, test_pregmatch_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Pregmatch, testWithoutReturnAndMatches) {

	zval pattern, subject, _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern);
	ZVAL_UNDEF(&subject);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&pattern);
	ZVAL_STRING(&pattern, "/def$/");
	ZEPHIR_INIT_VAR(&subject);
	ZVAL_STRING(&subject, "abcdef");
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_preg_match(&_1, &pattern, &subject, &_0, 0, 0 , 0 );
	ZEPHIR_INIT_VAR(&_2);
	zephir_preg_match(return_value, &pattern, &subject, &_2, 0, 0 , 0 );
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testWithoutReturns) {

	zval pattern, subject, matches, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern);
	ZVAL_UNDEF(&subject);
	ZVAL_UNDEF(&matches);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&matches);
	ZVAL_NULL(&matches);
	ZEPHIR_INIT_VAR(&pattern);
	ZVAL_STRING(&pattern, "/def$/");
	ZEPHIR_INIT_VAR(&subject);
	ZVAL_STRING(&subject, "abcdef");
	ZEPHIR_INIT_VAR(&_0);
	zephir_preg_match(&_0, &pattern, &subject, &matches, 0, 0 , 0 );
	RETURN_CCTOR(&matches);

}

PHP_METHOD(Test_Pregmatch, testWithoutMatches) {

	zval pattern, subject, matched, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern);
	ZVAL_UNDEF(&subject);
	ZVAL_UNDEF(&matched);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&pattern);
	ZVAL_STRING(&pattern, "/def$/");
	ZEPHIR_INIT_VAR(&subject);
	ZVAL_STRING(&subject, "abcdef");
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_VAR(&matched);
	zephir_preg_match(&matched, &pattern, &subject, &_0, 0, 0 , 0 );
	RETURN_CCTOR(&matched);

}

PHP_METHOD(Test_Pregmatch, testPregMatchAll) {

	zval pattern, subject, results;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern);
	ZVAL_UNDEF(&subject);
	ZVAL_UNDEF(&results);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&pattern);
	ZVAL_STRING(&pattern, "/def$/");
	ZEPHIR_INIT_VAR(&subject);
	ZVAL_STRING(&subject, "abcdef");
	ZEPHIR_INIT_VAR(&results);
	array_init(&results);
	zephir_preg_match(return_value, &pattern, &subject, &results, 1, 0 , 0 );
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testPregMatchFallback) {

	zval pattern, subject, matches;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern);
	ZVAL_UNDEF(&subject);
	ZVAL_UNDEF(&matches);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&matches);
	ZVAL_NULL(&matches);
	ZEPHIR_INIT_NVAR(&matches);
	array_init(&matches);
	ZEPHIR_INIT_VAR(&pattern);
	ZVAL_STRING(&pattern, "/def$/");
	ZEPHIR_INIT_VAR(&subject);
	ZVAL_STRING(&subject, "abcdef");
	zephir_preg_match(return_value, &pattern, &subject, &matches, 0, 0 , 0 );
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testPregMatch2Params) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *pattern, pattern_sub, *subject, subject_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&subject_sub);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &pattern, &subject);



	ZEPHIR_INIT_VAR(&_0);
	zephir_preg_match(return_value, pattern, subject, &_0, 0, 0 , 0 );
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testPregMatch3Params) {

	zval *pattern, pattern_sub, *subject, subject_sub, *matches, matches_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&subject_sub);
	ZVAL_UNDEF(&matches_sub);

	zephir_fetch_params_without_memory_grow(3, 0, &pattern, &subject, &matches);



	zephir_preg_match(return_value, pattern, subject, matches, 0, 0 , 0 );
	return;

}

PHP_METHOD(Test_Pregmatch, testPregMatch4Params) {

	zval *pattern, pattern_sub, *subject, subject_sub, *matches, matches_sub, *flags, flags_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&subject_sub);
	ZVAL_UNDEF(&matches_sub);
	ZVAL_UNDEF(&flags_sub);

	zephir_fetch_params_without_memory_grow(4, 0, &pattern, &subject, &matches, &flags);



	zephir_preg_match(return_value, pattern, subject, matches, 0, zephir_get_intval(flags) , 0 );
	return;

}

PHP_METHOD(Test_Pregmatch, testPregMatch5Params) {

	zval *pattern, pattern_sub, *subject, subject_sub, *matches, matches_sub, *flags, flags_sub, *offset, offset_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&pattern_sub);
	ZVAL_UNDEF(&subject_sub);
	ZVAL_UNDEF(&matches_sub);
	ZVAL_UNDEF(&flags_sub);
	ZVAL_UNDEF(&offset_sub);

	zephir_fetch_params_without_memory_grow(5, 0, &pattern, &subject, &matches, &flags, &offset);



	zephir_preg_match(return_value, pattern, subject, matches, 0, zephir_get_intval(flags) , zephir_get_intval(offset) );
	return;

}

/**
 * @link https://github.com/phalcon/zephir/issues/287
 */
PHP_METHOD(Test_Pregmatch, testPregMatchSaveMatches) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *str_param = NULL, *pattern_param = NULL, matches, _0;
	zval str, pattern;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&pattern);
	ZVAL_UNDEF(&matches);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &str_param, &pattern_param);

	zephir_get_strval(&str, str_param);
	zephir_get_strval(&pattern, pattern_param);


	ZEPHIR_INIT_VAR(&matches);
	ZVAL_NULL(&matches);
	ZEPHIR_INIT_VAR(&_0);
	zephir_preg_match(&_0, &pattern, &str, &matches, 0, 0 , 0 );
	RETURN_CCTOR(&matches);

}

PHP_METHOD(Test_Pregmatch, testMatchAll) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flags, flags_sub, text, matches, _0, _1, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&flags_sub);
	ZVAL_UNDEF(&text);
	ZVAL_UNDEF(&matches);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &flags);



	ZEPHIR_INIT_VAR(&matches);
	array_init(&matches);
	ZEPHIR_INIT_VAR(&text);
	ZVAL_STRING(&text, "test1,test2");
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "/(test[0-9]+)/");
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "/(test[0-9]+)/");
	zephir_preg_match(&_1, &_2, &text, &matches, 1, zephir_get_intval(flags) , 0 );
	RETURN_CCTOR(&matches);

}

PHP_METHOD(Test_Pregmatch, testMatchAllInZep) {

	zval m1, m2, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_1 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&m1);
	ZVAL_UNDEF(&m2);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZVAL_LONG(&_0, 1);
	ZEPHIR_CALL_METHOD(&m1, this_ptr, "testmatchall", &_1, 0, &_0);
	zephir_check_call_status();
	ZVAL_LONG(&_0, 2);
	ZEPHIR_CALL_METHOD(&m2, this_ptr, "testmatchall", &_1, 0, &_0);
	zephir_check_call_status();
	zephir_create_array(return_value, 2, 0);
	zephir_array_fast_append(return_value, &m1);
	zephir_array_fast_append(return_value, &m2);
	RETURN_MM();

}

