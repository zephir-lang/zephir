
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
#include "kernel/memory.h"
#include "kernel/array.h"


/**
 * Class with constructor + params
 */
ZEPHIR_INIT_CLASS(Test_SPropertyAccess) {

	ZEPHIR_REGISTER_CLASS(Test, SPropertyAccess, test, spropertyaccess, test_spropertyaccess_method_entry, 0);

	zend_declare_property_null(test_spropertyaccess_ce, SL("a"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(test_spropertyaccess_ce, SL("b"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_SPropertyAccess, __construct) {

	zval *_0, *_1, *_2 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "abc", 1);
	zephir_update_static_property_ce(test_spropertyaccess_ce, SL("a"), &_0 TSRMLS_CC);
	_0 = zephir_fetch_static_property_ce(test_spropertyaccess_ce, SL("a") TSRMLS_CC);
	zephir_array_fetch_long(&_1, _0, 0, PH_NOISY | PH_READONLY, "test/spropertyaccess.zep", 15 TSRMLS_CC);
	zephir_update_static_property_ce(test_spropertyaccess_ce, SL("b"), &_1 TSRMLS_CC);
	ZEPHIR_OBS_VAR(_2);
	zephir_read_static_property_ce(&_2, test_spropertyaccess_ce, SL("b") TSRMLS_CC);
	zephir_update_static_property_ce(test_scallexternal_ce, SL("sproperty"), &_2 TSRMLS_CC);
	ZEPHIR_OBS_NVAR(_2);
	zephir_read_static_property_ce(&_2, test_scallexternal_ce, SL("sproperty") TSRMLS_CC);
	zephir_update_static_property_ce(test_spropertyaccess_ce, SL("b"), &_2 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

