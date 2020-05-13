
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_OoImpl_ZBeginning) {

	ZEPHIR_REGISTER_INTERFACE(Stub\\OoImpl, ZBeginning, stub, ooimpl_zbeginning, NULL);

	return SUCCESS;

}

