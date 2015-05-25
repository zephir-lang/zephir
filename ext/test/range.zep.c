
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
#include "kernel/operators.h"


/**
 * Arithmetic operations
 */
ZEPHIR_INIT_CLASS(Test_Range) {

	ZEPHIR_REGISTER_CLASS(Test, Range, test, range, test_range_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Range, inclusive1) {

	zval *_3 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval _0, _1, *_2 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, 0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_LONG(&_1, 10);
	ZEPHIR_CALL_FUNCTION(&_2, "range", NULL, 58, &_0, &_1);
	zephir_check_call_status();
	zephir_get_arrval(_3, _2);
	RETURN_CTOR(_3);

}

PHP_METHOD(Test_Range, exclusive1) {

	zval *_3 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval _0, _1, *_2 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, 0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_LONG(&_1, 10);
	ZEPHIR_CALL_FUNCTION(&_2, "range", NULL, 58, &_0, &_1);
	zephir_check_call_status();
	zephir_get_arrval(_3, _2);
	RETURN_CTOR(_3);

}

