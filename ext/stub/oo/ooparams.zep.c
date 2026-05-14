
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
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"


/**
 *
 */
ZEPHIR_INIT_CLASS(Stub_Oo_OoParams)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Oo, OoParams, stub, oo_ooparams, stub_oo_ooparams_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Oo_OoParams, createThisClassWithoutWriteCurrentNamespace)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	object_init_ex(return_value, stub_oo_ooparams_ce);
	if (zephir_has_constructor(return_value)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	RETURN_MM();
}

PHP_METHOD(Stub_Oo_OoParams, createOtherClassWithoutWriteCurrentNamespace)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	object_init_ex(return_value, stub_oo_oodynamica_ce);
	if (zephir_has_constructor(return_value)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	RETURN_MM();
}

PHP_METHOD(Stub_Oo_OoParams, setAge)
{
	zval *age_param = NULL;
	zend_long age;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(age)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &age_param);
	RETURN_LONG(age);
}

PHP_METHOD(Stub_Oo_OoParams, setAverage)
{
	zval *average_param = NULL;
	double average;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(average_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &average_param);
	average = zephir_get_doubleval(average_param);
	RETURN_DOUBLE(average);
}

PHP_METHOD(Stub_Oo_OoParams, setName)
{
	zval name_zv;
	zend_string *name = NULL;

	ZVAL_UNDEF(&name_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&name_zv, name);
	RETURN_STR(zend_string_copy(name));
}

PHP_METHOD(Stub_Oo_OoParams, setEnabled)
{
	zval *enabled_param = NULL;
	zend_bool enabled;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(enabled)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &enabled_param);
	RETURN_BOOL(enabled);
}

PHP_METHOD(Stub_Oo_OoParams, setList)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *someList_param = NULL;
	zval someList;

	ZVAL_UNDEF(&someList);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(someList, someList_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &someList_param);
	zephir_get_arrval(&someList, someList_param);
	RETURN_CTOR(&someList);
}

PHP_METHOD(Stub_Oo_OoParams, setStrictAge)
{
	zval *age_param = NULL;
	zend_long age;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(age)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &age_param);
	if (UNEXPECTED(Z_TYPE_P(age_param) != IS_LONG)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'age' must be of the type int"));
		RETURN_NULL();
	}
	age = Z_LVAL_P(age_param);
	RETURN_LONG(age);
}

PHP_METHOD(Stub_Oo_OoParams, setStrictAverage)
{
	zval *average_param = NULL;
	double average;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(average_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &average_param);
	if (UNEXPECTED(Z_TYPE_P(average_param) != IS_DOUBLE)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'average' must be of the type double"));
		RETURN_NULL();
	}
	average = Z_DVAL_P(average_param);
	RETURN_DOUBLE(average);
}

PHP_METHOD(Stub_Oo_OoParams, setStrictNameFromZephirLand)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_LONG(&_0, 12345);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "setstrictname", NULL, 0, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Oo_OoParams, setStrictName)
{
	zval name_zv;
	zend_string *name = NULL;

	ZVAL_UNDEF(&name_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&name_zv, name);
	RETURN_STR(zend_string_copy(name));
}

PHP_METHOD(Stub_Oo_OoParams, setStrictEnabled)
{
	zval *enabled_param = NULL;
	zend_bool enabled;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(enabled)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &enabled_param);
	RETURN_BOOL(enabled);
}

PHP_METHOD(Stub_Oo_OoParams, setStrictList)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *someList_param = NULL;
	zval someList;

	ZVAL_UNDEF(&someList);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(someList, someList_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &someList_param);
	ZEPHIR_OBS_COPY_OR_DUP(&someList, someList_param);
	RETURN_CTOR(&someList);
}

PHP_METHOD(Stub_Oo_OoParams, setAgeDefault)
{
	zval *age_param = NULL;
	zend_long age;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(age)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &age_param);
	if (!age_param) {
		age = 25;
	} else {
		}
	RETURN_LONG(age);
}

PHP_METHOD(Stub_Oo_OoParams, setAverageDefault)
{
	zval *average_param = NULL;
	double average;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(average_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &average_param);
	if (!average_param) {
		average = (double) 25;
	} else {
		average = zephir_get_doubleval(average_param);
	}
	RETURN_DOUBLE(average);
}

PHP_METHOD(Stub_Oo_OoParams, setNameDefault)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval name_zv;
	zend_string *name = NULL;

	ZVAL_UNDEF(&name_zv);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	if (!name) {
		name = zend_string_init(ZEND_STRL("peter"), 0);
		zephir_memory_observe(&name_zv);
		ZVAL_STR(&name_zv, name);
	} else {
		zephir_memory_observe(&name_zv);
	ZVAL_STR_COPY(&name_zv, name);
	}
	RETURN_MM_STR(zend_string_copy(name));
}

PHP_METHOD(Stub_Oo_OoParams, setEnabledDefault)
{
	zval *enabled_param = NULL;
	zend_bool enabled;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(enabled)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &enabled_param);
	if (!enabled_param) {
		enabled = 0;
	} else {
		}
	RETURN_BOOL(enabled);
}

PHP_METHOD(Stub_Oo_OoParams, setListDefault)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *someList_param = NULL;
	zval someList;

	ZVAL_UNDEF(&someList);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		ZEPHIR_Z_PARAM_ARRAY(someList, someList_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 1, &someList_param);
	if (!someList_param) {
		ZEPHIR_INIT_VAR(&someList);
		array_init(&someList);
	} else {
		zephir_get_arrval(&someList, someList_param);
	}
	RETURN_CTOR(&someList);
}

PHP_METHOD(Stub_Oo_OoParams, setConstAge)
{
	zval *age_param = NULL;
	zend_long age;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(age)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &age_param);
	RETURN_LONG(age);
}

PHP_METHOD(Stub_Oo_OoParams, setConstAverage)
{
	zval *average_param = NULL;
	double average;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(average_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &average_param);
	average = zephir_get_doubleval(average_param);
	RETURN_DOUBLE(average);
}

PHP_METHOD(Stub_Oo_OoParams, setObject)
{
	zval *obj, obj_sub;

	ZVAL_UNDEF(&obj_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT(obj)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &obj);
	RETVAL_ZVAL(obj, 1, 0);
	return;
}

PHP_METHOD(Stub_Oo_OoParams, setCallable)
{
	zval *obj, obj_sub;

	ZVAL_UNDEF(&obj_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(obj)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &obj);
	RETVAL_ZVAL(obj, 1, 0);
	return;
}

PHP_METHOD(Stub_Oo_OoParams, setResource)
{
	zval *obj, obj_sub;

	ZVAL_UNDEF(&obj_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_RESOURCE(obj)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &obj);
	RETVAL_ZVAL(obj, 1, 0);
	return;
}

PHP_METHOD(Stub_Oo_OoParams, setObjectClassCast)
{
	zval *parameter, parameter_sub;

	ZVAL_UNDEF(&parameter_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(parameter, stub_oo_param_ce)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &parameter);
	RETVAL_ZVAL(parameter, 1, 0);
	return;
}

PHP_METHOD(Stub_Oo_OoParams, getDefaultParamValue)
{
	zval *i_param = NULL;
	zend_long i;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(i)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &i_param);
	if (!i_param) {
		i = 0;
	} else {
		}
	RETURN_LONG(i);
}

PHP_METHOD(Stub_Oo_OoParams, getDefaultParamValue1)
{
	zval *i_param = NULL;
	zend_long i;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(i)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &i_param);
	if (!i_param) {
		i = 1;
	} else {
		}
	RETURN_LONG(i);
}

PHP_METHOD(Stub_Oo_OoParams, getDefaultParamValueStr)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval str_zv;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	if (!str) {
		str = zend_string_init(ZEND_STRL("default"), 0);
		zephir_memory_observe(&str_zv);
		ZVAL_STR(&str_zv, str);
	} else {
		zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	}
	RETURN_MM_STR(zend_string_copy(str));
}

