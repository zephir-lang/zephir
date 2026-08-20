
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"
#include "ext/json/php_json.h"
#include "ext/spl/spl_observer.h"


ZEPHIR_INIT_CLASS(Stub_ExtendedInterface)
{
	ZEPHIR_REGISTER_INTERFACE(Stub, ExtendedInterface, stub, extendedinterface, NULL);

	zend_class_implements(stub_extendedinterface_ce, 1, zend_ce_aggregate);
	zend_class_implements(stub_extendedinterface_ce, 1, zend_ce_countable);
	zend_class_implements(stub_extendedinterface_ce, 1, php_json_serializable_ce);
	zend_class_implements(stub_extendedinterface_ce, 1, spl_ce_SplObserver);
	return SUCCESS;
}

