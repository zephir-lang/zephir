
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
#include "kernel/concat.h"


/**
 * Method calls
 */
ZEPHIR_INIT_CLASS(Stub_McallDynamic) {

	ZEPHIR_REGISTER_CLASS(Stub, McallDynamic, stub, mcalldynamic, stub_mcalldynamic_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_McallDynamic, testMethod1) {

	zval *this_ptr = getThis();



	RETURN_LONG(1);

}

PHP_METHOD(Stub_McallDynamic, testMagicCall1) {

	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "method1", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "method1", NULL, 0);
	zephir_check_call_status();
	zephir_add_function(return_value, &_0, &_1);
	RETURN_MM();

}

PHP_METHOD(Stub_McallDynamic, __call) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *method, method_sub, *arguments, arguments_sub, realMethod;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&method_sub);
	ZVAL_UNDEF(&arguments_sub);
	ZVAL_UNDEF(&realMethod);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(method)
		Z_PARAM_ZVAL(arguments)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &method, &arguments);



	ZEPHIR_INIT_VAR(&realMethod);
	ZEPHIR_CONCAT_SV(&realMethod, "test", method);
	ZEPHIR_RETURN_CALL_METHOD_ZVAL(this_ptr, &realMethod, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * @link https://github.com/phalcon/zephir/issues/1751
 */
PHP_METHOD(Stub_McallDynamic, testCallAnonymousFunctionWithContext) {

	zval result, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_NVAR(&_1);
	zephir_create_closure_ex(&_1, NULL, stub_12__closure_ce, SL("__invoke"));
	ZEPHIR_CALL_CE_STATIC(&result, stub_mcall_caller_ce, "start", &_0, 0, &_1);
	zephir_check_call_status();
	RETURN_CCTOR(&result);

}

