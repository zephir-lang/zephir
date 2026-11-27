
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
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Stub_ArrayManipulation)
{
	ZEPHIR_REGISTER_CLASS(Stub, ArrayManipulation, stub, arraymanipulation, stub_arraymanipulation_method_entry, 0);

	{
		zval _zc0;
		array_init_size(&_zc0, 1);
		zephir_declare_property_array(stub_arraymanipulation_ce, SL("arrayFillQueue"), &_zc0, ZEND_ACC_PROTECTED);
	}

	return SUCCESS;
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1187
 */
PHP_METHOD(Stub_ArrayManipulation, arrayFillMany)
{
	zval _0, _1, _2, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("arrayFillQueue", 14, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_LONG(&_0, 0);
	ZVAL_LONG(&_1, 1000000);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "message");
	ZEPHIR_CALL_FUNCTION(&_3, "array_fill", NULL, 5, &_0, &_1, &_2);
	zephir_check_call_status();
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 20, &_3);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER_TYPED(getThis(), "arrayFillQueue", IS_ARRAY);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/774
 */
PHP_METHOD(Stub_ArrayManipulation, issue774)
{
	zval _3;
	zval obj, _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("prop0", 5, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("prop1", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init(&_0);
	ZEPHIR_CPY_WRT(&obj, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, 0);
	zephir_update_property_zval_cached(&obj, _zephir_prop_0, 0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_zval_cached(&obj, _zephir_prop_1, 0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&_2, &obj);
	zephir_get_arrval(&_3, &_2);
	ZEPHIR_CPY_WRT(&obj, &_3);
	RETURN_CCTOR(&obj);
}

