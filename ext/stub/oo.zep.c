
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
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/concat.h"


/**
 * OO operations
 */
ZEPHIR_INIT_CLASS(Stub_Oo)
{
	ZEPHIR_REGISTER_CLASS(Stub, Oo, stub, oo, stub_oo_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Oo, testInstance1)
{
	zval o, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init(&_0);
	ZEPHIR_CPY_WRT(&o, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	RETURN_CCTOR(&o);
}

PHP_METHOD(Stub_Oo, testInstance2)
{
	zval o, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_oo_ooconstruct_ce);
	zephir_constructor_start(&_0);
	ZEPHIR_CALL_METHOD_CACHED(NULL, &_0, "__construct", &_zephir_call_cache[0]);
	zephir_check_call_status();
	zephir_constructor_finish(&_0);
	ZEPHIR_CPY_WRT(&o, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	RETURN_CCTOR(&o);
}

PHP_METHOD(Stub_Oo, testInstance3)
{
	zval o, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_oo_oonoconstruct_ce);
	if (zephir_has_constructor(&_0)) {
		zephir_constructor_start(&_0);
		ZEPHIR_CALL_METHOD_CACHED(NULL, &_0, "__construct", &_zephir_call_cache[0]);
		zephir_check_call_status();
		zephir_constructor_finish(&_0);
	}

	ZEPHIR_CPY_WRT(&o, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	RETURN_CCTOR(&o);
}

PHP_METHOD(Stub_Oo, testInstance4)
{
	zval o, a, b, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "a");
	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "b");
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_oo_ooconstructparams_ce);
	zephir_constructor_start(&_0);
	ZEPHIR_CALL_METHOD_CACHED(NULL, &_0, "__construct", &_zephir_call_cache[0], &a, &b);
	zephir_check_call_status();
	zephir_constructor_finish(&_0);
	ZEPHIR_CPY_WRT(&o, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	RETURN_CCTOR(&o);
}

PHP_METHOD(Stub_Oo, testInstance5)
{
	zval o, _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_oo_ooconstructparams_ce);
	zephir_constructor_start(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "a");
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "b");
	ZEPHIR_CALL_METHOD_CACHED(NULL, &_0, "__construct", &_zephir_call_cache[0], &_1, &_2);
	zephir_check_call_status();
	zephir_constructor_finish(&_0);
	ZEPHIR_CPY_WRT(&o, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	RETURN_CCTOR(&o);
}

PHP_METHOD(Stub_Oo, testInstance6)
{
	zval o, _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_oo_ooconstructparams_ce);
	zephir_constructor_start(&_0);
	ZVAL_LONG(&_1, 1);
	ZVAL_LONG(&_2, 2);
	ZEPHIR_CALL_METHOD_CACHED(NULL, &_0, "__construct", &_zephir_call_cache[0], &_1, &_2);
	zephir_check_call_status();
	zephir_constructor_finish(&_0);
	ZEPHIR_CPY_WRT(&o, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	RETURN_CCTOR(&o);
}

PHP_METHOD(Stub_Oo, testInstance7)
{
	zval o, _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_oo_ooconstructparams_ce);
	zephir_constructor_start(&_0);
	ZVAL_BOOL(&_1, 0);
	ZVAL_BOOL(&_2, 1);
	ZEPHIR_CALL_METHOD_CACHED(NULL, &_0, "__construct", &_zephir_call_cache[0], &_1, &_2);
	zephir_check_call_status();
	zephir_constructor_finish(&_0);
	ZEPHIR_CPY_WRT(&o, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	RETURN_CCTOR(&o);
}

PHP_METHOD(Stub_Oo, testInstance8)
{
	zval o, _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_oo_ooconstructparams_ce);
	zephir_constructor_start(&_0);
	ZVAL_DOUBLE(&_1, 1.2);
	ZVAL_DOUBLE(&_2, 7.30);
	ZEPHIR_CALL_METHOD_CACHED(NULL, &_0, "__construct", &_zephir_call_cache[0], &_1, &_2);
	zephir_check_call_status();
	zephir_constructor_finish(&_0);
	ZEPHIR_CPY_WRT(&o, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	RETURN_CCTOR(&o);
}

PHP_METHOD(Stub_Oo, testInstance9)
{
	zval o;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&o);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_CALL_CE_STATIC_CACHED(&o, stub_oo_oodynamica_ce, "getNew", &_zephir_call_cache[0]);
	zephir_check_call_status();
	RETURN_CCTOR(&o);
}

PHP_METHOD(Stub_Oo, testInstance10)
{
	zval o;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&o);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_CALL_CE_STATIC_CACHED(&o, stub_oo_oodynamicb_ce, "getNew", &_zephir_call_cache[0]);
	zephir_check_call_status();
	RETURN_CCTOR(&o);
}

PHP_METHOD(Stub_Oo, testInstance11)
{
	zval o, _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_oo_ooconstructparams_ce);
	zephir_constructor_start(&_0);
	ZVAL_LONG(&_1, 1);
	ZVAL_LONG(&_2, 2);
	ZEPHIR_CALL_METHOD_CACHED(NULL, &_0, "__construct", &_zephir_call_cache[0], &_1, &_2);
	zephir_check_call_status();
	zephir_constructor_finish(&_0);
	ZEPHIR_CPY_WRT(&o, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	RETURN_CCTOR(&o);
}

PHP_METHOD(Stub_Oo, testInstance12)
{
	zval o;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&o);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_CALL_CE_STATIC_CACHED(&o, stub_oo_oodynamica_ce, "call1", &_zephir_call_cache[0]);
	zephir_check_call_status();
	RETURN_CCTOR(&o);
}

PHP_METHOD(Stub_Oo, createInstancesInLoop)
{
	zend_class_entry *_5$$3;
	zval result, _0;
	zval className, instance, fqcn, _1, *_2, _3$$3, _4$$3, _6$$3, _7$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&className);
	ZVAL_UNDEF(&instance);
	ZVAL_UNDEF(&fqcn);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_6$$3);
	ZVAL_UNDEF(&_7$$3);
	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_0);
	ZEND_TLS zephir_method_cache _zephir_call_cache[3];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "OoDynamicA");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "OoDynamicB");
	zephir_array_fast_append(&_0, &_1);
	zephir_is_iterable(&_0, 0, "stub/oo.zep", 112);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&_0), _2)
	{
		ZEPHIR_INIT_NVAR(&className);
		ZVAL_COPY(&className, _2);
		ZEPHIR_INIT_NVAR(&fqcn);
		ZEPHIR_CONCAT_SV(&fqcn, "Stub\\Oo\\", &className);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_INIT_NVAR(&_3$$3);
		zephir_fetch_safe_class(&_4$$3, &fqcn);
		_5$$3 = zephir_fetch_class_str_ex(Z_STRVAL_P(&_4$$3), Z_STRLEN_P(&_4$$3), ZEND_FETCH_CLASS_AUTO | ZEND_FETCH_CLASS_EXCEPTION);
		if (!_5$$3) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		object_init_ex(&_3$$3, _5$$3);
		ZEPHIR_LAST_CALL_STATUS = zephir_check_constructor_access(&_3$$3);
		zephir_check_call_status();
		if (zephir_has_constructor(&_3$$3)) {
			zephir_constructor_start(&_3$$3);
			ZEPHIR_CALL_METHOD_CACHED(NULL, &_3$$3, "__construct", &_zephir_call_cache[0]);
			zephir_check_call_status();
			zephir_constructor_finish(&_3$$3);
		}

		ZEPHIR_CPY_WRT(&instance, &_3$$3);
		ZEPHIR_INIT_NVAR(&_3$$3);
		ZEPHIR_CALL_METHOD_CACHED(&_6$$3, &instance, "execute", &_zephir_call_cache[1]);
		zephir_check_call_status();
		zephir_array_append(&result, &_6$$3, PH_SEPARATE, "stub/oo.zep", 108);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_CALL_METHOD_CACHED(&_7$$3, &instance, "execute2", &_zephir_call_cache[2]);
		zephir_check_call_status();
		zephir_array_append(&result, &_7$$3, PH_SEPARATE, "stub/oo.zep", 109);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
	} ZEND_HASH_FOREACH_END();
	RETURN_CTOR(&result);
}

