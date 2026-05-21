
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


/**
 * Regression coverage for PR #2537 / issue #2505 follow-up: the parent
 * declares a `-> <static>` method, the child (in issue2537child.zep)
 * overrides the same signature. Under the prior codegen the `<static>`
 * return type was emitted as the literal lowercase string `static`
 * inside `ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX`. PHP then ran the
 * engine's arginfo class-name resolver on the override, which has no
 * `static` class entry to point at, and aborted MINIT with:
 *
 *     Fatal error: static must be registered before Stub\Issue2537Child
 *
 * The fix emits a type-mask form (`ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX`
 * with `MAY_BE_STATIC`) for `<static>`, which the engine treats as a
 * built-in type and resolves without touching the class table.
 *
 * cphalcon hit this on its fluent-setter methods (`-> <static>`
 * overridden by component subclasses).
 *
 * @see https://github.com/zephir-lang/zephir/pull/2537
 * @see https://github.com/zephir-lang/zephir/issues/2505
 */
ZEPHIR_INIT_CLASS(Stub_Issue2537)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2537, stub, issue2537, stub_issue2537_method_entry, 0);

	zend_declare_property_string(stub_issue2537_ce, SL("tag"), "parent", ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2537, make)
{
	zval *this_ptr = getThis();
	RETURN_THISW();
}

PHP_METHOD(Stub_Issue2537, makeNullable)
{
	zval *this_ptr = getThis();
	RETURN_THISW();
}

PHP_METHOD(Stub_Issue2537, getTag)
{

	RETURN_MEMBER_TYPED(getThis(), "tag", IS_STRING);
}

