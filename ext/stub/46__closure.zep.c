
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


ZEPHIR_INIT_CLASS(stub_46__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 46__closure, stub, 46__closure, stub_46__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_46__closure_ce, SL("x"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_46__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval x;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&x);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&x);
	zephir_read_property(&x, this_ptr, SL("x"), PH_NOISY_CC);

	zephir_gettype(return_value, &x);
	RETURN_MM();
}

