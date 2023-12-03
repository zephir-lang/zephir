
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
#include "kernel/operators.h"
#include "kernel/concat.h"


ZEPHIR_INIT_CLASS(Stub_Concat)
{
	ZEPHIR_REGISTER_CLASS(Stub, Concat, stub, concat, stub_concat_method_entry, 0);

	zend_declare_property_null(stub_concat_ce, SL("testProperty"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Concat, getTestProperty)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);



	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("testProperty"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

PHP_METHOD(Stub_Concat, testConcatBySelfProperty)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *title_param = NULL, _0;
	zval title;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&title);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(title)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &title_param);
	zephir_get_strval(&title, title_param);


	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("testProperty"), PH_NOISY_CC);
	SEPARATE_ZVAL(&title);
	zephir_concat_function(&title, &_0, &title);
	zephir_update_static_property_ce(stub_concat_ce, ZEND_STRL("testProperty"), &title);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Concat, testConcat1)
{
	zval url, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&url);
	ZVAL_UNDEF(&_0);


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&url);
	ZVAL_STRING(&url, "test");
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_SV(&_0, "append", &url);
	ZEPHIR_CPY_WRT(&url, &_0);
	RETURN_CCTOR(&url);
}

PHP_METHOD(Stub_Concat, testConcat2)
{
	zval _1;
	zval url, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&url);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&url);
	ZVAL_STRING(&url, "test");
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_SVS(&_0, "append", &url, "other");
	ZEPHIR_CPY_WRT(&url, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_CONCAT_SS(&_1, "append", "other");
	ZEPHIR_CPY_WRT(&url, &_1);
	ZEPHIR_INIT_NVAR(&_0);
	ZEPHIR_CONCAT_SSV(&_0, "append", "other", &url);
	ZEPHIR_CPY_WRT(&url, &_0);
	RETURN_CCTOR(&url);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/1573
 */
PHP_METHOD(Stub_Concat, testConcat3)
{
	zval a, b;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "1");
	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "2");
	zephir_concat_self(&b, &a);
	RETURN_CCTOR(&b);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/1893
 */
PHP_METHOD(Stub_Concat, testConcat4)
{
	zval query, _1, _3;
	double min = 0, max = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value, value_sub, _0, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&query);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &value);


	ZEPHIR_INIT_VAR(&query);
	min = (zephir_safe_div_zval_long(value, 100) * (double) (25));
	max = (zephir_safe_div_zval_long(value, 100) * (double) (50));
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, max);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_CONCAT_SV(&_1, "SELECT * FROM TEST WHERE value <= ", &_0);
	zephir_concat_self(&query, &_1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_DOUBLE(&_2, min);
	ZEPHIR_INIT_VAR(&_3);
	ZEPHIR_CONCAT_SV(&_3, " AND value >= ", &_2);
	zephir_concat_self(&query, &_3);
	RETURN_CTOR(&query);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/1893
 */
PHP_METHOD(Stub_Concat, testConcat5)
{
	zval retval, left;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *number_param = NULL, _0;
	double number;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&retval);
	ZVAL_UNDEF(&left);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(number)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &number_param);
	number = zephir_get_doubleval(number_param);


	ZEPHIR_INIT_VAR(&left);
	ZVAL_STRING(&left, "Concatenated string with number ");
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, number);
	ZEPHIR_INIT_VAR(&retval);
	ZEPHIR_CONCAT_VV(&retval, &left, &_0);
	RETURN_CTOR(&retval);
}

