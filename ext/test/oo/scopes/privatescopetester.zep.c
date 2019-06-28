
#ifdef HAVE_CONFIG_H
#include "../../../ext_config.h"
#endif

#include <php.h>
#include "../../../php_ext.h"
#include "../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Test_Oo_Scopes_PrivateScopeTester) {

	ZEPHIR_REGISTER_CLASS_EX(Test\\Oo\\Scopes, PrivateScopeTester, test, oo_scopes_privatescopetester, test_oo_scopes_hasprivatemethod_ce, test_oo_scopes_privatescopetester_method_entry, 0);

	zend_class_implements(test_oo_scopes_privatescopetester_ce TSRMLS_CC, 1, test_oo_scopes_scopetesterinterface_ce);
	return SUCCESS;

}

PHP_METHOD(Test_Oo_Scopes_PrivateScopeTester, run) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();


	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "callprivatemethod", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

