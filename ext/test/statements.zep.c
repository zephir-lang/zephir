
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
#include "kernel/object.h"
#include "kernel/string.h"


ZEPHIR_INIT_CLASS(Test_Statements) {

	ZEPHIR_REGISTER_CLASS(Test, Statements, test, statements, test_statements_method_entry, 0);

	zend_declare_property_string(test_statements_ce, SL("tmp1"), "test", ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_property_string(test_statements_ce, SL("tmp2"), "test string", ZEND_ACC_PUBLIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Statements, testElseIf) {

	zval *num_param = NULL;
	int num;

	zephir_fetch_params(0, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	if (num > 0) {
		RETURN_STRING("more", 1);
	} else if (num == 0) {
		RETURN_STRING("equal", 1);
	} else if (num == -1) {
		RETURN_STRING("-1", 1);
	} else {
		RETURN_STRING("less", 1);
	}

}

PHP_METHOD(Test_Statements, testPropertyAcccessAvoidTmpReuse) {

	zval *result1, *result2, *result3, *result4, *_0, *_1, *_2, *_3, *_4, *_5, *_6, *_7;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("tmp2"), PH_NOISY_CC);
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("tmp1"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(result1);
	zephir_fast_strpos(result1, _0, _1, 0 );
	_2 = zephir_fetch_nproperty_this(this_ptr, SL("tmp2"), PH_NOISY_CC);
	_3 = zephir_fetch_nproperty_this(this_ptr, SL("tmp1"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(result2);
	zephir_fast_strpos(result2, _2, _3, 0 );
	_4 = zephir_fetch_nproperty_this(this_ptr, SL("tmp2"), PH_NOISY_CC);
	_5 = zephir_fetch_nproperty_this(this_ptr, SL("tmp1"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(result3);
	zephir_fast_strpos(result3, _4, _5, 0 );
	_6 = zephir_fetch_nproperty_this(this_ptr, SL("tmp2"), PH_NOISY_CC);
	_7 = zephir_fetch_nproperty_this(this_ptr, SL("tmp1"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(result4);
	zephir_fast_strpos(result4, _6, _7, 0 );
	ZEPHIR_MM_RESTORE();

}

