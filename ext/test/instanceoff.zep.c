
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
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"


/**
 * OO operations
 */
ZEPHIR_INIT_CLASS(Test_Instanceoff) {

	ZEPHIR_REGISTER_CLASS(Test, Instanceoff, test, instanceoff, test_instanceoff_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Instanceoff, testInstanceOf1) {

	zval a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);


	object_init(&a);ZEPHIR_MM_ADD_ENTRY(&a);
	RETURN_MM_BOOL(zephir_instance_of_ev(&a, zend_standard_class_def TSRMLS_CC));

}

PHP_METHOD(Test_Instanceoff, testInstanceOf2) {

	zval a;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);


	object_init_ex(&a, test_instanceoff_ce);ZEPHIR_MM_ADD_ENTRY(&a);
	if (zephir_has_constructor(&a TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, &a, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_MM_BOOL(zephir_instance_of_ev(&a, test_instanceoff_ce TSRMLS_CC));

}

PHP_METHOD(Test_Instanceoff, testInstanceOf3) {

	zval a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);


	object_init(&a);ZEPHIR_MM_ADD_ENTRY(&a);
	RETURN_MM_BOOL(zephir_instance_of_ev(&a, test_unknownclass_ce TSRMLS_CC));

}

PHP_METHOD(Test_Instanceoff, testInstanceOf4) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params(1, 1, 0, &a);



	if (zephir_zval_is_traversable(a TSRMLS_CC)) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_Instanceoff, testInstanceOf5) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params(1, 1, 0, &a);



	if (zephir_instance_of_ev(a, test_instanceoff_ce TSRMLS_CC)) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_Instanceoff, testInstanceOf6) {

	zval a;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);


	object_init_ex(&a, test_instanceoff_ce);ZEPHIR_MM_ADD_ENTRY(&a);
	if (zephir_has_constructor(&a TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, &a, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_MM_BOOL(zephir_instance_of_ev(&a, test_instanceoff_ce TSRMLS_CC));

}

PHP_METHOD(Test_Instanceoff, testInstanceOf7) {

	zval *test, test_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&test_sub);

	zephir_fetch_params(1, 1, 0, &test);



	RETURN_MM_BOOL(zephir_instance_of_ev(test, test_instanceoff_ce TSRMLS_CC));

}

PHP_METHOD(Test_Instanceoff, testInstanceOf8) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *test_param = NULL, a;
	zval test;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&test);
	ZVAL_UNDEF(&a);

	zephir_fetch_params(1, 1, 0, &test_param);

	zephir_get_strval(&test, test_param);


	object_init_ex(&a, test_instanceoff_ce);ZEPHIR_MM_ADD_ENTRY(&a);
	if (zephir_has_constructor(&a TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, &a, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_MM_BOOL(zephir_is_instance_of(&a, Z_STRVAL_P(&test), Z_STRLEN_P(&test) TSRMLS_CC));

}

PHP_METHOD(Test_Instanceoff, testInstanceOf9) {

	zval test;
	zval *a, a_sub, *test_param = NULL;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&test);

	zephir_fetch_params(1, 2, 0, &a, &test_param);

	zephir_get_strval(&test, test_param);


	RETURN_MM_BOOL(zephir_is_instance_of(a, Z_STRVAL_P(&test), Z_STRLEN_P(&test) TSRMLS_CC));

}

