
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

	ZVAL_UNDEF(&_0);
	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("testProperty"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

PHP_METHOD(Stub_Concat, testConcatBySelfProperty)
{
	zval *title_param = NULL, _0;
	zval title;

	ZVAL_UNDEF(&title);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(title)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &title_param);
	zephir_get_strval(&title, title_param);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("testProperty"), PH_NOISY_CC);
	SEPARATE_ZVAL(&title);
	zephir_concat_function(&title, &_0, &title);
	zephir_update_static_property_ce(stub_concat_ce, ZEND_STRL("testProperty"), &title);
}

PHP_METHOD(Stub_Concat, testConcat1)
{
	zval url, _0;

	ZVAL_UNDEF(&url);
	ZVAL_UNDEF(&_0);
	ZVAL_NULL(&url);
	ZVAL_STRING(&url, "test");
	ZVAL_NULL(&_0);
	ZEPHIR_CONCAT_SV(&_0, "append", &url);
	ZEPHIR_CPY_WRT(&url, &_0);
	RETURN_CCTOR(&url);
}

PHP_METHOD(Stub_Concat, testConcat2)
{
	zval _1;
	zval url, _0;

	ZVAL_UNDEF(&url);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_NULL(&url);
	ZVAL_STRING(&url, "test");
	ZVAL_NULL(&_0);
	ZEPHIR_CONCAT_SVS(&_0, "append", &url, "other");
	ZEPHIR_CPY_WRT(&url, &_0);
	ZVAL_NULL(&_1);
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

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_NULL(&a);
	ZVAL_STRING(&a, "1");
	ZVAL_NULL(&b);
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
	zval *value, value_sub, _0, _2;

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&query);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &value);
	ZVAL_NULL(&query);
	min =  ((zephir_safe_div_zval_long(value, 100) * (double) (25)));
	max =  ((zephir_safe_div_zval_long(value, 100) * (double) (50)));
	ZVAL_DOUBLE(&_0, max);
	ZVAL_NULL(&_1);
	ZEPHIR_CONCAT_SV(&_1, "SELECT * FROM TEST WHERE value <= ", &_0);
	zephir_concat_self(&query, &_1);
	ZVAL_DOUBLE(&_2, min);
	ZVAL_NULL(&_3);
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
	zval *number_param = NULL, _0;
	double number;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&retval);
	ZVAL_UNDEF(&left);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(number)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &number_param);
	number = zephir_get_doubleval(number_param);
	ZVAL_NULL(&left);
	ZVAL_STRING(&left, "Concatenated string with number ");
	ZVAL_DOUBLE(&_0, number);
	ZVAL_NULL(&retval);
	ZEPHIR_CONCAT_VV(&retval, &left, &_0);
	RETURN_CTOR(&retval);
}

