
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


/**
 * @link https://github.com/phalcon/zephir/issues/367
 * @link https://github.com/phalcon/zephir/issues/188
 */
ZEPHIR_INIT_CLASS(Test_Properties_StaticPropertyArray) {

	ZEPHIR_REGISTER_CLASS(Test\\Properties, StaticPropertyArray, test, properties_staticpropertyarray, NULL, 0);

	/**
	 * This is a public property with an initial empty-array value
	 */
	zend_declare_property_null(test_properties_staticpropertyarray_ce, SL("someEmptyArray"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	/**
	 * This is a public property with an initial array value
	 */
	zend_declare_property_null(test_properties_staticpropertyarray_ce, SL("someArray"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;

}

void zephir_init_static_properties_Test_Properties_StaticPropertyArray(TSRMLS_D) {

	zval _1;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 4, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 3);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 4);
	zephir_array_fast_append(&_0, &_1);
	zephir_update_static_property_ce(test_properties_staticpropertyarray_ce, ZEND_STRL("someArray"), &_0);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_ce(test_properties_staticpropertyarray_ce, ZEND_STRL("someEmptyArray"), &_1);
	ZEPHIR_MM_RESTORE();

}

