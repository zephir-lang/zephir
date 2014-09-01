
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
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/memory.h"


/**
 * Arithmetic operations
 */
ZEPHIR_INIT_CLASS(Test_Oo_Test) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, Test, test, oo_test, test_oo_test_method_entry, 0);

	zend_class_implements(test_oo_test_ce TSRMLS_CC, 1, test_oo_testinterface_ce);
	return SUCCESS;

}

PHP_METHOD(Test_Oo_Test, setParam) {

	zval *param;

	zephir_fetch_params(0, 1, 0, &param);



	if (!(zephir_is_instance_of(param, SL("Param") TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_InvalidArgumentException, "Parameter 'param' must be an instance of 'Param'", "", 0);
		return;
	}

}

