
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


ZEPHIR_INIT_CLASS(stub_41__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 41__closure, stub, 41__closure, stub_41__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_41__closure_ce, SL("items"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_41__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval items;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&items);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&items);
	zephir_read_property(&items, this_ptr, SL("items"), PH_NOISY_CC);

	RETVAL_ZVAL(&items, 1, 0);
	RETURN_MM();
}

