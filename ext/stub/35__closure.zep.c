
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
#include "kernel/operators.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(stub_35__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 35__closure, stub, 35__closure, stub_35__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_35__closure_ce, SL("counter"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_35__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval counter, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&counter);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_read_property(&counter, this_ptr, SL("counter"), PH_NOISY_CC | PH_READONLY);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	ZEPHIR_INIT_VAR(&_1);
	zephir_add_function(&_1, Z_REFVAL_P(&counter), &_0);
	ZEPHIR_CPY_WRT(Z_REFVAL_P(&counter), &_1);
	RETVAL_ZVAL(Z_REFVAL_P(&counter), 1, 0);
	RETURN_MM();
}

