
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
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"


/**
 *
 */
ZEPHIR_INIT_CLASS(Test_Oo_OoParams) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, OoParams, test, oo_ooparams, test_oo_ooparams_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Oo_OoParams, createThisClassWithoutWriteCurrentNamespace) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	object_init_ex(return_value, test_oo_ooparams_ce);
	if (zephir_has_constructor(return_value TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_MM();

}

PHP_METHOD(Test_Oo_OoParams, createOtherClassWithoutWriteCurrentNamespace) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	object_init_ex(return_value, test_oo_oodynamica_ce);
	if (zephir_has_constructor(return_value TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_MM();

}

PHP_METHOD(Test_Oo_OoParams, setAge) {

	zval *age_param = NULL;
	int age;

	zephir_fetch_params(0, 1, 0, &age_param);

	age = zephir_get_intval(age_param);


	RETURN_LONG(age);

}

PHP_METHOD(Test_Oo_OoParams, setAverage) {

	zval *average_param = NULL;
	double average;

	zephir_fetch_params(0, 1, 0, &average_param);

	average = zephir_get_doubleval(average_param);


	RETURN_DOUBLE(average);

}

PHP_METHOD(Test_Oo_OoParams, setName) {

	zval *name_param = NULL;
	zval *name = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name_param);

	zephir_get_strval(name, name_param);


	RETURN_CTOR(name);

}

PHP_METHOD(Test_Oo_OoParams, setEnabled) {

	zval *enabled_param = NULL;
	zend_bool enabled;

	zephir_fetch_params(0, 1, 0, &enabled_param);

	enabled = zephir_get_boolval(enabled_param);


	RETURN_BOOL(enabled);

}

PHP_METHOD(Test_Oo_OoParams, setList) {

	zval *someList_param = NULL;
	zval *someList = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &someList_param);

	zephir_get_arrval(someList, someList_param);


	RETURN_CTOR(someList);

}

PHP_METHOD(Test_Oo_OoParams, setStrictAge) {

	zval *age_param = NULL;
	int age;

	zephir_fetch_params(0, 1, 0, &age_param);

	if (unlikely(Z_TYPE_P(age_param) != IS_LONG)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'age' must be a long/integer") TSRMLS_CC);
		RETURN_NULL();
	}

	age = Z_LVAL_P(age_param);


	RETURN_LONG(age);

}

PHP_METHOD(Test_Oo_OoParams, setStrictAverage) {

	zval *average_param = NULL;
	double average;

	zephir_fetch_params(0, 1, 0, &average_param);

	if (unlikely(Z_TYPE_P(average_param) != IS_DOUBLE)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'average' must be a double") TSRMLS_CC);
		RETURN_NULL();
	}

	average = Z_DVAL_P(average_param);


	RETURN_DOUBLE(average);

}

PHP_METHOD(Test_Oo_OoParams, setStrictName) {

	zval *name_param = NULL;
	zval *name = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name_param);

	if (unlikely(Z_TYPE_P(name_param) != IS_STRING && Z_TYPE_P(name_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'name' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(name_param) == IS_STRING)) {
		zephir_get_strval(name, name_param);
	} else {
		ZEPHIR_INIT_VAR(name);
		ZVAL_EMPTY_STRING(name);
	}


	RETURN_CTOR(name);

}

PHP_METHOD(Test_Oo_OoParams, setStrictEnabled) {

	zval *enabled_param = NULL;
	zend_bool enabled;

	zephir_fetch_params(0, 1, 0, &enabled_param);

	enabled = zephir_get_boolval(enabled_param);


	RETURN_BOOL(enabled);

}

PHP_METHOD(Test_Oo_OoParams, setStrictList) {

	zval *someList_param = NULL;
	zval *someList = NULL;

	zephir_fetch_params(0, 1, 0, &someList_param);

	someList = someList_param;



	RETURN_CTORW(someList);

}

PHP_METHOD(Test_Oo_OoParams, setAgeDefault) {

	zval *age_param = NULL;
	int age;

	zephir_fetch_params(0, 0, 1, &age_param);

	if (!age_param) {
		age = 25;
	} else {
		age = zephir_get_intval(age_param);
	}


	RETURN_LONG(age);

}

PHP_METHOD(Test_Oo_OoParams, setAverageDefault) {

	zval *average_param = NULL;
	double average;

	zephir_fetch_params(0, 0, 1, &average_param);

	if (!average_param) {
		average = (double) 25;
	} else {
		average = zephir_get_doubleval(average_param);
	}


	RETURN_DOUBLE(average);

}

PHP_METHOD(Test_Oo_OoParams, setNameDefault) {

	zval *name_param = NULL;
	zval *name = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &name_param);

	if (!name_param) {
		ZEPHIR_INIT_VAR(name);
		ZVAL_STRING(name, "peter", 1);
	} else {
		zephir_get_strval(name, name_param);
	}


	RETURN_CTOR(name);

}

PHP_METHOD(Test_Oo_OoParams, setEnabledDefault) {

	zval *enabled_param = NULL;
	zend_bool enabled;

	zephir_fetch_params(0, 0, 1, &enabled_param);

	if (!enabled_param) {
		enabled = 0;
	} else {
		enabled = zephir_get_boolval(enabled_param);
	}


	RETURN_BOOL(enabled);

}

PHP_METHOD(Test_Oo_OoParams, setListDefault) {

	zval *someList_param = NULL;
	zval *someList = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &someList_param);

	if (!someList_param) {
		ZEPHIR_INIT_VAR(someList);
		array_init(someList);
	} else {
		zephir_get_arrval(someList, someList_param);
	}


	RETURN_CTOR(someList);

}

PHP_METHOD(Test_Oo_OoParams, setConstAge) {

	zval *age_param = NULL;
	int age;

	zephir_fetch_params(0, 1, 0, &age_param);

	age = zephir_get_intval(age_param);


	RETURN_LONG(age);

}

PHP_METHOD(Test_Oo_OoParams, setConstAverage) {

	zval *average_param = NULL;
	double average;

	zephir_fetch_params(0, 1, 0, &average_param);

	average = zephir_get_doubleval(average_param);


	RETURN_DOUBLE(average);

}

PHP_METHOD(Test_Oo_OoParams, setObject) {

	zval *obj;

	zephir_fetch_params(0, 1, 0, &obj);



	RETVAL_ZVAL(obj, 1, 0);
	return;

}

PHP_METHOD(Test_Oo_OoParams, setCallable) {

	zval *obj;

	zephir_fetch_params(0, 1, 0, &obj);



	RETVAL_ZVAL(obj, 1, 0);
	return;

}

PHP_METHOD(Test_Oo_OoParams, setResource) {

	zval *obj;

	zephir_fetch_params(0, 1, 0, &obj);



	RETVAL_ZVAL(obj, 1, 0);
	return;

}

PHP_METHOD(Test_Oo_OoParams, setObjectClassCast) {

	zval *parameter;

	zephir_fetch_params(0, 1, 0, &parameter);



	RETVAL_ZVAL(parameter, 1, 0);
	return;

}

