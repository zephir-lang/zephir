
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
#include "kernel/math.h"


ZEPHIR_INIT_CLASS(Stub_Flow_SwitchFlow) {

	ZEPHIR_REGISTER_CLASS(Stub\\Flow, SwitchFlow, stub, flow_switchflow, stub_flow_switchflow_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch1) {

	zend_long a = 0;
	zval *this_ptr = getThis();




	a = 10;
	RETURN_LONG(a);

}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch2) {

	zend_long a = 0;
	zval *this_ptr = getThis();




	a = 10;
	do {
		if (a == 10) {
			RETURN_BOOL(1);
		}
	} while(0);

	RETURN_BOOL(0);

}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch3) {

	zend_long a = 0;
	zval *this_ptr = getThis();




	a = 10;
	do {
		if (a == 5) {
			RETURN_BOOL(1);
		}
	} while(0);

	RETURN_BOOL(0);

}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch4) {

	zend_long a = 0;
	zval *this_ptr = getThis();




	a = 10;
	do {
		if (a == 5) {
			RETURN_LONG(1);
		}
		RETURN_LONG(2);
	} while(0);

	RETURN_LONG(3);

}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch5) {

	zend_long a = 0;
	zval *this_ptr = getThis();




	a = 10;
	do {
		if (a == 10) {
			RETURN_LONG(1);
		}
		RETURN_LONG(2);
	} while(0);

	RETURN_LONG(3);

}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch6) {

	zend_long a = 0;
	zval *this_ptr = getThis();




	a = 10;
	do {
		if (a == 10 || a == 4) {
			RETURN_LONG(1);
		}
	} while(0);

	RETURN_LONG(2);

}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch7) {

	zend_long a = 0;
	zval *this_ptr = getThis();




	a = 4;
	do {
		if (a == 10 || a == 4) {
			RETURN_LONG(1);
		}
	} while(0);

	RETURN_LONG(2);

}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch8) {

	zend_long a = 0;
	zval *this_ptr = getThis();




	a = 5;
	do {
		if (a == 10 || a == 4) {
			RETURN_LONG(1);
		}
	} while(0);

	RETURN_LONG(2);

}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch9) {

	zend_long a = 0;
	zval *this_ptr = getThis();




	a = 5;
	do {
		if (a == 3 || a == 4) {
			RETURN_LONG(0);
		}
		if (a == 10 || a == 4) {
			RETURN_LONG(1);
		}
	} while(0);

	RETURN_LONG(2);

}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch10) {

	zend_long a = 0;
	zval *this_ptr = getThis();




	a = 5;
	do {
		if (a == 3 || a == 4) {
			RETURN_LONG(0);
		}
		if (a == 10 || a == 4) {
			RETURN_LONG(1);
		}
	} while(0);

	RETURN_LONG(2);

}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch11) {

	zend_long a = 0;
	zval *this_ptr = getThis();




	a = 1;
	do {
		if (a == 2) {
			RETURN_LONG(2);
		}
		RETURN_LONG(1);
	} while(0);


}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch12) {

	zend_bool _0, _1, _2;
	zval *var1, var1_sub, *var2, var2_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&var1_sub);
	ZVAL_UNDEF(&var2_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(var1)
		Z_PARAM_ZVAL(var2)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(2, 0, &var1, &var2);



	do {
		_0 = 1;
		_1 = ZEPHIR_GT(var1, var2);
		if (_1) {
			_1 = ZEPHIR_GT_LONG(var2, 5);
		}
		if (_0 == _1) {
			RETURN_LONG(1);
		}
		_2 = ZEPHIR_LT(var1, var2);
		if (_2) {
			_2 = ZEPHIR_LT_LONG(var1, 5);
		}
		if (_0 == _2) {
			RETURN_LONG(2);
		}
		RETURN_LONG(0);
	} while(0);


}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch13) {

	zval *a_param = NULL, _0, _1;
	long a, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &a_param);

	a = zephir_get_intval(a_param);


	ZVAL_LONG(&_0, 1);
	ZVAL_LONG(&_1, 2);
	do {
		_2 = zephir_mt_rand(zephir_get_intval(&_0), zephir_get_intval(&_1));
		if (_2 == 100) {
			RETURN_LONG(1);
		}
		RETURN_LONG(0);
	} while(0);


}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch14) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *result_type = NULL, result_type_sub, ret, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&result_type_sub);
	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(result_type)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &result_type);

	if (!result_type) {
		result_type = &result_type_sub;
		ZEPHIR_INIT_VAR(result_type);
		ZVAL_LONG(result_type, 1);
	}


	do {
		ZEPHIR_INIT_VAR(&_0);
		ZEPHIR_GET_CONSTANT(&_0, "MYSQL_ASSOC");
		ZEPHIR_INIT_VAR(&_1);
		ZEPHIR_GET_CONSTANT(&_1, "MYSQLI_ASSOC");
		if (ZEPHIR_IS_LONG(result_type, 1) || ZEPHIR_IS_EQUAL(result_type, &_0) || ZEPHIR_IS_EQUAL(result_type, &_1)) {
			ZEPHIR_INIT_VAR(&ret);
			ZVAL_STRING(&ret, "aaa");
			break;
		}
		ZEPHIR_INIT_NVAR(&ret);
		ZVAL_STRING(&ret, "bbb");
		break;
	} while(0);

	RETURN_CCTOR(&ret);

}

