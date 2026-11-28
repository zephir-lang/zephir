
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
#include "kernel/memory.h"
#include "kernel/object.h"


/**
 * A string that spans lines in a declaration keeps its line breaks, as a
 * string in a method body does. Array defaults are declared on the class
 * since #2799, which made the template of a class like this reach C.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2799
 */
ZEPHIR_INIT_CLASS(Stub_Issue2799Multiline)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2799Multiline, stub, issue2799multiline, stub_issue2799multiline_method_entry, 0);

	{
		zval _zc0;
		array_init_size(&_zc0, 3);
		add_assoc_stringl_ex(&_zc0, SL("main"), SL("\n<nav>\n    <ol>\n%items%\n    </ol>\n</nav>"));
		add_assoc_long_ex(&_zc0, SL("k1\nk2"), 1);
		zephir_declare_property_array(stub_issue2799multiline_ce, SL("template"), &_zc0, ZEND_ACC_PUBLIC);
	}

	zend_declare_property_string(stub_issue2799multiline_ce, SL("text"), "t1\nt2", ZEND_ACC_PUBLIC);
	{
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "y1\ny2", sizeof("y1\ny2") - 1);
		zephir_declare_typed_property(stub_issue2799multiline_ce, SL("typedText"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_STRING, NULL, 0);
	}

	zend_declare_property_string(stub_issue2799multiline_ce, SL("shared"), "s1\ns2", ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	zephir_declare_class_constant_string(stub_issue2799multiline_ce, SL("TEXT"), "c1\nc2");

	{
		zval _zc0;
		array_init_size(&_zc0, 3);
		add_next_index_stringl(&_zc0, SL("l1\nl2"));
		add_assoc_stringl_ex(&_zc0, SL("k1\nk2"), SL("v1\nv2"));
		zephir_declare_class_constant_array(stub_issue2799multiline_ce, SL("LINES"), &_zc0);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2799Multiline, withDefault)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval x_zv;
	zend_string *x = NULL;

	ZVAL_UNDEF(&x_zv);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(x)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	if (!x) {
		x = zend_string_init(ZEND_STRL("p1\np2"), 0);
		zephir_memory_observe(&x_zv);
		ZVAL_STR(&x_zv, x);
	} else {
		zephir_memory_observe(&x_zv);
	ZVAL_STR_COPY(&x_zv, x);
	}
	RETURN_MM_STR(zend_string_copy(x));
}

