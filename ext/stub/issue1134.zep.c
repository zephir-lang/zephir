
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
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/operators.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/1134
 */
ZEPHIR_INIT_CLASS(Stub_Issue1134)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue1134, stub, issue1134, stub_issue1134_method_entry, 0);

	zend_declare_property_null(stub_issue1134_ce, SL("data"), ZEND_ACC_PRIVATE);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue1134, __construct)
{
	zval *config_param = NULL;
	zval config;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&config);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY(config)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 0, 1, &config_param);
	if (!config_param) {
		ZVAL_NULL(&config);
		array_init(&config);
	} else {
		zephir_get_arrval(&config, config_param);
	}
	zephir_update_property_zval(this_ptr, ZEND_STRL("data"), &config);
}

PHP_METHOD(Stub_Issue1134, arrayWithFilledDefaultValue)
{
	zval *someDefaultData_param = NULL;
	zval someDefaultData;

	ZVAL_UNDEF(&someDefaultData);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY(someDefaultData)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 0, 1, &someDefaultData_param);
	if (!someDefaultData_param) {
		ZVAL_NULL(&someDefaultData);
		array_init(&someDefaultData);
	} else {
		zephir_get_arrval(&someDefaultData, someDefaultData_param);
	}
	RETURN_CTOR(&someDefaultData);
}

