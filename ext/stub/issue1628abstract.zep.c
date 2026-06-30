
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
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Issue1628Abstract)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue1628Abstract, stub, issue1628abstract, stub_issue1628abstract_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue1628Abstract, concrete)
{

	RETURN_STRING("concrete");
}

PHP_METHOD(Stub_Issue1628Abstract, mustImplement)
{
}

