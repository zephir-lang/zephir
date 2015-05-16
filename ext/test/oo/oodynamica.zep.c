
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
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"


/**
 * Class with dynamic new
 */
ZEPHIR_INIT_CLASS(Test_Oo_OoDynamicA) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, OoDynamicA, test, oo_oodynamica, test_oo_oodynamica_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Oo_OoDynamicA, getNew) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_class_entry *_1;
	zval *className, *fullClassName, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(className);
	zephir_get_called_class(className TSRMLS_CC);
	ZEPHIR_INIT_VAR(fullClassName);
	ZEPHIR_CONCAT_SV(fullClassName, "\\", className);
	zephir_fetch_safe_class(_0, fullClassName);
	_1 = zend_fetch_class(Z_STRVAL_P(_0), Z_STRLEN_P(_0), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(return_value, _1);
	if (zephir_has_constructor(return_value TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_MM();

}

