
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
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"


/**
 * Chars specific tests
 */
ZEPHIR_INIT_CLASS(Stub_Chars) {

	ZEPHIR_REGISTER_CLASS(Stub, Chars, stub, chars, stub_chars_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_Chars, sumChars1) {

	char ch, chlower = 0;
	zval *this_ptr = getThis();




	ch = 'A';
	chlower = (ch + 32);
	RETURN_LONG(chlower);

}

PHP_METHOD(Stub_Chars, sumChars2) {

	zval *ch_param = NULL;
	char ch, chlower = 0;
	zval *this_ptr = getThis();

#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(ch)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &ch_param);

	ch = zephir_get_charval(ch_param);


	chlower = (ch + 32);
	RETURN_LONG(chlower);

}

PHP_METHOD(Stub_Chars, diffChars1) {

	char ch, chlower = 0;
	zval *this_ptr = getThis();




	ch = 'a';
	chlower = (ch - 32);
	RETURN_LONG(chlower);

}

PHP_METHOD(Stub_Chars, diffChars2) {

	zval *ch_param = NULL;
	char ch, chlower = 0;
	zval *this_ptr = getThis();

#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(ch)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &ch_param);

	ch = zephir_get_charval(ch_param);


	chlower = (ch - 32);
	RETURN_LONG(chlower);

}

