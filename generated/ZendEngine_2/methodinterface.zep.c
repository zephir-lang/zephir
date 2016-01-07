
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Test_MethodInterface) {

	ZEPHIR_REGISTER_INTERFACE(Test, MethodInterface, test, methodinterface, test_methodinterface_method_entry);

	return SUCCESS;

}

ZEPHIR_DOC_METHOD(Test_MethodInterface, testMethod);

