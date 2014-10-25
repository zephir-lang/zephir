
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
#include "kernel/fcall.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_Pregmatch) {

	ZEPHIR_REGISTER_CLASS(Test, Pregmatch, test, pregmatch, test_pregmatch_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Pregmatch, testWithoutReturnAndMatches) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *pattern, *subject;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(pattern);
	ZVAL_STRING(pattern, "/def$/", 1);
	ZEPHIR_INIT_VAR(subject);
	ZVAL_STRING(subject, "abcdef", 1);
	ZEPHIR_CALL_FUNCTION(NULL, "preg_match", &_0, pattern, subject);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_FUNCTION("preg_match", &_0, pattern, subject);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testWithoutReturns) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *pattern, *subject, *matches = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(matches);
	ZVAL_NULL(matches);

	ZEPHIR_INIT_VAR(pattern);
	ZVAL_STRING(pattern, "/def$/", 1);
	ZEPHIR_INIT_VAR(subject);
	ZVAL_STRING(subject, "abcdef", 1);
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(NULL, "preg_match", &_0, pattern, subject, matches);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	RETURN_CCTOR(matches);

}

PHP_METHOD(Test_Pregmatch, testWithoutMatches) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *pattern, *subject, *matched = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(pattern);
	ZVAL_STRING(pattern, "/def$/", 1);
	ZEPHIR_INIT_VAR(subject);
	ZVAL_STRING(subject, "abcdef", 1);
	ZEPHIR_CALL_FUNCTION(&matched, "preg_match", &_0, pattern, subject);
	zephir_check_call_status();
	RETURN_CCTOR(matched);

}

PHP_METHOD(Test_Pregmatch, testPregMatchAll) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *pattern, *subject, *results;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(results);
	array_init(results);

	ZEPHIR_INIT_VAR(pattern);
	ZVAL_STRING(pattern, "/def$/", 1);
	ZEPHIR_INIT_VAR(subject);
	ZVAL_STRING(subject, "abcdef", 1);
	Z_SET_ISREF_P(results);
	ZEPHIR_RETURN_CALL_FUNCTION("preg_match_all", &_0, pattern, subject, results);
	Z_UNSET_ISREF_P(results);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testPregMatchFallback) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
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
	ZEPHIR_RETURN_CALL_FUNCTION("preg_match", &_2, pattern, subject, matches, _0, _1);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testPregMatch2Params) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *pattern, *subject;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &pattern, &subject);



	ZEPHIR_RETURN_CALL_FUNCTION("preg_match", &_0, pattern, subject);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testPregMatch3Params) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *pattern, *subject, *matches;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &pattern, &subject, &matches);



	Z_SET_ISREF_P(matches);
	ZEPHIR_RETURN_CALL_FUNCTION("preg_match", &_0, pattern, subject, matches);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testPregMatch4Params) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *pattern, *subject, *matches, *flags;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 4, 0, &pattern, &subject, &matches, &flags);



	Z_SET_ISREF_P(matches);
	ZEPHIR_RETURN_CALL_FUNCTION("preg_match", &_0, pattern, subject, matches, flags);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testPregMatch5Params) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *pattern, *subject, *matches, *flags, *offset;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 5, 0, &pattern, &subject, &matches, &flags, &offset);



	Z_SET_ISREF_P(matches);
	ZEPHIR_RETURN_CALL_FUNCTION("preg_match", &_0, pattern, subject, matches, flags, offset);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * @link https://github.com/phalcon/zephir/issues/287
 */
PHP_METHOD(Test_Pregmatch, testPregMatchSaveMatches) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *str_param = NULL, *pattern_param = NULL, *matches = NULL;
	zval *str = NULL, *pattern = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &str_param, &pattern_param);

	zephir_get_strval(str, str_param);
	zephir_get_strval(pattern, pattern_param);
	ZEPHIR_INIT_VAR(matches);
	ZVAL_NULL(matches);


	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(NULL, "preg_match", &_0, pattern, str, matches);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	RETURN_CCTOR(matches);

}

