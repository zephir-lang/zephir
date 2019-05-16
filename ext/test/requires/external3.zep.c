
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
#include "kernel/require.h"
#include "kernel/object.h"


/**
 * External3 operations
 */
ZEPHIR_INIT_CLASS(Test_Requires_External3) {

	ZEPHIR_REGISTER_CLASS(Test\\Requires, External3, test, requires_external3, test_requires_external3_method_entry, 0);

	zend_declare_property_null(test_requires_external3_ce, SL("someVariable"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Requires_External3, req) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *path, path_sub, *requires, requires_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&requires_sub);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &path, &requires);



	ZEPHIR_CALL_FUNCTION(NULL, "ob_clean", NULL, 64);
	zephir_check_call_status();
	if (zephir_require_zval(path TSRMLS_CC) == FAILURE) {
		RETURN_MM_NULL();
	}
	ZEPHIR_CALL_FUNCTION(&_0, "ob_get_contents", NULL, 65);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, requires, "setcontent", NULL, 0, &_0);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

