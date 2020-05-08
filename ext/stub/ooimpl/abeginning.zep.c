
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_OoImpl_ABeginning) {

	ZEPHIR_REGISTER_INTERFACE(Stub\\OoImpl, ABeginning, stub, ooimpl_abeginning, NULL);

	zend_class_implements(stub_ooimpl_abeginning_ce, 1, stub_ooimpl_zbeginning_ce);
	return SUCCESS;

}

