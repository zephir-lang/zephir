
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Test_OoImpl_ABeginning) {

	ZEPHIR_REGISTER_INTERFACE(Test\\OoImpl, ABeginning, test, ooimpl_abeginning, NULL);

	zend_class_implements(test_ooimpl_abeginning_ce TSRMLS_CC, 1, test_ooimpl_zbeginning_ce);
	return SUCCESS;

}

