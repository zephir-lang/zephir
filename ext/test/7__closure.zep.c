
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
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(test_7__closure) {

	ZEPHIR_REGISTER_CLASS(test, 7__closure, test, 7__closure, test_7__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	return SUCCESS;

}

PHP_METHOD(test_7__closure, __invoke) {

	zval *x, x_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&x_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &x);



	RETURN_LONG((((zephir_get_numberval(x) + 100)) + ((zephir_get_numberval(x) * 150))));

}

