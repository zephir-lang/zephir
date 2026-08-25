
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
#include "kernel/concat.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/object.h"


/**
 * `go()` is inherited by Outer from Inner, `extra()` is declared by Outer
 * itself. Both have to resolve from an abstract class implementing Outer.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2635
 */
ZEPHIR_INIT_CLASS(Stub_Issue2635_Base)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Issue2635, Base, stub, issue2635_base, stub_issue2635_base_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_class_implements(stub_issue2635_base_ce, 1, stub_issue2635_outer_ce);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2635_Base, run)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "go", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "extra", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_CONCAT_VSV(return_value, &_0, "/", &_1);
	RETURN_MM();
}

