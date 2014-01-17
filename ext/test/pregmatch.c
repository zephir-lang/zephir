
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


ZEPHIR_INIT_CLASS(Test_Pregmatch) {

	ZEPHIR_REGISTER_CLASS(Test, Pregmatch, test, pregmatch, test_pregmatch_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Pregmatch, testWithoutReturnAndMatches) {

	zval *pattern, *subject;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(pattern);
	ZVAL_STRING(pattern, "/def$/", 1);
	ZEPHIR_INIT_VAR(subject);
	ZVAL_STRING(subject, "abcdef", 1);
	zephir_call_func_p2_noret("preg_match", pattern, subject);
	zephir_call_func_p2(return_value, "preg_match", pattern, subject);
	RETURN_MM();

}

PHP_METHOD(Test_Pregmatch, testWithoutReturns) {

	zval *pattern, *subject, *matches;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(matches);
	ZVAL_STRING(matches, "", 1);

	ZEPHIR_INIT_VAR(pattern);
	ZVAL_STRING(pattern, "/def$/", 1);
	ZEPHIR_INIT_VAR(subject);
	ZVAL_STRING(subject, "abcdef", 1);
	Z_SET_ISREF_P(matches);
	zephir_call_func_p3_noret("preg_match", pattern, subject, matches);
	Z_UNSET_ISREF_P(matches);
	RETURN_CCTOR(matches);

}

PHP_METHOD(Test_Pregmatch, testWithoutMatches) {

	zval *pattern, *subject, *matched;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(pattern);
	ZVAL_STRING(pattern, "/def$/", 1);
	ZEPHIR_INIT_VAR(subject);
	ZVAL_STRING(subject, "abcdef", 1);
	ZEPHIR_INIT_VAR(matched);
	zephir_call_func_p2(matched, "preg_match", pattern, subject);
	RETURN_CCTOR(matched);

}

