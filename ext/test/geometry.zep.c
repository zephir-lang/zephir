
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


ZEPHIR_INIT_CLASS(Test_Geometry) {

	ZEPHIR_REGISTER_CLASS(Test, Geometry, test, geometry, test_geometry_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Geometry, run) {

	zephir_fcall_cache_entry *_0 = NULL;
	int count, i = 0, ZEPHIR_LAST_CALL_STATUS;
	zval *list_param = NULL, *count_param = NULL, *_1, *_2, *_3, *_4, *_5, *_6, *_7, *_8;
	zval *list = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &list_param, &count_param);

	zephir_get_arrval(list, list_param);
	count = zephir_get_intval(count_param);


	while (1) {
		if (!(i < count)) {
			break;
		}
		zephir_array_fetch_long(&_1, list, i, PH_NOISY | PH_READONLY, "test/geometry.zep", 11 TSRMLS_CC);
		zephir_array_fetch_long(&_2, _1, 0, PH_NOISY | PH_READONLY, "test/geometry.zep", 11 TSRMLS_CC);
		zephir_array_fetch_long(&_3, list, i, PH_NOISY | PH_READONLY, "test/geometry.zep", 11 TSRMLS_CC);
		zephir_array_fetch_long(&_4, _3, 1, PH_NOISY | PH_READONLY, "test/geometry.zep", 11 TSRMLS_CC);
		zephir_array_fetch_long(&_5, list, i, PH_NOISY | PH_READONLY, "test/geometry.zep", 11 TSRMLS_CC);
		zephir_array_fetch_long(&_6, _5, 2, PH_NOISY | PH_READONLY, "test/geometry.zep", 11 TSRMLS_CC);
		zephir_array_fetch_long(&_7, list, i, PH_NOISY | PH_READONLY, "test/geometry.zep", 11 TSRMLS_CC);
		zephir_array_fetch_long(&_8, _7, 3, PH_NOISY | PH_READONLY, "test/geometry.zep", 11 TSRMLS_CC);
		ZEPHIR_CALL_STATIC(NULL, "distancestatic", &_0, 0, _2, _4, _6, _8);
		zephir_check_call_status();
		i = (i + 1);
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Geometry, runOptimize) {

	zephir_fcall_cache_entry *_0 = NULL;
	int count, i = 0, ZEPHIR_LAST_CALL_STATUS;
	zval *list_param = NULL, *count_param = NULL, *item = NULL, *_1, *_2, *_3, *_4;
	zval *list = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &list_param, &count_param);

	zephir_get_arrval(list, list_param);
	count = zephir_get_intval(count_param);


	while (1) {
		if (!(i < count)) {
			break;
		}
		ZEPHIR_OBS_NVAR(item);
		zephir_array_fetch_long(&item, list, i, PH_NOISY, "test/geometry.zep", 22 TSRMLS_CC);
		zephir_array_fetch_long(&_1, item, 0, PH_NOISY | PH_READONLY, "test/geometry.zep", 24 TSRMLS_CC);
		zephir_array_fetch_long(&_2, item, 1, PH_NOISY | PH_READONLY, "test/geometry.zep", 24 TSRMLS_CC);
		zephir_array_fetch_long(&_3, item, 2, PH_NOISY | PH_READONLY, "test/geometry.zep", 24 TSRMLS_CC);
		zephir_array_fetch_long(&_4, item, 3, PH_NOISY | PH_READONLY, "test/geometry.zep", 24 TSRMLS_CC);
		ZEPHIR_CALL_STATIC(NULL, "distancestatic", &_0, 0, _1, _2, _3, _4);
		zephir_check_call_status();
		i++;
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Geometry, distanceStatic) {

	zval *x1_param = NULL, *y1_param = NULL, *x2_param = NULL, *y2_param = NULL;
	double x1, y1, x2, y2;

	zephir_fetch_params(0, 4, 0, &x1_param, &y1_param, &x2_param, &y2_param);

	x1 = zephir_get_doubleval(x1_param);
	y1 = zephir_get_doubleval(y1_param);
	x2 = zephir_get_doubleval(x2_param);
	y2 = zephir_get_doubleval(y2_param);


	RETURN_LONG(sqrt(((((x1 - x2)) * ((x1 - x2))) + (((y1 - y2)) * ((y1 - y2))))));

}

