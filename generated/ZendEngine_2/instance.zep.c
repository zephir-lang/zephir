
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
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/object.h"


/**
 * Instance operations
 */
ZEPHIR_INIT_CLASS(Test_Instance) {

	ZEPHIR_REGISTER_CLASS(Test, Instance, test, instance, test_instance_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Instance, __construct) {

	zval *a1, *a2, *a3, *a4, *a5, *a6, *a7, *a8, *a9, *a10, *a11;

	zephir_fetch_params(0, 11, 0, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9, &a10, &a11);




}

PHP_METHOD(Test_Instance, testIssue1339) {

	zval *parameters = NULL, *_0 = NULL, _1;
	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(parameters);
	zephir_create_array(parameters, 11, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	object_init_ex(_0, test_arithmetic_ce);
	if (zephir_has_constructor(_0 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	zephir_array_fast_append(parameters, _0);
	ZEPHIR_INIT_NVAR(_0);
	object_init_ex(_0, test_arrayobject_ce);
	ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 38);
	zephir_check_call_status();
	zephir_array_fast_append(parameters, _0);
	ZEPHIR_INIT_NVAR(_0);
	object_init_ex(_0, test_assign_ce);
	if (zephir_has_constructor(_0 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	zephir_array_fast_append(parameters, _0);
	ZEPHIR_INIT_NVAR(_0);
	object_init_ex(_0, test_bitwise_ce);
	if (zephir_has_constructor(_0 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	zephir_array_fast_append(parameters, _0);
	ZEPHIR_INIT_NVAR(_0);
	object_init_ex(_0, test_branchprediction_ce);
	if (zephir_has_constructor(_0 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	zephir_array_fast_append(parameters, _0);
	ZEPHIR_INIT_NVAR(_0);
	object_init_ex(_0, test_cast_ce);
	if (zephir_has_constructor(_0 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	zephir_array_fast_append(parameters, _0);
	ZEPHIR_INIT_NVAR(_0);
	object_init_ex(_0, test_cblock_ce);
	if (zephir_has_constructor(_0 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	zephir_array_fast_append(parameters, _0);
	ZEPHIR_INIT_NVAR(_0);
	object_init_ex(_0, test_chars_ce);
	if (zephir_has_constructor(_0 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	zephir_array_fast_append(parameters, _0);
	ZEPHIR_INIT_NVAR(_0);
	object_init_ex(_0, test_closures_ce);
	if (zephir_has_constructor(_0 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	zephir_array_fast_append(parameters, _0);
	ZEPHIR_INIT_NVAR(_0);
	object_init_ex(_0, test_compare_ce);
	if (zephir_has_constructor(_0 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	zephir_array_fast_append(parameters, _0);
	ZEPHIR_INIT_NVAR(_0);
	object_init_ex(_0, test_concat_ce);
	if (zephir_has_constructor(_0 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	zephir_array_fast_append(parameters, _0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_STRING(&_1, "Test\\Instance", 0);
	ZEPHIR_LAST_CALL_STATUS = zephir_create_instance_params(return_value, &_1, parameters TSRMLS_CC);
	zephir_check_call_status();
	RETURN_MM();

}

