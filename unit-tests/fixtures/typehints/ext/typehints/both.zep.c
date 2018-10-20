
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


ZEPHIR_INIT_CLASS(TypeHints_Both) {

	ZEPHIR_REGISTER_CLASS(TypeHints, Both, typehints, both, typehints_both_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(TypeHints_Both, both) {

	zval *_string_param = NULL;
	zval _string;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_string);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &_string_param);

	zephir_get_strval(&_string, _string_param);


	RETURN_CTOR(&_string);

}

PHP_FUNCTION(f_TypeHints_both) {
	zval *_string_param = NULL;
	zval _string;
		ZVAL_UNDEF(&_string);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &_string_param);

	zephir_get_strval(&_string, _string_param);


	RETURN_CTOR(&_string);

}

