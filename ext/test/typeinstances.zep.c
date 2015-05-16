
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
#include "kernel/string.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"


/**
 * Cast tests
 */
ZEPHIR_INIT_CLASS(Test_TypeInstances) {

	ZEPHIR_REGISTER_CLASS(Test, TypeInstances, test, typeinstances, test_typeinstances_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_TypeInstances, testInstanceOfString1) {

	zval *_2 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 24);
	ZEPHIR_CALL_FUNCTION(&_1, "create_string", NULL, 0, _0);
	zephir_check_call_status();
	zephir_get_strval(_2, _1);
	RETURN_CTOR(_2);

}

PHP_METHOD(Test_TypeInstances, testInstanceOfString2) {

	zval *_2 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, -24);
	ZEPHIR_CALL_FUNCTION(&_1, "create_string", NULL, 0, _0);
	zephir_check_call_status();
	zephir_get_strval(_2, _1);
	RETURN_CTOR(_2);

}

PHP_METHOD(Test_TypeInstances, testInstanceOfString3) {

	zval *_2 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 0);
	ZEPHIR_CALL_FUNCTION(&_1, "create_string", NULL, 0, _0);
	zephir_check_call_status();
	zephir_get_strval(_2, _1);
	RETURN_CTOR(_2);

}

