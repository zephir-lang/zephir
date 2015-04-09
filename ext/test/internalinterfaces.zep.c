
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
#include "ext/spl/spl_iterators.h"


ZEPHIR_INIT_CLASS(Test_InternalInterfaces) {

	ZEPHIR_REGISTER_CLASS(Test, InternalInterfaces, test, internalinterfaces, test_internalinterfaces_method_entry, 0);

	zend_class_implements(test_internalinterfaces_ce TSRMLS_CC, 1, spl_ce_Countable);
	return SUCCESS;

}

PHP_METHOD(Test_InternalInterfaces, count) {



}

