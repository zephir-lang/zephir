
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
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Stub_Types_MixedType)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Types, MixedType, stub, types_mixedtype, stub_types_mixedtype_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Types_MixedType, returnMixedObject)
{
	zval *this_ptr = getThis();



	object_init(return_value);
	return;
}

PHP_METHOD(Stub_Types_MixedType, returnMixedArray)
{
	zval *this_ptr = getThis();



	array_init(return_value);
	return;
}

PHP_METHOD(Stub_Types_MixedType, returnMixedString)
{
	zval *this_ptr = getThis();



	RETURN_STRING("mixed string");
}

PHP_METHOD(Stub_Types_MixedType, returnMixedInt)
{
	zval *this_ptr = getThis();



	RETURN_LONG(1);
}

PHP_METHOD(Stub_Types_MixedType, returnMixedFloat)
{
	zval *this_ptr = getThis();



	RETURN_DOUBLE(3.14);
}

PHP_METHOD(Stub_Types_MixedType, returnMixedBool)
{
	zval *this_ptr = getThis();



	RETURN_BOOL(1);
}

PHP_METHOD(Stub_Types_MixedType, returnMixedNull)
{
	zval *this_ptr = getThis();



	RETURN_NULL();
}

PHP_METHOD(Stub_Types_MixedType, returnMixed74)
{
	zval *diff_param = NULL;
	zend_bool diff;
	zval *this_ptr = getThis();

#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(diff)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(0, 1, &diff_param);
	if (!diff_param) {
		diff = 0;
	} else {
		diff = zephir_get_boolval(diff_param);
	}


	if (diff) {
		RETURN_STRING("string");
	}
	object_init(return_value);
	return;
}

PHP_METHOD(Stub_Types_MixedType, returnMultiButAlwaysMixed)
{
	zval *this_ptr = getThis();



	RETURN_STRING("IS_MIXED in header");
}

