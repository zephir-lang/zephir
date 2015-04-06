
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
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"

ZEPHIR_INIT_CLASS(Test_Properties_PropertyArray) {

	ZEPHIR_REGISTER_CLASS(Test\\Properties, PropertyArray, test, properties_propertyarray, test_properties_propertyarray_method_entry, 0);

	/**
	 * This is a public property with an initial empty-array value
	 */
	zend_declare_property_null(test_properties_propertyarray_ce, SL("someEmptyArray"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * This is a public property with an initial array value
	 */
	zend_declare_property_null(test_properties_propertyarray_ce, SL("someArray"), ZEND_ACC_PUBLIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Properties_PropertyArray, __construct) {

	zval *_1 = NULL, *_3;
	zval *_0, *_2;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 4, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 2);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 3);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 4);
	zephir_array_fast_append(_0, _1);
	zephir_update_property_this(this_ptr, SL("someArray"), _0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_1);
	array_init(_1);
	zephir_update_property_this(this_ptr, SL("someEmptyArray"), _1 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_2);
	zephir_create_array(_2, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_LONG(_3, 1);
	zephir_array_fast_append(_2, _3);
	zephir_update_property_this(this_ptr, SL("someArray"), _2 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

