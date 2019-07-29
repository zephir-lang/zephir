
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
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Test_Globals_Env) {

	ZEPHIR_REGISTER_CLASS(Test\\Globals, Env, test, globals_env, test_globals_env_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Globals_Env, read) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *name_param = NULL, _ENV, _0;
	zval name;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&_ENV);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_get_global(&_ENV, SL("_ENV"));
	zephir_fetch_params(1, 1, 0, &name_param);

	zephir_get_strval(&name, name_param);


	zephir_array_fetch(&_0, &_ENV, &name, PH_NOISY | PH_READONLY, "test/globals/env.zep", 8 TSRMLS_CC);
	RETURN_CTOR(&_0);

}

