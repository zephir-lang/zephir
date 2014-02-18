
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
#include "kernel/concat.h"


ZEPHIR_INIT_CLASS(Test_Concat) {

	ZEPHIR_REGISTER_CLASS(Test, Concat, test, concat, test_concat_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Concat, testConcat1) {



}

PHP_METHOD(Test_Concat, testConcat10) {

	zval *url = NULL, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(url);
	ZVAL_STRING(url, "test", 1);
	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_CONCAT_SV(_0, "append", url);
	ZEPHIR_CPY_WRT(url, _0);
	RETURN_CCTOR(url);

}

