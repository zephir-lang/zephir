
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
#include "kernel/operators.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Test_MethodArgs) {

	ZEPHIR_REGISTER_CLASS(Test, MethodArgs, test, methodargs, test_methodargs_method_entry, 0);

	zend_declare_property_null(test_methodargs_ce, SL("a"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_MethodArgs, setCallable) {

	zval *a, *a_param = NULL;

	zephir_fetch_params(0, 1, 0, &a_param);



	zephir_update_property_this(this_ptr, SL("a"), a TSRMLS_CC);

}

PHP_METHOD(Test_MethodArgs, setObject) {

	zval *a, *a_param = NULL;

	zephir_fetch_params(0, 1, 0, &a_param);



	zephir_update_property_this(this_ptr, SL("a"), a TSRMLS_CC);

}

