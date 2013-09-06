
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"


/**
 * OO operations
 */
ZEPHIR_INIT_CLASS(Test_Oo) {

	ZEPHIR_REGISTER_CLASS(Test, Oo, oo, test_oo_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Oo, testInstance1) {

	zval *o;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(o);
	object_init(o);
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance2) {

	zval *o;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(o);
	object_init_ex(o, test_oo_ooconstruct_ce);
	zephir_call_method_noret(o, "__construct");
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance3) {

	zval *o;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(o);
	object_init_ex(o, test_oo_oonoconstruct_ce);
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance4) {

	zval *o, *a, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_STRING(a, "a", 1);
	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "b", 1);
	ZEPHIR_INIT_VAR(o);
	object_init_ex(o, test_oo_ooconstructparams_ce);
	zephir_call_method_p2_noret(o, "__construct", a, b);
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance5) {

	zval *o, *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(o);
	object_init_ex(o, test_oo_ooconstructparams_ce);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "a", 1);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "b", 1);
	zephir_call_method_p2_noret(o, "__construct", _0, _1);
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance6) {

	zval *o, *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(o);
	object_init_ex(o, test_oo_ooconstructparams_ce);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 2);
	zephir_call_method_p2_noret(o, "__construct", _0, _1);
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance7) {

	zval *o, *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(o);
	object_init_ex(o, test_oo_ooconstructparams_ce);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_BOOL(_1, 1);
	zephir_call_method_p2_noret(o, "__construct", _0, _1);
	RETURN_CCTOR(o);

}

PHP_METHOD(Test_Oo, testInstance8) {

	zval *o, *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(o);
	object_init_ex(o, test_oo_ooconstructparams_ce);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, 1.2);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_DOUBLE(_1, 7.30);
	zephir_call_method_p2_noret(o, "__construct", _0, _1);
	RETURN_CCTOR(o);

}

