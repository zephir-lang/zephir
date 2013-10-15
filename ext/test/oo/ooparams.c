
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
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"


/**
 *
 */
ZEPHIR_INIT_CLASS(Test_Oo_OoParams) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, OoParams, test, oo_ooparams, test_oo_ooparams_method_entry, 0);


	return SUCCESS;

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

PHP_METHOD(Test_Oo_OoParams, setStrictAge) {

	zval *age_param = NULL;
	int age;

	zephir_fetch_params(0, 1, 0, &age_param);

		if (Z_TYPE_P(age_param) != IS_LONG) {
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

		if (Z_TYPE_P(average_param) != IS_DOUBLE) {
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

		zephir_get_strval(name, name_param);


	RETURN_CTOR(name);

}

PHP_METHOD(Test_Oo_OoParams, setStrictEnabled) {

	zval *enabled_param = NULL;
	zend_bool enabled;

	zephir_fetch_params(0, 1, 0, &enabled_param);

		enabled = zephir_get_boolval(enabled_param);


	RETURN_BOOL(enabled);

}

