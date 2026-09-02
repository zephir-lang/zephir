
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
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"


/**
 * Trait with property, constant, static and abstract members (zephir#504).
 */
ZEPHIR_INIT_CLASS(Stub_Traits_Counter)
{
	ZEPHIR_REGISTER_TRAIT(Stub\\Traits, Counter, stub, traits_counter, stub_traits_counter_method_entry);

	zend_declare_property_long(stub_traits_counter_ce, SL("count"), 0, ZEND_ACC_PROTECTED);
	{
		zval _zc0;
		array_init_size(&_zc0, 3);
		add_next_index_long(&_zc0, 10);
		add_next_index_long(&_zc0, 20);
		zephir_declare_property_array(stub_traits_counter_ce, SL("steps"), &_zc0, ZEND_ACC_PROTECTED);
	}

	zephir_declare_class_constant_long(stub_traits_counter_ce, SL("STEP"), 2);

	return SUCCESS;
}

PHP_METHOD(Stub_Traits_Counter, increment)
{
	zval _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("count", 5, 1);
	}
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 130, PH_NOISY_CC | PH_READONLY);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, (zephir_get_numberval(&_0) + 2));
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 130, &_1);
	RETURN_MEMBER_TYPED(getThis(), "count", IS_LONG);
}

PHP_METHOD(Stub_Traits_Counter, describe)
{

	RETURN_STRING("counter");
}

PHP_METHOD(Stub_Traits_Counter, label)
{
}

PHP_METHOD(Stub_Traits_Counter, tagged)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("count", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "label", NULL, 0);
	zephir_check_call_status();
	zephir_read_property_cached(&_1, this_ptr, _zephir_prop_0, 130, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_CALL_FUNCTION(&_2, "strval", NULL, 62, &_1);
	zephir_check_call_status();
	ZEPHIR_CONCAT_VSV(return_value, &_0, ":", &_2);
	RETURN_MM();
}

PHP_METHOD(Stub_Traits_Counter, getSteps)
{

	RETURN_MEMBER_TYPED(getThis(), "steps", IS_ARRAY);
}

