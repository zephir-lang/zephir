
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
#include "kernel/object.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(stub_37__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 37__closure, stub, 37__closure, stub_37__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_37__closure_ce, SL("value"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_37__closure, __invoke)
{
	zend_long _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval value;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_read_property(&value, this_ptr, SL("value"), PH_NOISY_CC | PH_READONLY);

	_0 = (zephir_get_numberval(Z_REFVAL_P(&value)) + 10);
	ZVAL_LONG(Z_REFVAL_P(&value), _0);
	ZEPHIR_MM_RESTORE();
}

