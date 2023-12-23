
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

	zend_declare_property_null(stub_arraymanipulation_ce, SL("arrayFillQueue"), ZEND_ACC_PROTECTED);
	stub_arraymanipulation_ce->create_object = zephir_init_properties_Stub_ArrayManipulation;

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
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_LONG(&_0, 0);
	ZVAL_LONG(&_1, 1000000);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "message");
	ZEPHIR_CALL_FUNCTION(&_3, "array_fill", NULL, 5, &_0, &_1, &_2);
	zephir_check_call_status();
	zephir_update_property_zval(this_ptr, ZEND_STRL("arrayFillQueue"), &_3);
	RETURN_MM_MEMBER(getThis(), "arrayFillQueue");
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/774
 */
PHP_METHOD(Stub_ArrayManipulation, issue774)
{
	zval _1;
	zval obj, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&obj);
	object_init(&obj);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_update_property_zval(&obj, ZEND_STRL("prop0"), &_0);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(&obj, ZEND_STRL("prop1"), &_0);
	zephir_get_arrval(&_1, &obj);
	ZEPHIR_CPY_WRT(&obj, &_1);
	RETURN_CCTOR(&obj);
}

zend_object *zephir_init_properties_Stub_ArrayManipulation(zend_class_entry *class_type)
{
		zval _0, _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1$$3);
	

		ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
		zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("arrayFillQueue"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			array_init(&_1$$3);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("arrayFillQueue"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

