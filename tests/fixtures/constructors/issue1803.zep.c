
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/memory.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/1803
 */
ZEPHIR_INIT_CLASS(Stub_Constructors_Issue1803)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Constructors, Issue1803, stub, constructors_issue1803, stub_constructors_issue1803_method_entry, 0);

	zend_declare_property_string(stub_constructors_issue1803_ce, SL("value"), "Original value", ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Constructors_Issue1803, Issue1803)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "Value that won't be updated on class init");
	zephir_update_property_zval(this_ptr, ZEND_STRL("value"), &_0);
	ZEPHIR_MM_RESTORE();
}

