
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


ZEPHIR_INIT_CLASS(stub_32__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 32__closure, stub, 32__closure, stub_32__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_32__closure_ce, SL("i"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_32__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval i;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&i);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&i);
	zephir_read_property(&i, this_ptr, SL("i"), PH_NOISY_CC);

	RETVAL_ZVAL(&i, 1, 0);
	RETURN_MM();
}

