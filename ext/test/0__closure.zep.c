
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


ZEPHIR_INIT_CLASS(test_0__closure) {

	ZEPHIR_REGISTER_CLASS(test, 0__closure, test, 0__closure, test_0__closure_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(test_0__closure, __invoke) {


	zephir_create_closure_ex(return_value, this_ptr, test_0__closure_ce, SS("__invoke") TSRMLS_CC);
	return;

}

