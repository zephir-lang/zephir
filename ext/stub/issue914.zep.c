
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
#include "kernel/fcall.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Issue914)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue914, stub, issue914, zephir_get_internal_ce(SL("mysqli")), stub_issue914_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue914, __construct)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_CALL_PARENT(NULL, stub_issue914_ce, getThis(), "__construct", NULL, 0);
	zephir_check_call_status();
}

