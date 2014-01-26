
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


/**
 * OO operations
 */
ZEPHIR_INIT_CLASS(Test_Instanceoff) {

	ZEPHIR_REGISTER_CLASS(Test, Instanceoff, test, instanceoff, test_instanceoff_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Instanceoff, testInstanceOf) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	object_init(a);
	RETURN_MM_BOOL(zephir_is_instance_of(a, SL("stdClass") TSRMLS_CC));

}

