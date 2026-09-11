
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


/**
 * Attributes on trait members. Zephir merges traits at compile time, so the
 * using class re-instantiates these members from the same raw AST nodes and
 * therefore carries their attributes too, which is what PHP does natively
 * (#2466).
 */
ZEPHIR_INIT_CLASS(Stub_Attributes_Counter)
{
	ZEPHIR_REGISTER_TRAIT(Stub\\Attributes, Counter, stub, attributes_counter, stub_attributes_counter_method_entry);

	{
		zend_attribute *_za = zephir_add_class_attribute(stub_attributes_counter_ce, SL("Stub\\Attributes\\Marker"), 1);
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "on-a-trait", sizeof("on-a-trait") - 1);
		zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);
	}
	{
		zend_attribute *_za = zephir_add_method_attribute(stub_attributes_counter_ce, SL("bump"), SL("Stub\\Attributes\\Marker"), 1);
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "trait-method", sizeof("trait-method") - 1);
		zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);
	}

	{
		zval _zc0;
		ZVAL_LONG(&_zc0, 0);
		zephir_declare_typed_property(stub_attributes_counter_ce, SL("hits"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_LONG, NULL, 0);
	}

	{
		zend_attribute *_za = zephir_add_property_attribute(stub_attributes_counter_ce, SL("hits"), SL("Stub\\Attributes\\Marker"), 1);
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "trait-property", sizeof("trait-property") - 1);
		zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Attributes_Counter, bump)
{
	zval _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("hits", 4, 1);
	}
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 20, PH_NOISY_CC | PH_READONLY);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, (zephir_get_numberval(&_0) + 1));
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 20, &_1);
	RETURN_MEMBER_TYPED(getThis(), "hits", IS_LONG);
}

