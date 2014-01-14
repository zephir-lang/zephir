
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
#include "kernel/array.h"
#include "kernel/variables.h"


ZEPHIR_INIT_CLASS(Test_Vars) {

	ZEPHIR_REGISTER_CLASS(Test, Vars, test, vars, test_vars_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Vars, testVarDump) {

	zval *a, *ar, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_STRING(a, "hello", 1);
	ZEPHIR_INIT_VAR(ar);
	array_init(ar);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(ar, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "world", 1);
	zephir_array_fast_append(ar, _0);
	zephir_array_fast_append(ar, ZEPHIR_GLOBAL(global_false));
	zephir_var_dump(&(ar) TSRMLS_CC);
	zephir_var_dump(&(a) TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Vars, testVarExport) {

	zval *a, *ar, *ret, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_STRING(a, "hello", 1);
	ZEPHIR_INIT_VAR(ar);
	array_init(ar);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(ar, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "world", 1);
	zephir_array_fast_append(ar, _0);
	zephir_array_fast_append(ar, ZEPHIR_GLOBAL(global_false));
	zephir_var_export(&(ar) TSRMLS_CC);
	ZEPHIR_INIT_VAR(ret);
	zephir_var_export_ex(ret, &(ar) TSRMLS_CC);
	zephir_var_export(&(a) TSRMLS_CC);
	ZEPHIR_INIT_BNVAR(ret);
	zephir_var_export_ex(ret, &(a) TSRMLS_CC);
	RETURN_CCTOR(ret);

}

