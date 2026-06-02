
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


/**
 * @issue https://github.com/zephir-lang/zephir/issues/1628
 */
ZEPHIR_INIT_CLASS(Stub_Issue1628)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue1628, stub, issue1628, stub_issue1628abstract_ce, stub_issue1628_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue1628, mustImplement)
{

	RETURN_STRING("implemented");
}

