
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
 * Expression initializers in declaration position.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2061
 */
ZEPHIR_INIT_CLASS(Stub_Issue2061)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2061, stub, issue2061, stub_issue2061_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_long(stub_issue2061_ce, SL("size"), 8192, ZEND_ACC_PUBLIC);
	zend_declare_property_long(stub_issue2061_ce, SL("mask"), 65280, ZEND_ACC_PROTECTED);
	{
		zval _zc0;
		ZVAL_LONG(&_zc0, 5);
		zephir_declare_typed_property(stub_issue2061_ce, SL("total"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_LONG, NULL, 0);
	}

	zend_declare_property_string(stub_issue2061_ce, SL("label"), "a/b", ZEND_ACC_PUBLIC);
	/** The declaration from the issue report */
	zephir_declare_class_constant_long(stub_issue2061_ce, SL("INT8_MIN"), -128);

	zephir_declare_class_constant_long(stub_issue2061_ce, SL("INT8_MAX"), 0x7f);

	zephir_declare_class_constant_long(stub_issue2061_ce, SL("INT16_MIN"), -32768);

	zephir_declare_class_constant_long(stub_issue2061_ce, SL("INT16_MAX"), 0x7fff);

	zephir_declare_class_constant_long(stub_issue2061_ce, SL("KB"), 1024);

	zephir_declare_class_constant_long(stub_issue2061_ce, SL("MB"), 1048576);

	zephir_declare_class_constant_long(stub_issue2061_ce, SL("MASK"), 65295);

	zephir_declare_class_constant_long(stub_issue2061_ce, SL("INVERTED"), -1);

	zephir_declare_class_constant_double(stub_issue2061_ce, SL("RATIO"), 2.5);

	zephir_declare_class_constant_long(stub_issue2061_ce, SL("REMAINDER"), 1);

	zephir_declare_class_constant_string(stub_issue2061_ce, SL("NAME"), "issue2061");

	zephir_declare_class_constant_string(stub_issue2061_ce, SL("PATH"), "issue2061/limits");

	zephir_declare_class_constant_long(stub_issue2061_ce, SL("DOUBLED"), 2048);

	zephir_declare_class_constant_long(stub_issue2061_ce, SL("FROM_ENV"), 64);

	zephir_declare_class_constant_bool(stub_issue2061_ce, SL("FLAG"), 1);

	zephir_declare_class_constant_bool(stub_issue2061_ce, SL("NEGATED"), 1);

	zephir_declare_class_constant_long(stub_issue2061_ce, SL("WORD"), 64);

	zephir_declare_class_constant_long(stub_issue2061_ce, SL("FALLBACK"), 42);

	{
		zval _zc0;
		array_init_size(&_zc0, 4);
		add_next_index_long(&_zc0, 2);
		add_next_index_long(&_zc0, 4);
		add_assoc_long_ex(&_zc0, SL("sum"), 7);
		zephir_declare_class_constant_array(stub_issue2061_ce, SL("NUMBERS"), &_zc0);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2061, getSize)
{

	RETURN_MEMBER(getThis(), "size");
}

PHP_METHOD(Stub_Issue2061, getMask)
{

	RETURN_MEMBER(getThis(), "mask");
}

PHP_METHOD(Stub_Issue2061, getTotal)
{

	RETURN_MEMBER(getThis(), "total");
}

PHP_METHOD(Stub_Issue2061, getLabel)
{

	RETURN_MEMBER(getThis(), "label");
}

/**
 * A parameter default still only accepts a literal or `self::CONST`, but the
 * constant it names may now hold an expression, so arg-info has to receive
 * the folded value.
 */
PHP_METHOD(Stub_Issue2061, withDefault)
{
	zval *size_param = NULL;
	zend_long size;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(size)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &size_param);
	if (!size_param) {
		size = 2048;
	} else {
		}
	RETURN_LONG(size);
}

