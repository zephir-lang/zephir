
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
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Test_Pregmatch) {

	ZEPHIR_REGISTER_CLASS(Test, Pregmatch, test, pregmatch, test_pregmatch_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Pregmatch, testWithoutReturnAndMatches) {

	zval *pattern, *subject, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(pattern);
	ZVAL_STRING(pattern, "/def$/", 1);
	ZEPHIR_INIT_VAR(subject);
	ZVAL_STRING(subject, "abcdef", 1);
	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_INIT_VAR(_1);
	zephir_preg_match(_1, pattern, subject, _0, 0, 0 , 0  TSRMLS_CC);
	ZEPHIR_INIT_VAR(_2);
	zephir_preg_match(return_value, pattern, subject, _2, 0, 0 , 0  TSRMLS_CC);
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testWithoutReturns) {

	zval *pattern, *subject, *matches = NULL, *_0;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(matches);
	ZVAL_NULL(matches);

	ZEPHIR_INIT_VAR(pattern);
	ZVAL_STRING(pattern, "/def$/", 1);
	ZEPHIR_INIT_VAR(subject);
	ZVAL_STRING(subject, "abcdef", 1);
	ZEPHIR_INIT_VAR(_0);
	zephir_preg_match(_0, pattern, subject, matches, 0, 0 , 0  TSRMLS_CC);
	RETURN_CCTOR(matches);

}

PHP_METHOD(Test_Pregmatch, testWithoutMatches) {

	zval *pattern, *subject, *matched, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(pattern);
	ZVAL_STRING(pattern, "/def$/", 1);
	ZEPHIR_INIT_VAR(subject);
	ZVAL_STRING(subject, "abcdef", 1);
	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_INIT_VAR(matched);
	zephir_preg_match(matched, pattern, subject, _0, 0, 0 , 0  TSRMLS_CC);
	RETURN_CCTOR(matched);

}

PHP_METHOD(Test_Pregmatch, testPregMatchAll) {

	zval *pattern, *subject, *results;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(results);
	array_init(results);

	ZEPHIR_INIT_VAR(pattern);
	ZVAL_STRING(pattern, "/def$/", 1);
	ZEPHIR_INIT_VAR(subject);
	ZVAL_STRING(subject, "abcdef", 1);
	zephir_preg_match(return_value, pattern, subject, results, 1, 0 , 0  TSRMLS_CC);
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testPregMatchFallback) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, *subject, *matches = NULL, *_0, *_1;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(matches);
	ZVAL_NULL(matches);

	ZEPHIR_INIT_NVAR(matches);
	array_init(matches);
	ZEPHIR_INIT_VAR(pattern);
	ZVAL_STRING(pattern, "/def$/", 1);
	ZEPHIR_INIT_VAR(subject);
	ZVAL_STRING(subject, "abcdef", 1);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 0);
	Z_SET_ISREF_P(matches);
	ZEPHIR_RETURN_CALL_FUNCTION("preg_match", NULL, 55, pattern, subject, matches, _0, _1);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testPregMatch2Params) {

	zval *pattern, *subject, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &pattern, &subject);



	ZEPHIR_INIT_VAR(_0);
	zephir_preg_match(return_value, pattern, subject, _0, 0, 0 , 0  TSRMLS_CC);
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testPregMatch3Params) {

	zval *pattern, *subject, *matches;

	zephir_fetch_params(0, 3, 0, &pattern, &subject, &matches);



	zephir_preg_match(return_value, pattern, subject, matches, 0, 0 , 0  TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Pregmatch, testPregMatch4Params) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, *subject, *matches, *flags;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 4, 0, &pattern, &subject, &matches, &flags);



	Z_SET_ISREF_P(matches);
	ZEPHIR_RETURN_CALL_FUNCTION("preg_match", NULL, 55, pattern, subject, matches, flags);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testPregMatch5Params) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *pattern, *subject, *matches, *flags, *offset;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 5, 0, &pattern, &subject, &matches, &flags, &offset);



	Z_SET_ISREF_P(matches);
	ZEPHIR_RETURN_CALL_FUNCTION("preg_match", NULL, 55, pattern, subject, matches, flags, offset);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * @link https://github.com/phalcon/zephir/issues/287
 */
PHP_METHOD(Test_Pregmatch, testPregMatchSaveMatches) {

	zval *str_param = NULL, *pattern_param = NULL, *matches = NULL, *_0;
	zval *str = NULL, *pattern = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &str_param, &pattern_param);

	zephir_get_strval(str, str_param);
	zephir_get_strval(pattern, pattern_param);
	ZEPHIR_INIT_VAR(matches);
	ZVAL_NULL(matches);


	ZEPHIR_INIT_VAR(_0);
	zephir_preg_match(_0, pattern, str, matches, 0, 0 , 0  TSRMLS_CC);
	RETURN_CCTOR(matches);

}

PHP_METHOD(Test_Pregmatch, testMatchAll) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *flags, *text, *matches, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &flags);

	ZEPHIR_INIT_VAR(matches);
	array_init(matches);


	ZEPHIR_INIT_VAR(text);
	ZVAL_STRING(text, "test1,test2", 1);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "/(test[0-9]+)/", ZEPHIR_TEMP_PARAM_COPY);
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(NULL, "preg_match_all", NULL, 56, _0, text, matches, flags);
	zephir_check_temp_parameter(_0);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	RETURN_CCTOR(matches);

}

PHP_METHOD(Test_Pregmatch, testMatchAllInZep) {

	zephir_fcall_cache_entry *_1 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *m1 = NULL, *m2 = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	ZEPHIR_CALL_METHOD(&m1, this_ptr, "testmatchall", &_1, 0, _0);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	ZEPHIR_CALL_METHOD(&m2, this_ptr, "testmatchall", &_1, 0, _0);
	zephir_check_call_status();
	zephir_create_array(return_value, 2, 0 TSRMLS_CC);
	zephir_array_fast_append(return_value, m1);
	zephir_array_fast_append(return_value, m2);
	RETURN_MM();

}

