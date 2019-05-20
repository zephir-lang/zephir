
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
#include "kernel/memory.h"
#include "kernel/operators.h"


/**
 * Chars specific tests
 */
ZEPHIR_INIT_CLASS(Test_Chars) {

	ZEPHIR_REGISTER_CLASS(Test, Chars, test, chars, test_chars_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Chars, sumChars1) {

	char ch, chlower = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	ch = 'A';
	chlower = (ch + 32);
	RETURN_MM_LONG(chlower);

}

PHP_METHOD(Test_Chars, sumChars2) {

	zval *ch_param = NULL;
	char ch, chlower = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 1, 0, &ch_param);

	ch = zephir_get_charval(ch_param);


	chlower = (ch + 32);
	RETURN_MM_LONG(chlower);

}

PHP_METHOD(Test_Chars, diffChars1) {

	char ch, chlower = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	ch = 'a';
	chlower = (ch - 32);
	RETURN_MM_LONG(chlower);

}

PHP_METHOD(Test_Chars, diffChars2) {

	zval *ch_param = NULL;
	char ch, chlower = 0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 1, 0, &ch_param);

	ch = zephir_get_charval(ch_param);


	chlower = (ch - 32);
	RETURN_MM_LONG(chlower);

}

