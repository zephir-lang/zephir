
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
#include "kernel/concat.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(stub_19__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 19__closure, stub, 19__closure, stub_19__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_19__closure_ce, SL("name"), ZEND_ACC_PUBLIC);
	zend_declare_property_null(stub_19__closure_ce, SL("__$zephir_this"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_19__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval name, __$zephir_this, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&__$zephir_this);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_read_property(&__$zephir_this, this_ptr, SL("__$zephir_this"), PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&name);
	zephir_read_property(&name, this_ptr, SL("name"), PH_NOISY_CC);
	this_ptr = &__$zephir_this;

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "issue2497helper", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_CONCAT_VSV(return_value, &_0, ":", &name);
	RETURN_MM();
}

