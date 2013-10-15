
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

