
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
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/memory.h"


/**
 * Assigning a non-constant boolean expression to a dynamic property used to
 * fail to compile with a cryptic "?" error.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/1790
 */
ZEPHIR_INIT_CLASS(Stub_Issue1790)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue1790, stub, issue1790, stub_issue1790_method_entry, 0);

	zend_declare_property_null(stub_issue1790_ce, SL("flag"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue1790, setStringProp)
{
	zval *value, value_sub, __$true, __$false;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	if (ZEPHIR_GT_LONG(value, 5)) {
		zephir_update_property_zval(this_ptr, ZEND_STRL("flag"), &__$true);
	} else {
		zephir_update_property_zval(this_ptr, ZEND_STRL("flag"), &__$false);
	}
}

PHP_METHOD(Stub_Issue1790, setVariableProp)
{
	zval *key, key_sub, *value, value_sub, __$true, __$false;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&value_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(key)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &key, &value);
	if (ZEPHIR_GT_LONG(value, 5)) {
		zephir_update_property_zval_zval(this_ptr, key, &__$true);
	} else {
		zephir_update_property_zval_zval(this_ptr, key, &__$false);
	}
}

