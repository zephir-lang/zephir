
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
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Stub_ArrayAccessTest)
{
	ZEPHIR_REGISTER_CLASS(Stub, ArrayAccessTest, stub, arrayaccesstest, stub_arrayaccesstest_method_entry, 0);

	zend_declare_property_null(stub_arrayaccesstest_ce, SL("data"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_arrayaccesstest_ce, SL("assigedFromMethod"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_arrayaccesstest_ce, SL("unsetData"), ZEND_ACC_PROTECTED);
	stub_arrayaccesstest_ce->create_object = zephir_init_properties_Stub_ArrayAccessTest;

	return SUCCESS;
}

PHP_METHOD(Stub_ArrayAccessTest, exits)
{
	zval arr;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&arr);
	ZVAL_NULL(&arr);
	object_init_ex(&arr, stub_arrayaccessarr_ce);
	ZEPHIR_CALL_METHOD(NULL, &arr, "__construct", NULL, 3);
	zephir_check_call_status();
	RETURN_MM_BOOL(zephir_array_isset_string(&arr, SL("one")));
}

PHP_METHOD(Stub_ArrayAccessTest, get)
{
	zval arr, _0;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_0);
	ZVAL_NULL(&arr);
	object_init_ex(&arr, stub_arrayaccessarr_ce);
	ZEPHIR_CALL_METHOD(NULL, &arr, "__construct", NULL, 3);
	zephir_check_call_status();
	zephir_array_fetch_string(&_0, &arr, SL("two"), PH_NOISY | PH_READONLY, "stub/arrayaccesstest.zep", 29);
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_ArrayAccessTest, unsetByKeyFromArray)
{
	zval data;
	zval *key_param = NULL, *data_param = NULL;
	zval key;

	ZVAL_UNDEF(&key);
	ZVAL_UNDEF(&data);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(key)
		Z_PARAM_ARRAY(data)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &key_param, &data_param);
	if (UNEXPECTED(Z_TYPE_P(key_param) != IS_STRING && Z_TYPE_P(key_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'key' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(key_param) == IS_STRING)) {
		zephir_get_strval(&key, key_param);
	} else {
		ZVAL_NULL(&key);
	}
	zephir_get_arrval(&data, data_param);
	zephir_array_unset(&data, &key, PH_SEPARATE);
	RETURN_CTOR(&data);
}

PHP_METHOD(Stub_ArrayAccessTest, unsetByKeyFromProperty)
{
	zval dataFromProperty;
	zval *key_param = NULL, *dataFromProperty_param = NULL, _0;
	zval key;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&dataFromProperty);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(key)
		Z_PARAM_ARRAY(dataFromProperty)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &key_param, &dataFromProperty_param);
	if (UNEXPECTED(Z_TYPE_P(key_param) != IS_STRING && Z_TYPE_P(key_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'key' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(key_param) == IS_STRING)) {
		zephir_get_strval(&key, key_param);
	} else {
		ZVAL_NULL(&key);
	}
	zephir_get_arrval(&dataFromProperty, dataFromProperty_param);
	zephir_update_property_zval(this_ptr, ZEND_STRL("assigedFromMethod"), &dataFromProperty);
	zephir_unset_property_array(this_ptr, ZEND_STRL("assigedFromMethod"), &key);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("assigedFromMethod"), PH_NOISY_CC | PH_READONLY);
	zephir_array_unset(&_0, &key, PH_SEPARATE);
	RETURN_MM_MEMBER(getThis(), "assigedFromMethod");
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/645
 */
PHP_METHOD(Stub_ArrayAccessTest, issue645)
{
	zval _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_NULL(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("data"), &_0);
	zephir_read_property(&_1, this_ptr, ZEND_STRL("data"), PH_NOISY_CC);
	RETURN_CCTOR(&_1);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1155
 */
PHP_METHOD(Stub_ArrayAccessTest, issue1155)
{
	zval s, _0;
	zval arr;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&_0);
	ZVAL_NULL(&arr);
	zephir_create_array(&arr, 3, 0);
	ZVAL_NULL(&_0);
	ZVAL_STRING(&_0, "0");
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "false");
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "");
	zephir_array_fast_append(&arr, &_0);
	ZVAL_NULL(&s);
	ZVAL_STRING(&s, "true");
	RETURN_MM_BOOL(zephir_fast_in_array(&s, &arr));
}

PHP_METHOD(Stub_ArrayAccessTest, issue1094Test1)
{
	zend_bool isItemsNULL = 0;
	zval *items_param = NULL;
	zval items;

	ZVAL_UNDEF(&items);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY_OR_NULL(items)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 0, 1, &items_param);
	if (!items_param) {
		ZVAL_NULL(&items);
	} else {
		zephir_get_arrval(&items, items_param);
	}
	RETURN_MM_BOOL(isItemsNULL);
}

PHP_METHOD(Stub_ArrayAccessTest, issue1094Test2)
{
	zend_bool isItemsNULL = 0;
	zval *items_param = NULL;
	zval items;

	ZVAL_UNDEF(&items);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY_OR_NULL(items)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 0, 1, &items_param);
	if (!items_param) {
		ZVAL_NULL(&items);
	} else {
		zephir_get_arrval(&items, items_param);
	}
	isItemsNULL = ZEPHIR_IS_NULL(&items);
	RETURN_MM_BOOL(isItemsNULL);
}

PHP_METHOD(Stub_ArrayAccessTest, issue1094Test3)
{
	zend_bool isItemsNULL = 0;
	zval *items_param = NULL;
	zval items;

	ZVAL_UNDEF(&items);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY_OR_NULL(items)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 0, 1, &items_param);
	if (!items_param) {
		ZVAL_NULL(&items);
	} else {
		zephir_get_arrval(&items, items_param);
	}
	isItemsNULL = Z_TYPE_P(&items) == IS_NULL;
	RETURN_MM_BOOL(isItemsNULL);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1086
 */
PHP_METHOD(Stub_ArrayAccessTest, issue1086Strict)
{
	zval *params_param = NULL, _0;
	zval params;

	ZVAL_UNDEF(&params);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(params)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &params_param);
	ZVAL_COPY(&params, params_param);
	ZVAL_NULL(&_0);
	ZVAL_LONG(&_0, 1234);
	zephir_array_update_string(&params, SL("test2"), &_0, PH_COPY | PH_SEPARATE);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1086
 */
PHP_METHOD(Stub_ArrayAccessTest, issue1086WontNullArrayAfterPassViaStaticWithStrictParams)
{
	zval _0;
	zval params;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&params);
	ZVAL_UNDEF(&_0);
	ZVAL_NULL(&params);
	array_init(&params);
	ZVAL_NULL(&_0);
	ZVAL_LONG(&_0, 123);
	zephir_array_update_string(&params, SL("test"), &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_CALL_SELF(NULL, "issue1086strict", NULL, 0, &params);
	zephir_check_call_status();
	RETURN_CTOR(&params);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1086
 */
PHP_METHOD(Stub_ArrayAccessTest, issue1086NotStrictParams)
{
	zval *params_param = NULL, _0;
	zval params;

	ZVAL_UNDEF(&params);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(params)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &params_param);
	zephir_get_arrval(&params, params_param);
	ZVAL_NULL(&_0);
	ZVAL_LONG(&_0, 1234);
	zephir_array_update_string(&params, SL("test2"), &_0, PH_COPY | PH_SEPARATE);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1086
 */
PHP_METHOD(Stub_ArrayAccessTest, issue1086WontNullArrayAfterPassViaStaticWithoutStrictParams)
{
	zval _0;
	zval params;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&params);
	ZVAL_UNDEF(&_0);
	ZVAL_NULL(&params);
	array_init(&params);
	ZVAL_NULL(&_0);
	ZVAL_LONG(&_0, 123);
	zephir_array_update_string(&params, SL("test"), &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_CALL_SELF(NULL, "issue1086notstrictparams", NULL, 0, &params);
	zephir_check_call_status();
	RETURN_CTOR(&params);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1259
 */
PHP_METHOD(Stub_ArrayAccessTest, issue1259UnsetKeyFromArrayInternalVariable)
{
	zval ret, unsetData;

	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&unsetData);
	ZVAL_NULL(&ret);
	array_init(&ret);
	ZVAL_NULL(&unsetData);
	zephir_create_array(&unsetData, 2, 0);
	add_assoc_stringl_ex(&unsetData, SL("key_a"), SL("marcin"));
	add_assoc_stringl_ex(&unsetData, SL("key_b"), SL("paula"));
	zephir_array_append(&ret, &unsetData, PH_SEPARATE, "stub/arrayaccesstest.zep", 154);
	zephir_array_unset_string(&unsetData, SL("key_a"), PH_SEPARATE);
	zephir_array_append(&ret, &unsetData, PH_SEPARATE, "stub/arrayaccesstest.zep", 156);
	RETURN_CTOR(&ret);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1259
 */
PHP_METHOD(Stub_ArrayAccessTest, issue1259UnsetStringKeyFromArrayProperty)
{
	zval _0, _1, _2, _3;
	zval ret;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_NULL(&ret);
	array_init(&ret);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("unsetData"), PH_NOISY_CC | PH_READONLY);
	zephir_array_append(&ret, &_0, PH_SEPARATE, "stub/arrayaccesstest.zep", 168);
	zephir_read_property(&_1, this_ptr, ZEND_STRL("unsetData"), PH_NOISY_CC | PH_READONLY);
	ZVAL_STR(&_1, "key_a");
	zephir_unset_property_array(this_ptr, ZEND_STRL("unsetData"), &_1);
	zephir_read_property(&_2, this_ptr, ZEND_STRL("unsetData"), PH_NOISY_CC | PH_READONLY);
	zephir_array_unset_string(&_2, SL("key_a"), PH_SEPARATE);
	zephir_read_property(&_3, this_ptr, ZEND_STRL("unsetData"), PH_NOISY_CC | PH_READONLY);
	zephir_array_append(&ret, &_3, PH_SEPARATE, "stub/arrayaccesstest.zep", 170);
	RETURN_CTOR(&ret);
}

PHP_METHOD(Stub_ArrayAccessTest, issue1259UnsetLongKeyFromArrayProperty)
{
	zval _0, _1, _2, _3;
	zval ret;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_NULL(&ret);
	array_init(&ret);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("unsetData"), PH_NOISY_CC | PH_READONLY);
	zephir_array_append(&ret, &_0, PH_SEPARATE, "stub/arrayaccesstest.zep", 179);
	zephir_read_property(&_1, this_ptr, ZEND_STRL("unsetData"), PH_NOISY_CC | PH_READONLY);
	ZVAL_LONG(&_1, 3);
	zephir_unset_property_array(this_ptr, ZEND_STRL("unsetData"), &_1);
	zephir_read_property(&_2, this_ptr, ZEND_STRL("unsetData"), PH_NOISY_CC | PH_READONLY);
	zephir_array_unset_long(&_2, 3, PH_SEPARATE);
	zephir_read_property(&_3, this_ptr, ZEND_STRL("unsetData"), PH_NOISY_CC | PH_READONLY);
	zephir_array_append(&ret, &_3, PH_SEPARATE, "stub/arrayaccesstest.zep", 181);
	RETURN_CTOR(&ret);
}

zend_object *zephir_init_properties_Stub_ArrayAccessTest(zend_class_entry *class_type)
{
		zval _1$$3;
	zval _0;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1$$3);
	

	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("unsetData"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZVAL_NULL(&_1$$3);
			zephir_create_array(&_1$$3, 3, 0);
			add_assoc_stringl_ex(&_1$$3, SL("key_a"), SL("marcin"));
			add_assoc_stringl_ex(&_1$$3, SL("key_b"), SL("paula"));
			add_index_stringl(&_1$$3, 3, SL("long value"));
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("unsetData"), &_1$$3);
		}
		return Z_OBJ_P(this_ptr);
	}
}

