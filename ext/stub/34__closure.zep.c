
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
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(stub_34__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 34__closure, stub, 34__closure, stub_34__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_34__closure_ce, SL("n"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_34__closure, __invoke)
{
	zend_long _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval n;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_read_property(&n, this_ptr, SL("n"), PH_NOISY_CC | PH_READONLY);

	_0 = (zephir_get_numberval(&n) + 1);
	ZEPHIR_INIT_NVAR(&n);
	ZVAL_LONG(&n, _0);
	RETVAL_ZVAL(&n, 1, 0);
	RETURN_MM();
}

