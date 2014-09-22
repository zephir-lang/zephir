
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


ZEPHIR_INIT_CLASS(Test_OoImpl_Bar) {

	ZEPHIR_REGISTER_CLASS(Test\\OoImpl, Bar, test, ooimpl_bar, NULL, 0);

	zend_class_implements(test_ooimpl_bar_ce TSRMLS_CC, 1, test_ooimpl_ovr_ce);
	return SUCCESS;

}

