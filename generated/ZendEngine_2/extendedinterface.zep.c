
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Test_ExtendedInterface) {

	ZEPHIR_REGISTER_INTERFACE(Test, ExtendedInterface, test, extendedinterface, NULL);

	zend_class_implements(test_extendedinterface_ce TSRMLS_CC, 1, zend_ce_aggregate);
	zend_class_implements(test_extendedinterface_ce TSRMLS_CC, 1, spl_ce_Countable);
	return SUCCESS;

}

