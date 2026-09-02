
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
#include "kernel/concat.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(stub_30__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 30__closure, stub, 30__closure, stub_30__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_30__closure_ce, SL("suffix"), ZEND_ACC_PUBLIC);
	zend_declare_property_null(stub_30__closure_ce, SL("__$zephir_this"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_30__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval suffix, __$zephir_this, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&suffix);
	ZVAL_UNDEF(&__$zephir_this);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("_name", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_read_property(&__$zephir_this, this_ptr, SL("__$zephir_this"), PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&suffix);
	zephir_read_property(&suffix, this_ptr, SL("suffix"), PH_NOISY_CC);
	this_ptr = &__$zephir_this;

	if (ZEPHIR_IS_STRING_IDENTICAL(&suffix, "")) {
		RETURN_MM_MEMBER(this_ptr, "_name");
	}
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 139, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_CONCAT_VV(return_value, &_0, &suffix);
	RETURN_MM();
}

