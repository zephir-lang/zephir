
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_Issue2635_Outer)
{
	ZEPHIR_REGISTER_INTERFACE(Stub\\Issue2635, Outer, stub, issue2635_outer, stub_issue2635_outer_method_entry);

	zend_class_implements(stub_issue2635_outer_ce, 1, stub_issue2635_inner_ce);
	return SUCCESS;
}

ZEPHIR_DOC_METHOD(Stub_Issue2635_Outer, extra);
