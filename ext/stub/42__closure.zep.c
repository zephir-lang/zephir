
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


ZEPHIR_INIT_CLASS(stub_42__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 42__closure, stub, 42__closure, stub_42__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_42__closure_ce, SL("box"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_42__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval box;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&box);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&box);
	zephir_read_property(&box, this_ptr, SL("box"), PH_NOISY_CC);

	RETVAL_ZVAL(&box, 1, 0);
	RETURN_MM();
}

