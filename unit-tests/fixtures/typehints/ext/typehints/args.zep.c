
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
#include "kernel/operators.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(TypeHints_Args) {

	ZEPHIR_REGISTER_CLASS(TypeHints, Args, typehints, args, typehints_args_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(TypeHints_Args, args) {

	double _double;
	long _long;
	zend_long _int;
	zend_bool _bool;
	zval _string;
	zval *_var, _var_sub, *_string_param = NULL, *_bool_param = NULL, *_int_param = NULL, *_long_param = NULL, *_double_param = NULL, *_args, _args_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_var_sub);
	ZVAL_UNDEF(&_args_sub);
	ZVAL_UNDEF(&_string);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 7, 0, &_var, &_string_param, &_bool_param, &_int_param, &_long_param, &_double_param, &_args);

	zephir_get_strval(&_string, _string_param);
	_bool = zephir_get_boolval(_bool_param);
	_int = zephir_get_intval(_int_param);
	_long = zephir_get_intval(_long_param);
	_double = zephir_get_doubleval(_double_param);


	zend_print_zval(_var, 0);
	zend_print_zval(&_string, 0);
	php_printf("%s", _bool ? "1": "");
	php_printf("%ld", _long);
	php_printf("%f", _double);
	zend_print_zval(_args, 0);
	ZEPHIR_MM_RESTORE();

}

PHP_FUNCTION(f_TypeHints_args) {
	double _double;
	long _long;
	zend_long _int;
	zend_bool _bool;
	zval _string;
	zval *_var, _var_sub, *_string_param = NULL, *_bool_param = NULL, *_int_param = NULL, *_long_param = NULL, *_double_param = NULL, *_args, _args_sub;
		ZVAL_UNDEF(&_var_sub);
	ZVAL_UNDEF(&_args_sub);
	ZVAL_UNDEF(&_string);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 7, 0, &_var, &_string_param, &_bool_param, &_int_param, &_long_param, &_double_param, &_args);

	zephir_get_strval(&_string, _string_param);
	_bool = zephir_get_boolval(_bool_param);
	_int = zephir_get_intval(_int_param);
	_long = zephir_get_intval(_long_param);
	_double = zephir_get_doubleval(_double_param);


	zend_print_zval(_var, 0);
	zend_print_zval(&_string, 0);
	php_printf("%s", _bool ? "1": "");
	php_printf("%ld", _long);
	php_printf("%f", _double);
	zend_print_zval(_args, 0);
	ZEPHIR_MM_RESTORE();

}

