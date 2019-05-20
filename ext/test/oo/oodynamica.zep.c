
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
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"


/**
 * Class with dynamic new
 */
ZEPHIR_INIT_CLASS(Test_Oo_OoDynamicA) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, OoDynamicA, test, oo_oodynamica, test_oo_oodynamica_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Oo_OoDynamicA, getNew) {

	zend_class_entry *_1;
	zval className, fullClassName, _0;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&className);
	ZVAL_UNDEF(&fullClassName);
	ZVAL_UNDEF(&_0);


	zephir_get_called_class(&className TSRMLS_CC);
	ZEPHIR_CONCAT_SV(&fullClassName, "\\", &className);
	ZEPHIR_MM_ADD_ENTRY(&fullClassName);
	zephir_fetch_safe_class(&_0, &fullClassName);
	_1 = zephir_fetch_class_str_ex(Z_STRVAL_P(&_0), Z_STRLEN_P(&_0), ZEND_FETCH_CLASS_AUTO);
	object_init_ex(return_value, _1);
	if (zephir_has_constructor(return_value TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_MM();

}

PHP_METHOD(Test_Oo_OoDynamicA, call2) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	ZEPHIR_RETURN_CALL_SELF("getnew", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Oo_OoDynamicA, call1) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	ZEPHIR_RETURN_CALL_SELF("call2", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Oo_OoDynamicA, execute) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_STRING("A");

}

PHP_METHOD(Test_Oo_OoDynamicA, execute2) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_STRING("AA");

}

