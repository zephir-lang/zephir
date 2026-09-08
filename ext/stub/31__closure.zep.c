
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


ZEPHIR_INIT_CLASS(stub_31__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 31__closure, stub, 31__closure, stub_31__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_31__closure_ce, SL("tag"), ZEND_ACC_PUBLIC);
	zend_declare_property_null(stub_31__closure_ce, SL("__$zephir_this"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_31__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval tag, __$zephir_this;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&tag);
	ZVAL_UNDEF(&__$zephir_this);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_read_property(&__$zephir_this, this_ptr, SL("__$zephir_this"), PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&tag);
	zephir_read_property(&tag, this_ptr, SL("tag"), PH_NOISY_CC);
	this_ptr = &__$zephir_this;

	if (ZEPHIR_IS_STRING_IDENTICAL(&tag, "self")) {
		RETURN_THIS_ZVAL(this_ptr);
	}
	RETVAL_ZVAL(&tag, 1, 0);
	RETURN_MM();
}

