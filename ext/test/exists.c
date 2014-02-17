
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
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/file.h"


ZEPHIR_INIT_CLASS(Test_Exists) {

	ZEPHIR_REGISTER_CLASS(Test, Exists, test, exists, test_exists_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Exists, testClassExists) {

	zend_bool autoload;
	zval *className, *autoload_param = NULL;

	zephir_fetch_params(0, 1, 1, &className, &autoload_param);

	if (!autoload_param) {
		autoload = 1;
	} else {
		autoload = zephir_get_boolval(autoload_param);
	}


	RETURN_BOOL(zephir_class_exists(className, zephir_is_true((autoload ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)))  TSRMLS_CC));

}

PHP_METHOD(Test_Exists, testInterfaceExists) {

	zend_bool autoload;
	zval *interfaceName, *autoload_param = NULL;

	zephir_fetch_params(0, 1, 1, &interfaceName, &autoload_param);

	if (!autoload_param) {
		autoload = 1;
	} else {
		autoload = zephir_get_boolval(autoload_param);
	}


	RETURN_BOOL(zephir_interface_exists(interfaceName, zephir_is_true((autoload ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)))  TSRMLS_CC));

}

PHP_METHOD(Test_Exists, testMethodExists) {

	zval *obj, *methodName;

	zephir_fetch_params(0, 2, 0, &obj, &methodName);



	RETURN_BOOL((zephir_method_exists(obj, methodName TSRMLS_CC)  == SUCCESS));

}

PHP_METHOD(Test_Exists, testFileExists) {

	zval *fileName;

	zephir_fetch_params(0, 1, 0, &fileName);



	RETURN_BOOL((zephir_file_exists(fileName TSRMLS_CC) == SUCCESS));

}

