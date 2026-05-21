
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
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"


/**
 * Child class that overrides every `<static>`-returning method declared
 * in `Issue2537`. The override is what makes PHP run the arginfo
 * class-name resolution on the child during `MINIT`. See `Issue2537`
 * for the full failure-mode rationale.
 *
 * Also exercises `-> <parent>` and `-> <self>` on a child that actually
 * has a parent class.
 *
 * @see https://github.com/zephir-lang/zephir/pull/2537
 */
ZEPHIR_INIT_CLASS(Stub_Issue2537Child)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2537Child, stub, issue2537child, stub_issue2537_ce, stub_issue2537child_method_entry, 0);

	zend_declare_property_string(stub_issue2537child_ce, SL("tag"), "child", ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2537Child, make)
{
	zval *this_ptr = getThis();
	RETURN_THISW();
}

PHP_METHOD(Stub_Issue2537Child, makeNullable)
{
	zval *this_ptr = getThis();
	RETURN_THISW();
}

PHP_METHOD(Stub_Issue2537Child, makeViaParent)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	object_init_ex(return_value, stub_issue2537_ce);
	if (zephir_has_constructor(return_value)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	RETURN_MM();
}

PHP_METHOD(Stub_Issue2537Child, makeViaSelf)
{
	zval *this_ptr = getThis();
	RETURN_THISW();
}

