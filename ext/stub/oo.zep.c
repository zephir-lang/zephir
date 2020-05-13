
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
ZEPHIR_INIT_CLASS(Stub_Oo) {

	ZEPHIR_REGISTER_CLASS(Stub, Oo, stub, oo, stub_oo_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_Oo, testInstance1) {

	zval o;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&o);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init(&o);
	RETURN_CCTOR(&o);

}

PHP_METHOD(Stub_Oo, testInstance2) {

	zval o;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&o);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, stub_oo_ooconstruct_ce);
	ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 55);
	zephir_check_call_status();
	RETURN_CCTOR(&o);

}

PHP_METHOD(Stub_Oo, testInstance3) {

	zval o;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&o);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, stub_oo_oonoconstruct_ce);
	if (zephir_has_constructor(&o)) {
		ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_CCTOR(&o);

}

PHP_METHOD(Stub_Oo, testInstance4) {

	zval o, a, b;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "a");
	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "b");
	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, stub_oo_ooconstructparams_ce);
	ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 56, &a, &b);
	zephir_check_call_status();
	RETURN_CCTOR(&o);

}

PHP_METHOD(Stub_Oo, testInstance5) {

	zval o, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, stub_oo_ooconstructparams_ce);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "a");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "b");
	ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 56, &_0, &_1);
	zephir_check_call_status();
	RETURN_CCTOR(&o);

}

PHP_METHOD(Stub_Oo, testInstance6) {

	zval o, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, stub_oo_ooconstructparams_ce);
	ZVAL_LONG(&_0, 1);
	ZVAL_LONG(&_1, 2);
	ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 56, &_0, &_1);
	zephir_check_call_status();
	RETURN_CCTOR(&o);

}

PHP_METHOD(Stub_Oo, testInstance7) {

	zval o, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, stub_oo_ooconstructparams_ce);
	ZVAL_BOOL(&_0, 0);
	ZVAL_BOOL(&_1, 1);
	ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 56, &_0, &_1);
	zephir_check_call_status();
	RETURN_CCTOR(&o);

}

PHP_METHOD(Stub_Oo, testInstance8) {

	zval o, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, stub_oo_ooconstructparams_ce);
	ZVAL_DOUBLE(&_0, 1.2);
	ZVAL_DOUBLE(&_1, 7.30);
	ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 56, &_0, &_1);
	zephir_check_call_status();
	RETURN_CCTOR(&o);

}

PHP_METHOD(Stub_Oo, testInstance9) {

	zval o;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&o);

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_CE_STATIC(&o, stub_oo_oodynamica_ce, "getnew", &_0, 0);
	zephir_check_call_status();
	RETURN_CCTOR(&o);

}

PHP_METHOD(Stub_Oo, testInstance10) {

	zval o;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&o);

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_CE_STATIC(&o, stub_oo_oodynamicb_ce, "getnew", &_0, 0);
	zephir_check_call_status();
	RETURN_CCTOR(&o);

}

PHP_METHOD(Stub_Oo, testInstance11) {

	zval o, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, stub_oo_ooconstructparams_ce);
	ZVAL_LONG(&_0, 1);
	ZVAL_LONG(&_1, 2);
	ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 56, &_0, &_1);
	zephir_check_call_status();
	RETURN_CCTOR(&o);

}

PHP_METHOD(Stub_Oo, testInstance12) {

	zval o;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&o);

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_CE_STATIC(&o, stub_oo_oodynamica_ce, "call1", &_0, 0);
	zephir_check_call_status();
	RETURN_CCTOR(&o);

}

PHP_METHOD(Stub_Oo, createInstancesInLoop) {

	zend_class_entry *_5$$3, *_9$$4;
	zval result, _0;
	zval className, instance, fqcn, _1, *_2, _3, _4$$3, _6$$3, _7$$3, _8$$4, _10$$4, _11$$4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&className);
	ZVAL_UNDEF(&instance);
	ZVAL_UNDEF(&fqcn);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_6$$3);
	ZVAL_UNDEF(&_7$$3);
	ZVAL_UNDEF(&_8$$4);
	ZVAL_UNDEF(&_10$$4);
	ZVAL_UNDEF(&_11$$4);
	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

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
	if (Z_TYPE_P(&_0) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&_0), _2)
		{
			ZEPHIR_INIT_NVAR(&className);
			ZVAL_COPY(&className, _2);
			ZEPHIR_INIT_NVAR(&fqcn);
			ZEPHIR_CONCAT_SV(&fqcn, "Stub\\Oo\\", &className);
			ZEPHIR_INIT_NVAR(&instance);
			zephir_fetch_safe_class(&_4$$3, &fqcn);
			_5$$3 = zephir_fetch_class_str_ex(Z_STRVAL_P(&_4$$3), Z_STRLEN_P(&_4$$3), ZEND_FETCH_CLASS_AUTO);
			if(!_5$$3) {
				RETURN_MM_NULL();
			}
			object_init_ex(&instance, _5$$3);
			if (zephir_has_constructor(&instance)) {
				ZEPHIR_CALL_METHOD(NULL, &instance, "__construct", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_6$$3, &instance, "execute", NULL, 0);
			zephir_check_call_status();
			zephir_array_append(&result, &_6$$3, PH_SEPARATE, "stub/oo.zep", 108);
			ZEPHIR_CALL_METHOD(&_7$$3, &instance, "execute2", NULL, 0);
			zephir_check_call_status();
			zephir_array_append(&result, &_7$$3, PH_SEPARATE, "stub/oo.zep", 109);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &_0, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_3, &_0, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_3)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&className, &_0, "current", NULL, 0);
			zephir_check_call_status();
				ZEPHIR_INIT_NVAR(&fqcn);
				ZEPHIR_CONCAT_SV(&fqcn, "Stub\\Oo\\", &className);
				ZEPHIR_INIT_NVAR(&instance);
				zephir_fetch_safe_class(&_8$$4, &fqcn);
				_9$$4 = zephir_fetch_class_str_ex(Z_STRVAL_P(&_8$$4), Z_STRLEN_P(&_8$$4), ZEND_FETCH_CLASS_AUTO);
				if(!_9$$4) {
					RETURN_MM_NULL();
				}
				object_init_ex(&instance, _9$$4);
				if (zephir_has_constructor(&instance)) {
					ZEPHIR_CALL_METHOD(NULL, &instance, "__construct", NULL, 0);
					zephir_check_call_status();
				}
				ZEPHIR_CALL_METHOD(&_10$$4, &instance, "execute", NULL, 0);
				zephir_check_call_status();
				zephir_array_append(&result, &_10$$4, PH_SEPARATE, "stub/oo.zep", 108);
				ZEPHIR_CALL_METHOD(&_11$$4, &instance, "execute2", NULL, 0);
				zephir_check_call_status();
				zephir_array_append(&result, &_11$$4, PH_SEPARATE, "stub/oo.zep", 109);
			ZEPHIR_CALL_METHOD(NULL, &_0, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_INIT_NVAR(&className);
	RETURN_CTOR(&result);

}

