
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
#include "kernel/concat.h"
#include "kernel/operators.h"


/**
 * Issue #2063: concat-assign (.=) must work with object properties.
 */
ZEPHIR_INIT_CLASS(Stub_Issue2063)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2063, stub, issue2063, stub_issue2063_method_entry, 0);

	zend_declare_property_string(stub_issue2063_ce, SL("store"), "", ZEND_ACC_PRIVATE);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2063, add)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval value_zv, _0;
	zend_string *value = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("store"), PH_NOISY_CC);
	SEPARATE_ZVAL(&value_zv);
	zephir_concat_function(&value_zv, &_0, &value_zv);
	zephir_update_property_zval(this_ptr, ZEND_STRL("store"), &value_zv);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2063, add2)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval value_zv, _0, _1;
	zend_string *value = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("store"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_CONCAT_VV(&_1, &_0, &value_zv);
	zephir_update_property_zval(this_ptr, ZEND_STRL("store"), &_1);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2063, addLiteral)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("store"), PH_NOISY_CC);
	zephir_concat_self_str(&_0, SL("ab"));
	zephir_update_property_zval(this_ptr, ZEND_STRL("store"), &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("store"), PH_NOISY_CC);
	zephir_concat_self_str(&_0, SL("cd"));
	zephir_update_property_zval(this_ptr, ZEND_STRL("store"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2063, get)
{

	RETURN_MEMBER_TYPED(getThis(), "store", IS_STRING);
}

