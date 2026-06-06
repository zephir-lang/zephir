
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
#include "kernel/memory.h"
#include "kernel/array.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/2409
 */
ZEPHIR_INIT_CLASS(Stub_Issue2409)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2409, stub, issue2409, stub_issue2409_method_entry, 0);

	zend_declare_property_null(stub_issue2409_ce, SL("someVar"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2409, setEmpty)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_UNDEF(&_0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_EMPTY_STRING(&_0);
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2409, setNonEmpty)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_UNDEF(&_0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "123");
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2409, setNull)
{
	zval __$null;

	ZVAL_NULL(&__$null);
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &__$null);
}

PHP_METHOD(Stub_Issue2409, setInt)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 42);
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &_0);
}

PHP_METHOD(Stub_Issue2409, setNegativeInt)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, -7);
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &_0);
}

PHP_METHOD(Stub_Issue2409, setDouble)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_0);
	ZVAL_DOUBLE(&_0, 3.14);
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &_0);
}

PHP_METHOD(Stub_Issue2409, setBoolTrue)
{
	zval __$true;

	ZVAL_BOOL(&__$true, 1);
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &__$true);
}

PHP_METHOD(Stub_Issue2409, setBoolFalse)
{
	zval __$false;

	ZVAL_BOOL(&__$false, 0);
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &__$false);
}

PHP_METHOD(Stub_Issue2409, setChar)
{
	zval _0;
	char ch;

	ZVAL_UNDEF(&_0);
	ch = 'A';
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, ch);
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &_0);
}

PHP_METHOD(Stub_Issue2409, setEmptyArray)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2409, setArray)
{
	zval _1;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 3);
	zephir_array_fast_append(&_0, &_1);
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2409, setFromStringVariable)
{
	zval tmp;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&tmp);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&tmp);
	ZVAL_STRING(&tmp, "hello");
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &tmp);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2409, setFromIntVariable)
{
	zval _0;
	zend_long tmp;

	ZVAL_UNDEF(&_0);
	tmp = 99;
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, tmp);
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &_0);
}

PHP_METHOD(Stub_Issue2409, setFromDoubleVariable)
{
	zval _0;
	double tmp;

	ZVAL_UNDEF(&_0);
	tmp =  (2.5);
	ZVAL_UNDEF(&_0);
	ZVAL_DOUBLE(&_0, tmp);
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &_0);
}

PHP_METHOD(Stub_Issue2409, setFromBoolVariable)
{
	zval _0;
	zend_bool tmp;

	ZVAL_UNDEF(&_0);
	tmp = 1;
	ZVAL_UNDEF(&_0);
	ZVAL_BOOL(&_0, tmp);
	zephir_update_static_property_ce(stub_issue2409_ce, ZEND_STRL("someVar"), &_0);
}

PHP_METHOD(Stub_Issue2409, getVar)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	zephir_read_static_property_ce(&_0, stub_issue2409_ce, SL("someVar"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

