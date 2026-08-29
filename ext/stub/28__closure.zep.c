
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


ZEPHIR_INIT_CLASS(stub_28__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 28__closure, stub, 28__closure, stub_28__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_28__closure_ce, SL("box"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_28__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval box, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&box);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("value", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&box);
	zephir_read_property(&box, this_ptr, SL("box"), PH_NOISY_CC);

	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, &box, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_0);
}

