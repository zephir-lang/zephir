
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
#include "kernel/object.h"
#include "kernel/memory.h"


/**
 * Class with constructor + params
 */
ZEPHIR_INIT_CLASS(Test_Oo_OoConstructParams) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, OoConstructParams, test, oo_ooconstructparams, test_oo_ooconstructparams_method_entry, 0);

	zend_declare_property_null(test_oo_ooconstructparams_ce, SL("a"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(test_oo_ooconstructparams_ce, SL("b"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Oo_OoConstructParams, __construct) {

	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);



	zephir_update_property_zval(this_ptr, SL("a"), a);
	zephir_update_property_zval(this_ptr, SL("b"), b);

}

PHP_METHOD(Test_Oo_OoConstructParams, getA) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "a");

}

PHP_METHOD(Test_Oo_OoConstructParams, getB) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "b");

}

