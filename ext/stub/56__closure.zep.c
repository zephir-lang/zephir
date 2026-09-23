
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
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(stub_56__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 56__closure, stub, 56__closure, stub_56__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_56__closure_ce, SL("s"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_56__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval s, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_read_property(&s, this_ptr, SL("s"), PH_NOISY_CC | PH_READONLY);

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_VS(&_0, Z_REFVAL_P(&s), "!");
	ZEPHIR_CPY_WRT(Z_REFVAL_P(&s), &_0);
	RETVAL_ZVAL(Z_REFVAL_P(&s), 1, 0);
	RETURN_MM();
}

