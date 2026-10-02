
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


ZEPHIR_INIT_CLASS(stub_70__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 70__closure, stub, 70__closure, stub_70__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_70__closure_ce, SL("i"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_70__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval i;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&i);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_read_property(&i, this_ptr, SL("i"), PH_NOISY_CC | PH_READONLY);

	ZEPHIR_INIT_NVAR(Z_REFVAL_P(&i));
	ZVAL_LONG(Z_REFVAL_P(&i), 5);
	RETURN_MM_STRING("v");
}

