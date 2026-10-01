
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
#include "kernel/array.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(stub_57__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 57__closure, stub, 57__closure, stub_57__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_57__closure_ce, SL("items"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_57__closure, __invoke)
{
	zval _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval items, _0, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&items);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_read_property(&items, this_ptr, SL("items"), PH_NOISY_CC | PH_READONLY);

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 1, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_array_fast_append(&_1, &_2);
	zephir_fast_array_merge(&_0, Z_REFVAL_P(&items), &_1);
	ZEPHIR_CPY_WRT(Z_REFVAL_P(&items), &_0);
	RETURN_MM_LONG(zephir_fast_count_int(Z_REFVAL_P(&items)));
}

