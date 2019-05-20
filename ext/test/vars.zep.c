
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
#include "kernel/object.h"
#include "kernel/operators.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"


ZEPHIR_INIT_CLASS(Test_Vars) {

	ZEPHIR_REGISTER_CLASS(Test, Vars, test, vars, test_vars_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Vars, testVarDump) {

	zval __$false, a, ar, _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&ar);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_ZVAL_STRING(&a, "hello");
	zephir_create_array(&ar, 3, 0);
	ZEPHIR_MM_ADD_ENTRY(&ar);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&ar, &_0);
	ZEPHIR_MM_ZVAL_STRING(&_0, "world");
	zephir_array_fast_append(&ar, &_0);
	zephir_array_fast_append(&ar, &__$false);
	zephir_var_dump(&ar TSRMLS_CC);
	zephir_var_dump(&a TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Vars, testVarDump2) {

	zval *ret, ret_sub, _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&ret_sub);
	ZVAL_UNDEF(&_0);

	zephir_fetch_params(1, 1, 0, &ret);



	ZVAL_BOOL(&_0, ZEPHIR_IS_LONG(ret, 1));
	zephir_var_dump(&_0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Vars, testVarExport) {

	zval __$false, a, ar, ret, _0, _1, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&ar);
	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);


	ZEPHIR_MM_ZVAL_STRING(&a, "hello");
	zephir_create_array(&ar, 3, 0);
	ZEPHIR_MM_ADD_ENTRY(&ar);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&ar, &_0);
	ZEPHIR_MM_ZVAL_STRING(&_0, "world");
	zephir_array_fast_append(&ar, &_0);
	zephir_array_fast_append(&ar, &__$false);
	ZEPHIR_CPY_WRT(&_1, &ar);
	zephir_var_export(&_1 TSRMLS_CC);
	zephir_var_export_ex(&ret, &ar TSRMLS_CC);
	ZEPHIR_CPY_WRT(&_2, &a);
	zephir_var_export(&_2 TSRMLS_CC);
	zephir_var_export_ex(&ret, &a TSRMLS_CC);
	RETURN_MM_CTOR(&ret);

}

PHP_METHOD(Test_Vars, test88Issue) {

	zval *param1_param = NULL, *param2_param = NULL, _0, _1, _2, _3;
	zval param1, param2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&param1);
	ZVAL_UNDEF(&param2);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);

	zephir_fetch_params(1, 1, 1, &param1_param, &param2_param);

	if (UNEXPECTED(Z_TYPE_P(param1_param) != IS_STRING && Z_TYPE_P(param1_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'param1' must be of the type string") TSRMLS_CC);
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(param1_param) == IS_STRING)) {
		zephir_get_strval(&param1, param1_param);
	} else {
		ZEPHIR_MM_ZVAL_EMPTY_STRING(&param1);
	}
	if (!param2_param) {
		ZEPHIR_MM_ZVAL_STRING(&param2, "");
	} else {
		zephir_get_strval(&param2, param2_param);
	}


	ZEPHIR_CPY_WRT(&_0, &param1);
	zephir_var_dump(&_0 TSRMLS_CC);
	ZEPHIR_CPY_WRT(&_1, &param2);
	zephir_var_dump(&_1 TSRMLS_CC);
	ZEPHIR_CPY_WRT(&_2, &param1);
	zephir_var_export(&_2 TSRMLS_CC);
	ZEPHIR_CPY_WRT(&_3, &param2);
	zephir_var_export(&_3 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Vars, test88IssueParam2InitString) {

	zval *param1_param = NULL, *param2_param = NULL, _0;
	zval param1, param2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&param1);
	ZVAL_UNDEF(&param2);
	ZVAL_UNDEF(&_0);

	zephir_fetch_params(1, 1, 1, &param1_param, &param2_param);

	if (UNEXPECTED(Z_TYPE_P(param1_param) != IS_STRING && Z_TYPE_P(param1_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'param1' must be of the type string") TSRMLS_CC);
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(param1_param) == IS_STRING)) {
		zephir_get_strval(&param1, param1_param);
	} else {
		ZEPHIR_MM_ZVAL_EMPTY_STRING(&param1);
	}
	if (!param2_param) {
		ZEPHIR_MM_ZVAL_STRING(&param2, "test string");
	} else {
		zephir_get_strval(&param2, param2_param);
	}


	ZEPHIR_CPY_WRT(&_0, &param2);
	zephir_var_export(&_0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Vars, testVarDump2param) {

	zval *p1, p1_sub, *p2, p2_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&p1_sub);
	ZVAL_UNDEF(&p2_sub);

	zephir_fetch_params(1, 2, 0, &p1, &p2);



	zephir_var_dump(p1 TSRMLS_CC);
	zephir_var_dump(p2 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Vars, testVarDump3param) {

	zval *p1, p1_sub, *p2, p2_sub, *p3, p3_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&p1_sub);
	ZVAL_UNDEF(&p2_sub);
	ZVAL_UNDEF(&p3_sub);

	zephir_fetch_params(1, 3, 0, &p1, &p2, &p3);



	zephir_var_dump(p1 TSRMLS_CC);
	zephir_var_dump(p2 TSRMLS_CC);
	zephir_var_dump(p3 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Vars, testCountOptimizerVarDumpAndExport) {

	zval *testVar, testVar_sub, _0, _1;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&testVar_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	zephir_fetch_params(1, 1, 0, &testVar);



	ZVAL_LONG(&_0, zephir_fast_count_int(testVar TSRMLS_CC));
	zephir_var_dump(&_0 TSRMLS_CC);
	ZVAL_LONG(&_1, zephir_fast_count_int(testVar TSRMLS_CC));
	zephir_var_export(&_1 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Vars, testArrayTypeVarDumpAndExport) {

	zval *testVar_param = NULL, _0, _1;
	zval testVar;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&testVar);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	zephir_fetch_params(1, 0, 1, &testVar_param);

	if (!testVar_param) {
		array_init(&testVar);
		ZEPHIR_MM_ADD_ENTRY(&testVar);
	} else {
		zephir_get_arrval(&testVar, testVar_param);
	}


	ZEPHIR_CPY_WRT(&_0, &testVar);
	zephir_var_dump(&_0 TSRMLS_CC);
	ZEPHIR_CPY_WRT(&_1, &testVar);
	zephir_var_export(&_1 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * @link https://github.com/phalcon/zephir/issues/681
 */
PHP_METHOD(Test_Vars, testIntVarDump) {

	zval _0, _1;
	zend_long a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	a = 1;
	ZVAL_LONG(&_0, a);
	zephir_var_dump(&_0 TSRMLS_CC);
	ZVAL_LONG(&_1, a);
	zephir_var_export(&_1 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Vars, testDoubleVarDump) {

	zval _0, _1;
	double a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	a = (double) (1);
	ZVAL_DOUBLE(&_0, a);
	zephir_var_dump(&_0 TSRMLS_CC);
	ZVAL_DOUBLE(&_1, a);
	zephir_var_export(&_1 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Vars, testBoolVarDump) {

	zval _0, _1;
	zend_bool a = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	a = 1;
	ZVAL_BOOL(&_0, a);
	zephir_var_dump(&_0 TSRMLS_CC);
	ZVAL_BOOL(&_1, a);
	zephir_var_export(&_1 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Vars, testGetDefinedVars) {

	double pi;
	zend_long a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	pi = 3.14;
	zephir_get_defined_vars(return_value);
	RETURN_MM();

}

