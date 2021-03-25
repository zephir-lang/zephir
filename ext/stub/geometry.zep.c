
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
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "math.h"


ZEPHIR_INIT_CLASS(Stub_Geometry) {

	ZEPHIR_REGISTER_CLASS(Stub, Geometry, stub, geometry, stub_geometry_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_Geometry, run) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL;
	zend_long count, ZEPHIR_LAST_CALL_STATUS, i;
	zval *list_param = NULL, *count_param = NULL, _1$$3, _2$$3, _3$$3, _4$$3, _5$$3, _6$$3, _7$$3, _8$$3;
	zval list;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&list);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$3);
	ZVAL_UNDEF(&_6$$3);
	ZVAL_UNDEF(&_7$$3);
	ZVAL_UNDEF(&_8$$3);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(list)
		Z_PARAM_LONG(count)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &list_param, &count_param);

	zephir_get_arrval(&list, list_param);
	count = zephir_get_intval(count_param);


	i = 0;
	while (1) {
		if (!(i < count)) {
			break;
		}
		zephir_array_fetch_long(&_1$$3, &list, i, PH_NOISY | PH_READONLY, "stub/geometry.zep", 11);
		zephir_array_fetch_long(&_2$$3, &_1$$3, 0, PH_NOISY | PH_READONLY, "stub/geometry.zep", 11);
		zephir_array_fetch_long(&_3$$3, &list, i, PH_NOISY | PH_READONLY, "stub/geometry.zep", 11);
		zephir_array_fetch_long(&_4$$3, &_3$$3, 1, PH_NOISY | PH_READONLY, "stub/geometry.zep", 11);
		zephir_array_fetch_long(&_5$$3, &list, i, PH_NOISY | PH_READONLY, "stub/geometry.zep", 11);
		zephir_array_fetch_long(&_6$$3, &_5$$3, 2, PH_NOISY | PH_READONLY, "stub/geometry.zep", 11);
		zephir_array_fetch_long(&_7$$3, &list, i, PH_NOISY | PH_READONLY, "stub/geometry.zep", 11);
		zephir_array_fetch_long(&_8$$3, &_7$$3, 3, PH_NOISY | PH_READONLY, "stub/geometry.zep", 11);
		ZEPHIR_CALL_STATIC(NULL, "distancestatic", &_0, 0, &_2$$3, &_4$$3, &_6$$3, &_8$$3);
		zephir_check_call_status();
		i = (i + 1);
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Stub_Geometry, runOptimize) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL;
	zend_long count, ZEPHIR_LAST_CALL_STATUS, i;
	zval *list_param = NULL, *count_param = NULL, item, _1$$3, _2$$3, _3$$3, _4$$3;
	zval list;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&list);
	ZVAL_UNDEF(&item);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(list)
		Z_PARAM_LONG(count)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &list_param, &count_param);

	zephir_get_arrval(&list, list_param);
	count = zephir_get_intval(count_param);


	i = 0;
	while (1) {
		if (!(i < count)) {
			break;
		}
		ZEPHIR_OBS_NVAR(&item);
		zephir_array_fetch_long(&item, &list, i, PH_NOISY, "stub/geometry.zep", 22);
		zephir_array_fetch_long(&_1$$3, &item, 0, PH_NOISY | PH_READONLY, "stub/geometry.zep", 24);
		zephir_array_fetch_long(&_2$$3, &item, 1, PH_NOISY | PH_READONLY, "stub/geometry.zep", 24);
		zephir_array_fetch_long(&_3$$3, &item, 2, PH_NOISY | PH_READONLY, "stub/geometry.zep", 24);
		zephir_array_fetch_long(&_4$$3, &item, 3, PH_NOISY | PH_READONLY, "stub/geometry.zep", 24);
		ZEPHIR_CALL_STATIC(NULL, "distancestatic", &_0, 0, &_1$$3, &_2$$3, &_3$$3, &_4$$3);
		zephir_check_call_status();
		i++;
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Stub_Geometry, distanceStatic) {

	zval *x1_param = NULL, *y1_param = NULL, *x2_param = NULL, *y2_param = NULL, _0;
	double x1, y1, x2, y2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_ZVAL(x1)
		Z_PARAM_ZVAL(y1)
		Z_PARAM_ZVAL(x2)
		Z_PARAM_ZVAL(y2)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(4, 0, &x1_param, &y1_param, &x2_param, &y2_param);

	x1 = zephir_get_doubleval(x1_param);
	y1 = zephir_get_doubleval(y1_param);
	x2 = zephir_get_doubleval(x2_param);
	y2 = zephir_get_doubleval(y2_param);


	ZVAL_DOUBLE(&_0, ((((x1 - x2)) * ((x1 - x2))) + (((y1 - y2)) * ((y1 - y2)))));
	RETURN_DOUBLE(sqrt(((((x1 - x2)) * ((x1 - x2))) + (((y1 - y2)) * ((y1 - y2))))));

}

