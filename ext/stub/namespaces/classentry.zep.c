
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
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Namespaces_ClassEntry) {

	ZEPHIR_REGISTER_CLASS(Stub\\Namespaces, ClassEntry, stub, namespaces_classentry, stub_namespaces_classentry_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_Namespaces_ClassEntry, setParam) {

	zval *param, param_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&param_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(param, stub_namespaces_a_b_sub_ce)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &param);



	RETVAL_ZVAL(param, 1, 0);
	return;

}

PHP_METHOD(Stub_Namespaces_ClassEntry, setParamImported) {

	zval *param, param_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&param_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(param, stub_namespaces_a_b_sub_ce)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &param);



	RETVAL_ZVAL(param, 1, 0);
	return;

}

