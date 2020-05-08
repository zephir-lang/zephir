
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_ExtendedInterface) {

	ZEPHIR_REGISTER_INTERFACE(Stub, ExtendedInterface, stub, extendedinterface, NULL);

	zend_class_implements(stub_extendedinterface_ce, 1, zend_ce_aggregate);
	zend_class_implements(stub_extendedinterface_ce, 1, spl_ce_Countable);
	return SUCCESS;

}

