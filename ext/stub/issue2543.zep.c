
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
#include "kernel/operators.h"
#include "kernel/memory.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/2543
 */
ZEPHIR_INIT_CLASS(Stub_Issue2543)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2543, stub, issue2543, stub_issue2543_method_entry, 0);

	/** @var int $varIntPositive */
	zend_declare_property_long(stub_issue2543_ce, SL("varIntPositive"), 10, ZEND_ACC_PUBLIC);
	/** @var string|null $label */
	zend_declare_property_null(stub_issue2543_ce, SL("label"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

/**
 */
PHP_METHOD(Stub_Issue2543, getVarIntPositive)
{

	RETURN_MEMBER_TYPED(getThis(), "varIntPositive", IS_LONG);
}

/**
 */
PHP_METHOD(Stub_Issue2543, setVarIntPositive)
{
	zval *varIntPositive_param = NULL, _0;
	zend_long varIntPositive;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("varIntPositive", 14, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(varIntPositive)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &varIntPositive_param);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, varIntPositive);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 52, &_0);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2543, getLabel)
{

	RETURN_MEMBER(getThis(), "label");
}

/**
 */
PHP_METHOD(Stub_Issue2543, setLabel)
{
	zval *label, label_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&label_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("label", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(label)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &label);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 53, label);
	RETURN_THISW();
}

