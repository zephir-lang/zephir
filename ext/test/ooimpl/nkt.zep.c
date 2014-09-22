
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Test_OoImpl_Nkt) {

	ZEPHIR_REGISTER_INTERFACE(Test\\OoImpl, Nkt, test, ooimpl_nkt, NULL);

	zend_class_implements(test_ooimpl_nkt_ce TSRMLS_CC, 1, test_ooimpl_ovr_ce);
	return SUCCESS;

}

