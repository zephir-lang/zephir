
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


/**
 * Extension Globals
 */
ZEPHIR_INIT_CLASS(Test_Globals) {

	ZEPHIR_REGISTER_CLASS(Test, Globals, test, globals, test_globals_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Globals, getDefaultGlobals1) {


	RETURN_BOOL(ZEPHIR_GLOBAL(test).my_setting_1);

}

PHP_METHOD(Test_Globals, getDefaultGlobals2) {


	RETURN_LONG(ZEPHIR_GLOBAL(test).my_setting_2);

}

PHP_METHOD(Test_Globals, getDefaultGlobals3) {


	RETURN_DOUBLE(ZEPHIR_GLOBAL(test).my_setting_3);

}

PHP_METHOD(Test_Globals, getDefaultGlobals4) {


	RETURN_BOOL(ZEPHIR_GLOBAL(my_setting_1));

}

PHP_METHOD(Test_Globals, getDefaultGlobals5) {


	RETURN_LONG(ZEPHIR_GLOBAL(my_setting_2));

}

PHP_METHOD(Test_Globals, getDefaultGlobals6) {


	RETURN_DOUBLE(ZEPHIR_GLOBAL(my_setting_3));

}

