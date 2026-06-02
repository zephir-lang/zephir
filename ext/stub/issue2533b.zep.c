
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
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Stub_Issue2533b)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2533b, stub, issue2533b, NULL, 0);

	zend_declare_property_null(stub_issue2533b_ce, SL("arr"), ZEND_ACC_PUBLIC);
	stub_issue2533b_ce->create_object = zephir_init_properties_Stub_Issue2533b;

	return SUCCESS;
}

zend_object *zephir_init_properties_Stub_Issue2533b(zend_class_entry *class_type)
{
		zval _1$$3;
	zval _0, _2$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_1$$3);
	

		ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
		zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("arr"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			zephir_create_array(&_1$$3, 3, 0);
			ZEPHIR_INIT_VAR(&_2$$3);
			ZVAL_LONG(&_2$$3, 1);
			zephir_array_fast_append(&_1$$3, &_2$$3);
			ZEPHIR_INIT_NVAR(&_2$$3);
			ZVAL_LONG(&_2$$3, 2);
			zephir_array_fast_append(&_1$$3, &_2$$3);
			ZEPHIR_INIT_NVAR(&_2$$3);
			ZVAL_LONG(&_2$$3, 3);
			zephir_array_fast_append(&_1$$3, &_2$$3);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("arr"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

