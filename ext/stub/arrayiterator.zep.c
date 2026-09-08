
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
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_ArrayIterator)
{
	ZEPHIR_REGISTER_CLASS(Stub, ArrayIterator, stub, arrayiterator, stub_arrayiterator_method_entry, 0);

	zend_declare_property_long(stub_arrayiterator_ce, SL("position"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_arrayiterator_ce, SL("test"), ZEND_ACC_PROTECTED);
	zend_class_implements(stub_arrayiterator_ce, 1, zend_ce_iterator);
	return SUCCESS;
}

PHP_METHOD(Stub_ArrayIterator, __construct)
{
	zval _1, _2;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("test", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("position", 8, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "one");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "two");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "three");
	zephir_array_fast_append(&_0, &_1);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 15, &_0);
	ZVAL_UNDEF(&_2);
	ZVAL_LONG(&_2, 0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_1, 16, &_2);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_ArrayIterator, rewind)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("position", 8, 1);
	}
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 16, &_0);
}

PHP_METHOD(Stub_ArrayIterator, current)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("test", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("position", 8, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 15, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&_1);
	zephir_memory_observe(&_2);
	zephir_read_property_cached(&_2, this_ptr, _zephir_prop_1, 16, PH_NOISY_CC);
	zephir_array_fetch(&_1, &_0, &_2, PH_NOISY, "stub/arrayiterator.zep", 25);
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_ArrayIterator, key)
{

	RETURN_MEMBER(getThis(), "position");
}

PHP_METHOD(Stub_ArrayIterator, next)
{
	zval *this_ptr = getThis();
	RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("position")));
}

PHP_METHOD(Stub_ArrayIterator, valid)
{
	zval _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("test", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("position", 8, 1);
	}
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 15, PH_NOISY_CC | PH_READONLY);
	zephir_read_property_cached(&_1, this_ptr, _zephir_prop_1, 16, PH_NOISY_CC | PH_READONLY);
	RETURN_BOOL(zephir_array_isset_value(&_0, &_1));
}

