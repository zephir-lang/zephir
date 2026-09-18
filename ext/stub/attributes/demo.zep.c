
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
#include "kernel/memory.h"
#include "kernel/concat.h"
#include "kernel/object.h"


/**
 * Every attribute target PHP allows on a class member, so the compiled
 * extension can be checked with the same Reflection API a hand-written PHP
 * class would be (#2466).
 *
 * @see https://github.com/zephir-lang/zephir/issues/2466
 */
ZEPHIR_INIT_CLASS(Stub_Attributes_Demo)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Attributes, Demo, stub, attributes_demo, stub_attributes_demo_method_entry, 0);

	{
		zend_attribute *_za = zephir_add_class_attribute(stub_attributes_demo_ce, SL("Stub\\Attributes\\Marker"), 2);
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "class-level", sizeof("class-level") - 1);
		zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);
		zval _zc1;
		ZVAL_BOOL(&_zc1, 1);
		zephir_attribute_set_arg(_za, 1, SL("flag"), &_zc1);
	}
	{
		zend_attribute *_za = zephir_add_class_attribute(stub_attributes_demo_ce, SL("Stub\\Attributes\\Marker"), 1);
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "second", sizeof("second") - 1);
		zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);
	}
	{
		zend_attribute *_za = zephir_add_method_attribute(stub_attributes_demo_ce, SL("getvalue"), SL("Stub\\Attributes\\Marker"), 1);
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "Stub\\Attributes\\Marker", sizeof("Stub\\Attributes\\Marker") - 1);
		zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);
	}
	{
		zend_attribute *_za = zephir_add_parameter_attribute(stub_attributes_demo_ce, SL("getvalue"), 0, SL("Stub\\Attributes\\Marker"), 1);
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "first", sizeof("first") - 1);
		zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);
	}
	zephir_add_parameter_attribute(stub_attributes_demo_ce, SL("getvalue"), 1, SL("SensitiveParameter"), 0);

	{
		zval _zc0;
		ZVAL_LONG(&_zc0, 0);
		zephir_declare_typed_property(stub_attributes_demo_ce, SL("counter"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_LONG, NULL, 0);
	}

	{
		zend_attribute *_za = zephir_add_property_attribute(stub_attributes_demo_ce, SL("counter"), SL("Stub\\Attributes\\Marker"), 3);
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "on-a-property", sizeof("on-a-property") - 1);
		zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);
		zval _zc1;
		array_init_size(&_zc1, 3);
		add_next_index_long(&_zc1, 1);
		add_assoc_stringl_ex(&_zc1, SL("k"), SL("v"));
		zephir_attribute_set_arg(_za, 1, SL("extra"), &_zc1);
		zval _zc2;
		ZVAL_LONG(&_zc2, 6);
		zephir_attribute_set_arg(_za, 2, SL("number"), &_zc2);
	}

	zend_declare_property_null(stub_attributes_demo_ce, SL("legacy"), ZEND_ACC_PROTECTED);
	{
		zend_attribute *_za = zephir_add_property_attribute(stub_attributes_demo_ce, SL("legacy"), SL("Stub\\Attributes\\Marker"), 1);
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "1.0", sizeof("1.0") - 1);
		zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);
	}

	zephir_declare_class_constant_string(stub_attributes_demo_ce, SL("VERSION"), "1.0");
	{
		zend_attribute *_za = zephir_add_class_constant_attribute(stub_attributes_demo_ce, SL("VERSION"), SL("Stub\\Attributes\\Marker"), 1);
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "on-a-constant", sizeof("on-a-constant") - 1);
		zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Attributes_Demo, getValue)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval name_zv, token_zv;
	zend_string *name = NULL, *token = NULL;

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&token_zv);
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(name)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(token)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&name_zv);
	ZVAL_STR_COPY(&name_zv, name);
	if (!token) {
		token = zend_string_init(ZEND_STRL(""), 0);
		zephir_memory_observe(&token_zv);
		ZVAL_STR(&token_zv, token);
	} else {
		zephir_memory_observe(&token_zv);
	ZVAL_STR_COPY(&token_zv, token);
	}
	ZEPHIR_CONCAT_VV(return_value, &name_zv, &token_zv);
	RETURN_MM();
}

PHP_METHOD(Stub_Attributes_Demo, plain)
{

	RETURN_LONG(1);
}

