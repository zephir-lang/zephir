
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
#include "kernel/fcall.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_Functional) {

	ZEPHIR_REGISTER_CLASS(Test, Functional, test, functional, test_functional_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Functional, map1) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, *_0 = NULL, *_1 = NULL;
	zval *a = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

	zephir_get_arrval(a, a_param);


	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_INIT_NVAR(_0);
	zephir_create_closure_ex(_0, NULL, test_8__closure_ce, SS("__invoke") TSRMLS_CC);
	ZEPHIR_CALL_FUNCTION(&_1, "array_map", NULL, 3, _0, a);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_Functional, map2) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, *b, *_0 = NULL;
	zval *a = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a_param, &b);

	zephir_get_arrval(a, a_param);


	ZEPHIR_CALL_FUNCTION(&_0, "array_map", NULL, 3, b, a);
	zephir_check_call_status();
	RETURN_CCTOR(_0);

}

