
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_Issue2165_IssueInterface) {

	ZEPHIR_REGISTER_INTERFACE(Stub\\Issue2165, IssueInterface, stub, issue2165_issueinterface, NULL);

	zephir_declare_class_constant_double(stub_issue2165_issueinterface_ce, SL("EPSILON"), 0.00000001);

	zend_class_implements(stub_issue2165_issueinterface_ce, 1, stub_issue2165_issueextendinterface_ce);
	return SUCCESS;

}

