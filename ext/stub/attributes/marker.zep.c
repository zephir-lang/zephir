
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
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/operators.h"


/**
 * A Zephir class used as a PHP attribute (#2466).
 *
 * `#[\Attribute(...)]` on an internal class is all `newInstance()` needs: it
 * reads the marker straight off `ce->attributes` and then calls the constructor
 * with the stored arguments, so the properties below are what reflection
 * observes. `\Attribute::TARGET_ALL` is a class constant of an internal class
 * and is folded at compile time, exactly as php-src's own stubs do — which
 * means the emitted literal follows the building PHP, where 8.5 renumbered
 * these flags.
 *
 * Every constructor parameter is typed on purpose: only a typed parameter gets
 * its default into arg_info, and without that PHP cannot skip one to honour a
 * named argument.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2466
 */
ZEPHIR_INIT_CLASS(Stub_Attributes_Marker)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Attributes, Marker, stub, attributes_marker, stub_attributes_marker_method_entry, 0);

	{
		zend_attribute *_za = zephir_add_class_attribute(stub_attributes_marker_ce, SL("Attribute"), 1);
		zval _zc0;
		ZVAL_LONG(&_zc0, 127);
		zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);
	}

	{
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "", sizeof("") - 1);
		zephir_declare_typed_property(stub_attributes_marker_ce, SL("text"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_STRING, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_BOOL(&_zc0, 0);
		zephir_declare_typed_property(stub_attributes_marker_ce, SL("flag"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_BOOL, NULL, 0);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 1);
		zephir_declare_typed_property(stub_attributes_marker_ce, SL("extra"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_ARRAY, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_LONG(&_zc0, 0);
		zephir_declare_typed_property(stub_attributes_marker_ce, SL("number"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_LONG, NULL, 0);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Attributes_Marker, __construct)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long number;
	zval extra;
	zend_bool flag;
	zval text_zv, *flag_param = NULL, *extra_param = NULL, *number_param = NULL, __$true, __$false, _0;
	zend_string *text = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&text_zv);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&extra);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	static zend_string *_zephir_prop_2 = NULL;
	static zend_string *_zephir_prop_3 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("text", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("flag", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_2)) {
		_zephir_prop_2 = zend_string_init("extra", 5, 1);
	}
	if (UNEXPECTED(!_zephir_prop_3)) {
		_zephir_prop_3 = zend_string_init("number", 6, 1);
	}

	ZEND_PARSE_PARAMETERS_START(0, 4)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(text)
		Z_PARAM_BOOL(flag)
		ZEPHIR_Z_PARAM_ARRAY(extra, extra_param)
		Z_PARAM_LONG(number)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	if (ZEND_NUM_ARGS() > 1) {
		flag_param = ZEND_CALL_ARG(execute_data, 2);
	}
	if (ZEND_NUM_ARGS() > 2) {
		extra_param = ZEND_CALL_ARG(execute_data, 3);
	}
	if (ZEND_NUM_ARGS() > 3) {
		number_param = ZEND_CALL_ARG(execute_data, 4);
	}
	if (!text) {
		text = zend_string_init(ZEND_STRL(""), 0);
		zephir_memory_observe(&text_zv);
		ZVAL_STR(&text_zv, text);
	} else {
		zephir_memory_observe(&text_zv);
	ZVAL_STR_COPY(&text_zv, text);
	}
	if (!flag_param) {
		flag = 0;
	} else {
		}
	if (!extra_param) {
		ZEPHIR_INIT_VAR(&extra);
		array_init(&extra);
	} else {
		zephir_get_arrval(&extra, extra_param);
	}
	if (!number_param) {
		number = 0;
	} else {
		}
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 22, &text_zv);
	if (flag) {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_1, 23, &__$true);
	} else {
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_1, 23, &__$false);
	}
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_2, 24, &extra);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, number);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_3, 25, &_0);
	ZEPHIR_MM_RESTORE();
}

