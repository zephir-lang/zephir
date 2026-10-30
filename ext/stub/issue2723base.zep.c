
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
#include "kernel/concat.h"
#include "kernel/memory.h"


/**
 * The parent class `parent::{name}()` and `Issue2723Base::{name}()` resolve
 * against in Stub\Issue2723.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2723
 */
ZEPHIR_INIT_CLASS(Stub_Issue2723Base)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2723Base, stub, issue2723base, stub_issue2723base_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2723Base, baseTarget)
{

	RETURN_STRING("base");
}

PHP_METHOD(Stub_Issue2723Base, baseWithArgument)
{
	zval *argument, argument_sub;

	ZVAL_UNDEF(&argument_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(argument)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &argument);
	ZEPHIR_CONCAT_SV(return_value, "base argument ", argument);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2723Base, baseCalledClass)
{

	zephir_get_called_class(return_value);
	return;
}

PHP_METHOD(Stub_Issue2723Base, baseInstance)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_get_class(&_0, this_ptr, 0);
	ZEPHIR_CONCAT_SV(return_value, "base instance of ", &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

