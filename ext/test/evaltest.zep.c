
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
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Test_EvalTest) {

	ZEPHIR_REGISTER_CLASS(Test, EvalTest, test, evaltest, test_evaltest_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_EvalTest, evalCode) {

	zval *code_param = NULL;
	zval code;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&code);

	zephir_fetch_params(1, 1, 0, &code_param);

	zephir_get_strval(&code, code_param);


	zephir_eval_php(&code, return_value, "test/evaltest.zep:7" TSRMLS_CC);
	RETURN_MM();

}

