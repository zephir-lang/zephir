
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


ZEPHIR_INIT_CLASS(stub_53__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 53__closure, stub, 53__closure, stub_53__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_53__closure_ce, SL("seed"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_53__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval seed;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&seed);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_read_property(&seed, this_ptr, SL("seed"), PH_NOISY_CC | PH_READONLY);

	ZEPHIR_INIT_NVAR(Z_REFVAL_P(&seed));
	ZVAL_LONG(Z_REFVAL_P(&seed), 999);
	ZEPHIR_MM_RESTORE();
}

