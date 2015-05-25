
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


ZEPHIR_INIT_CLASS(Test_Oo_DeprecatedMethods) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, DeprecatedMethods, test, oo_deprecatedmethods, test_oo_deprecatedmethods_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Oo_DeprecatedMethods, deprecatedMethod) {


	RETURN_BOOL(1);

}

PHP_METHOD(Test_Oo_DeprecatedMethods, normalMethod) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "privatedepricatedmethod", NULL, 54);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Oo_DeprecatedMethods, privateDepricatedMethod) {


	RETURN_BOOL(1);

}

