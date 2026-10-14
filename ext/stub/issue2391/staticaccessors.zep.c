
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
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/operators.h"


/**
 * Shortcuts on a static property read and write the static property.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2391
 */
ZEPHIR_INIT_CLASS(Stub_Issue2391_StaticAccessors)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Issue2391, StaticAccessors, stub, issue2391_staticaccessors, stub_issue2391_staticaccessors_method_entry, 0);

	zend_declare_property_long(stub_issue2391_staticaccessors_ce, SL("plain"), 5, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	/** @var int */
	zend_declare_property_long(stub_issue2391_staticaccessors_ce, SL("count"), 1, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	/** @var bool */
	zend_declare_property_null(stub_issue2391_staticaccessors_ce, SL("flag"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_string(stub_issue2391_staticaccessors_ce, SL("label"), "static", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2391_StaticAccessors, getPlain)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	zephir_read_static_property_ce(&_0, stub_issue2391_staticaccessors_ce, SL("plain"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

PHP_METHOD(Stub_Issue2391_StaticAccessors, setPlain)
{
	zval *plain, plain_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&plain_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(plain)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &plain);
	zephir_update_static_property_ce(stub_issue2391_staticaccessors_ce, ZEND_STRL("plain"), plain);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2391_StaticAccessors, getCount)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	zephir_read_static_property_ce(&_0, stub_issue2391_staticaccessors_ce, SL("count"), PH_NOISY_CC | PH_READONLY);
	ZVAL_COPY(return_value, &_0);
	zephir_verify_return_value(return_value);
	return;
}

/**
 */
PHP_METHOD(Stub_Issue2391_StaticAccessors, setCount)
{
	zval *count_param = NULL, _0;
	zend_long count;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(count)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &count_param);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, count);
	zephir_update_static_property_ce(stub_issue2391_staticaccessors_ce, ZEND_STRL("count"), &_0);
	RETURN_THISW();
}

/**
 */
PHP_METHOD(Stub_Issue2391_StaticAccessors, getFlag)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	zephir_read_static_property_ce(&_0, stub_issue2391_staticaccessors_ce, SL("flag"), PH_NOISY_CC | PH_READONLY);
	ZVAL_COPY(return_value, &_0);
	zephir_verify_return_value(return_value);
	return;
}

PHP_METHOD(Stub_Issue2391_StaticAccessors, __toString)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	zephir_read_static_property_ce(&_0, stub_issue2391_staticaccessors_ce, SL("label"), PH_NOISY_CC | PH_READONLY);
	ZVAL_COPY(return_value, &_0);
	zephir_verify_return_value(return_value);
	return;
}

PHP_METHOD(Stub_Issue2391_StaticAccessors, plainValue)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	zephir_read_static_property_ce(&_0, stub_issue2391_staticaccessors_ce, SL("plain"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

PHP_METHOD(Stub_Issue2391_StaticAccessors, countValue)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	zephir_read_static_property_ce(&_0, stub_issue2391_staticaccessors_ce, SL("count"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

