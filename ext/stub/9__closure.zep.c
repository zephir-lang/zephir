
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


ZEPHIR_INIT_CLASS(stub_9__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 9__closure, stub, 9__closure, stub_9__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_9__closure_ce, SL("abc"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_9__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval abc, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&abc);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&abc);
	zephir_read_property(&abc, this_ptr, SL("abc"), PH_NOISY_CC);

	zephir_memory_observe(&_0);
	zephir_array_fetch_string(&_0, &abc, SL("a"), PH_NOISY, "stub/closures.zep", 68);
	RETURN_CCTOR(&_0);
}

