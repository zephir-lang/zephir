
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
#include "kernel/concat.h"


/**
 * `Attribute::*` flags whose values depend on the PHP that compiles the
 * extension, not on the one that ran `zephir generate` (#2738). PHP 8.5
 * renumbered `TARGET_ALL` and `IS_REPEATABLE`.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2738
 */
ZEPHIR_INIT_CLASS(Stub_Issue2738)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2738, stub, issue2738, stub_issue2738_method_entry, 0);

	{
		zend_attribute *_za = zephir_add_class_attribute(stub_issue2738_ce, SL("Attribute"), 1);
		zval _zc0;
		ZVAL_LONG(&_zc0, ZEND_ATTRIBUTE_TARGET_METHOD | ZEND_ATTRIBUTE_IS_REPEATABLE);
		zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);
	}

	zephir_declare_class_constant_long(stub_issue2738_ce, SL("TARGETS"), ZEND_ATTRIBUTE_TARGET_METHOD | ZEND_ATTRIBUTE_IS_REPEATABLE);

	{
		zval _zc0;
		array_init_size(&_zc0, 3);
		add_next_index_long(&_zc0, ZEND_ATTRIBUTE_TARGET_CLASS);
		add_assoc_long_ex(&_zc0, SL("all"), ZEND_ATTRIBUTE_TARGET_ALL);
		zephir_declare_class_constant_array(stub_issue2738_ce, SL("LIST"), &_zc0);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2738, withDefault)
{
	zval *flags_param = NULL;
	zend_long flags;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(flags)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &flags_param);
	if (!flags_param) {
		flags = ZEND_ATTRIBUTE_IS_REPEATABLE;
	} else {
		}
	RETURN_LONG(flags);
}

PHP_METHOD(Stub_Issue2738, returned)
{

	RETURN_LONG(ZEND_ATTRIBUTE_TARGET_ALL);
}

PHP_METHOD(Stub_Issue2738, combined)
{

	RETURN_LONG((ZEND_ATTRIBUTE_TARGET_ALL | ZEND_ATTRIBUTE_IS_REPEATABLE));
}

PHP_METHOD(Stub_Issue2738, described)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, ZEND_ATTRIBUTE_TARGET_ALL);
	ZEPHIR_CONCAT_SV(return_value, "flags=", &_0);
	RETURN_MM();
}

