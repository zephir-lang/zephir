
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


/**
 * OO operations
 */
ZEPHIR_INIT_CLASS(Test_Oo) {

	ZEPHIR_REGISTER_CLASS(Test, Oo, test, oo, test_oo_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Oo, testInstance1) {

	zval o;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&o);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init(&o);
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance2) {

	zval o;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&o);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, test_oo_ooconstruct_ce);
	ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 47);
	zephir_check_call_status();
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance3) {

	zval o;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&o);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, test_oo_oonoconstruct_ce);
	if (zephir_has_constructor(&o TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance4) {

	zval o, a, b;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "a");
	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "b");
	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, test_oo_ooconstructparams_ce);
	ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 48, &a, &b);
	zephir_check_call_status();
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance5) {

	zval o, _0, _1;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, test_oo_ooconstructparams_ce);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "a");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "b");
	ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 48, &_0, &_1);
	zephir_check_call_status();
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance6) {

	zval o, _0, _1;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, test_oo_ooconstructparams_ce);
	ZVAL_LONG(&_0, 1);
	ZVAL_LONG(&_1, 2);
	ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 48, &_0, &_1);
	zephir_check_call_status();
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance7) {

	zval o, _0, _1;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, test_oo_ooconstructparams_ce);
	ZVAL_BOOL(&_0, 0);
	ZVAL_BOOL(&_1, 1);
	ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 48, &_0, &_1);
	zephir_check_call_status();
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance8) {

	zval o, _0, _1;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, test_oo_ooconstructparams_ce);
	ZVAL_DOUBLE(&_0, 1.2);
	ZVAL_DOUBLE(&_1, 7.30);
	ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 48, &_0, &_1);
	zephir_check_call_status();
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance9) {

	zval o;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&o);

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_CE_STATIC(&o, test_oo_oodynamica_ce, "getnew", &_0, 49);
	zephir_check_call_status();
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance10) {

	zval o;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&o);

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_CE_STATIC(&o, test_oo_oodynamicb_ce, "getnew", &_0, 49);
	zephir_check_call_status();
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance11) {

	zval o, _0, _1;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&o);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&o);
	object_init_ex(&o, test_oo_ooconstructparams_ce);
	ZVAL_LONG(&_0, 1);
	ZVAL_LONG(&_1, 2);
	ZEPHIR_CALL_METHOD(NULL, &o, "__construct", NULL, 48, &_0, &_1);
	zephir_check_call_status();
	RETURN_CCTOR(o);

}

