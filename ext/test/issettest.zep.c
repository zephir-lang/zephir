
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
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/operators.h"


/**
 * Unset statement tests
 */
ZEPHIR_INIT_CLASS(Test_IssetTest) {

	ZEPHIR_REGISTER_CLASS(Test, IssetTest, test, issettest, test_issettest_method_entry, 0);

	zend_declare_property_null(test_issettest_ce, SL("s"), ZEND_ACC_PUBLIC);

	test_issettest_ce->create_object = zephir_init_properties_Test_IssetTest;
	return SUCCESS;

}

PHP_METHOD(Test_IssetTest, testIssetArray1) {

	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);



	RETURN_BOOL(zephir_array_isset(a, b));

}

PHP_METHOD(Test_IssetTest, testIssetArray2) {

	zend_long b;
	zval *a, a_sub, *b_param = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params_without_memory_grow(2, 0, &a, &b_param);

	b = zephir_get_intval(b_param);


	RETURN_BOOL(zephir_array_isset_long(a, b));

}

PHP_METHOD(Test_IssetTest, testIssetArray3) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval b;
	zval *a, a_sub, *b_param = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b_param);

	zephir_get_strval(&b, b_param);


	RETURN_MM_BOOL(zephir_array_isset(a, &b));

}

PHP_METHOD(Test_IssetTest, testIssetArray4) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &a);



	RETURN_BOOL(zephir_array_isset_long(a, 0));

}

PHP_METHOD(Test_IssetTest, testIssetArray5) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &a);



	RETURN_BOOL(zephir_array_isset_string(a, SL("a")));

}

PHP_METHOD(Test_IssetTest, testIssetProperty1) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &a);



	RETURN_BOOL(zephir_isset_property(a, SL("b")));

}

PHP_METHOD(Test_IssetTest, testIssetProperty2) {

	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);



	RETURN_BOOL(zephir_isset_property_zval(a, b));

}

PHP_METHOD(Test_IssetTest, testIssetProperty3) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &a);



	RETURN_BOOL((0 == 0));

}

PHP_METHOD(Test_IssetTest, testIssetDynamicProperty1) {

	zval g;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&g);

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(&g);
	zephir_read_property(&g, this_ptr, SL("s"), PH_NOISY_CC);
	if (zephir_array_isset_string(&g, SL("a"))) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_IssetTest, testIssetDynamicProperty2) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *inp, inp_sub, g;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&inp_sub);
	ZVAL_UNDEF(&g);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &inp);



	ZEPHIR_OBS_VAR(&g);
	zephir_read_property(&g, inp, SL("s"), PH_NOISY_CC);
	if (zephir_array_isset_string(&g, SL("a"))) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);

}

zend_object *zephir_init_properties_Test_IssetTest(zend_class_entry *class_type TSRMLS_DC) {

		zval _1$$3;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1$$3);

		ZEPHIR_MM_GROW();
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property(&_0, this_ptr, SL("s"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			zephir_create_array(&_1$$3, 1, 0);
			add_assoc_stringl_ex(&_1$$3, SL("a"), SL("true"));
			zephir_init_property_zval(this_ptr, ZEND_STRL("s"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}

}

