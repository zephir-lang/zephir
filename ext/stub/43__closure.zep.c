
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
#include "kernel/object.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(stub_43__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 43__closure, stub, 43__closure, stub_43__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_43__closure_ce, SL("tag"), ZEND_ACC_PUBLIC);
	zend_declare_property_null(stub_43__closure_ce, SL("__$zephir_this"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_43__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval tag, __$zephir_this, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&tag);
	ZVAL_UNDEF(&__$zephir_this);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("_name", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_read_property(&__$zephir_this, this_ptr, SL("__$zephir_this"), PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&tag);
	zephir_read_property(&tag, this_ptr, SL("tag"), PH_NOISY_CC);
	this_ptr = &__$zephir_this;

	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 151, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_CONCAT_VSV(return_value, &tag, ":", &_0);
	RETURN_MM();
}

