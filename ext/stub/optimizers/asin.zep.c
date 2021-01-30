
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "math.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/math.h"


ZEPHIR_INIT_CLASS(Stub_Optimizers_ASin) {

	ZEPHIR_REGISTER_CLASS(Stub\\Optimizers, ASin, stub, optimizers_asin, stub_optimizers_asin_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_Optimizers_ASin, testInt) {

	zval _0;
	zend_long a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	a = 1;
	ZVAL_LONG(&_0, a);
	RETURN_DOUBLE(asin(a));

}

PHP_METHOD(Stub_Optimizers_ASin, testVar) {

	zval _0;
	zend_long a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	a = 1;
	ZVAL_LONG(&_0, a);
	RETURN_DOUBLE(asin(a));

}

PHP_METHOD(Stub_Optimizers_ASin, testIntValue1) {

	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZVAL_LONG(&_0, 0);
	RETURN_DOUBLE(asin(0));

}

PHP_METHOD(Stub_Optimizers_ASin, testIntParameter) {

	zval *a_param = NULL, _0;
	zend_long a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	zephir_fetch_params_without_memory_grow(1, 0, &a_param);

	a = zephir_get_intval(a_param);


	ZVAL_LONG(&_0, a);
	RETURN_DOUBLE(asin(a));

}

PHP_METHOD(Stub_Optimizers_ASin, testVarParameter) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &a);



	RETURN_DOUBLE(zephir_asin(a));

}

