
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"


/**
 * Call external static functions
 */
ZEPHIR_INIT_CLASS(Test_ScallExternal) {

	ZEPHIR_REGISTER_CLASS(Test, test, ScallExternal, scallexternal, test_scallexternal_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_ScallExternal, testCall1) {



}

PHP_METHOD(Test_ScallExternal, testCall2) {



}

PHP_METHOD(Test_ScallExternal, testCall3) {



}

PHP_METHOD(Test_ScallExternal, testCall4) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);




}

PHP_METHOD(Test_ScallExternal, testCall5) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);




}

PHP_METHOD(Test_ScallExternal, testCall6) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);




}

PHP_METHOD(Test_ScallExternal, testCall7) {



}

PHP_METHOD(Test_ScallExternal, testCall8) {



}

PHP_METHOD(Test_ScallExternal, testCall9) {



}

PHP_METHOD(Test_ScallExternal, testCall10) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);




}

PHP_METHOD(Test_ScallExternal, testCall11) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);




}

PHP_METHOD(Test_ScallExternal, testCall12) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);




}

