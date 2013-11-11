
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
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/concat.h"


/**
 * Class with dynamic new
 */
ZEPHIR_INIT_CLASS(Test_Oo_OoDynamicA) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, OoDynamicA, test, oo_oodynamica, test_oo_oodynamica_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Oo_OoDynamicA, getNew) {

	zend_class_entry *_0;
	zval *className, *fullClassName;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(className);
	zephir_call_func(className, "get_called_class");
	ZEPHIR_INIT_VAR(fullClassName);
	ZEPHIR_CONCAT_SV(fullClassName, "\\", className);
	_0 = zend_fetch_class(Z_STRVAL_P(fullClassName), Z_STRLEN_P(fullClassName), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(return_value, _0);
	RETURN_MM();

}

