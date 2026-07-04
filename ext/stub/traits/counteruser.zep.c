
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
 * Zephir class consuming a Zephir trait (zephir#504).
 */
ZEPHIR_INIT_CLASS(Stub_Traits_CounterUser)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Traits, CounterUser, stub, traits_counteruser, stub_traits_counteruser_method_entry, 0);

	zend_declare_property_long(stub_traits_counteruser_ce, SL("count"), 0, ZEND_ACC_PROTECTED);
	zephir_declare_class_constant_long(stub_traits_counteruser_ce, SL("STEP"), 2);

	return SUCCESS;
}

PHP_METHOD(Stub_Traits_CounterUser, label)
{

	RETURN_STRING("c");
}

PHP_METHOD(Stub_Traits_CounterUser, increment)
{
	zval _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("count"), PH_NOISY_CC | PH_READONLY);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, (zephir_get_numberval(&_0) + 2));
	zephir_update_property_zval(this_ptr, ZEND_STRL("count"), &_1);
	RETURN_MEMBER_TYPED(getThis(), "count", IS_LONG);
}

PHP_METHOD(Stub_Traits_CounterUser, describe)
{

	RETURN_STRING("counter");
}

PHP_METHOD(Stub_Traits_CounterUser, tagged)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "label", NULL, 0);
	zephir_check_call_status();
	zephir_read_property(&_1, this_ptr, ZEND_STRL("count"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_CALL_FUNCTION(&_2, "strval", NULL, 61, &_1);
	zephir_check_call_status();
	ZEPHIR_CONCAT_VSV(return_value, &_0, ":", &_2);
	RETURN_MM();
}

