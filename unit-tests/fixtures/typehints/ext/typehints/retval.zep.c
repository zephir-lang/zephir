
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


ZEPHIR_INIT_CLASS(TypeHints_RetVal) {

	ZEPHIR_REGISTER_CLASS(TypeHints, RetVal, typehints, retval, typehints_retval_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(TypeHints_RetVal, retval_var) {

	zval _var;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_var);


	RETURN_CCTORW(&_var);

}

PHP_METHOD(TypeHints_RetVal, retval_string) {

	zval _var;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_var);


	RETURN_CTORW(&_var);

}

PHP_METHOD(TypeHints_RetVal, retval_boolean) {

	zend_bool _var = 0;
	zval *this_ptr = getThis();



	RETURN_BOOL(_var);

}

PHP_METHOD(TypeHints_RetVal, retval_int) {

	zend_long _var = 0;
	zval *this_ptr = getThis();



	RETURN_LONG(_var);

}

PHP_METHOD(TypeHints_RetVal, retval_long) {

	long _var = 0;
	zval *this_ptr = getThis();



	RETURN_LONG(_var);

}

PHP_METHOD(TypeHints_RetVal, retval_double) {

	double _var = 0;
	zval *this_ptr = getThis();



	RETURN_DOUBLE(_var);

}

PHP_METHOD(TypeHints_RetVal, retval_retval) {

	zval _var;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_var);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_var);
	object_init_ex(&_var, typehints_retval_ce);
	if (zephir_has_constructor(&_var TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, &_var, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_CCTOR(&_var);

}

PHP_FUNCTION(f_TypeHints_retval_var) {
	zval _var;
		ZVAL_UNDEF(&_var);


	RETURN_CCTORW(&_var);

}

PHP_FUNCTION(f_TypeHints_retval_string) {
	zval _var;
		ZVAL_UNDEF(&_var);


	RETURN_CTORW(&_var);

}

PHP_FUNCTION(f_TypeHints_retval_boolean) {
	zend_bool _var = 0;


	RETURN_BOOL(_var);

}

PHP_FUNCTION(f_TypeHints_retval_int) {
	zend_long _var = 0;


	RETURN_LONG(_var);

}

PHP_FUNCTION(f_TypeHints_retval_long) {
	long _var = 0;


	RETURN_LONG(_var);

}

PHP_FUNCTION(f_TypeHints_retval_double) {
	double _var = 0;


	RETURN_DOUBLE(_var);

}

PHP_FUNCTION(f_TypeHints_retval_retval) {
	zval _var;
	zend_long ZEPHIR_LAST_CALL_STATUS;
		ZVAL_UNDEF(&_var);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_var);
	object_init_ex(&_var, typehints_retval_ce);
	if (zephir_has_constructor(&_var TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, &_var, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_CCTOR(&_var);

}

