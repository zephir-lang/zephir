
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
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/array.h"


/**
 * Control Flow
 */
/**
 * @link https://ru2.php.net/empty
 */
ZEPHIR_INIT_CLASS(Stub_EmptyTest)
{
	ZEPHIR_REGISTER_CLASS(Stub, EmptyTest, stub, emptytest, stub_emptytest_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_EmptyTest, testDynamicVarArrayEmpty)
{
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_NULL(&a);
	array_init(&a);
	RETURN_MM_BOOL(ZEPHIR_IS_EMPTY(&a));
}

PHP_METHOD(Stub_EmptyTest, testDynamicVarArrayNotEmpty)
{
	zval a, _0;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_NULL(&a);
	zephir_create_array(&a, 4, 0);
	ZVAL_NULL(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 4);
	zephir_array_fast_append(&a, &_0);
	RETURN_MM_BOOL(ZEPHIR_IS_EMPTY(&a));
}

PHP_METHOD(Stub_EmptyTest, testEmptyString)
{
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_NULL(&a);
	ZVAL_STRING(&a, "");
	RETURN_MM_BOOL(ZEPHIR_IS_EMPTY(&a));
}

PHP_METHOD(Stub_EmptyTest, testNotEmptyString)
{
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_NULL(&a);
	ZVAL_STRING(&a, "test string");
	RETURN_MM_BOOL(ZEPHIR_IS_EMPTY(&a));
}

PHP_METHOD(Stub_EmptyTest, testString)
{
	zval *a_param = NULL;
	zval a;

	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_strval(&a, a_param);
	RETURN_MM_BOOL(ZEPHIR_IS_EMPTY(&a));
}

