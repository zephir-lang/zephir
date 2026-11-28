
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
 * An exception with array defaults is still created by \Exception, which
 * records where it was created.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2799
 */
ZEPHIR_INIT_CLASS(Stub_Issue2799Exception)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2799Exception, stub, issue2799exception, zend_ce_exception, stub_issue2799exception_method_entry, 0);

	{
		zval _zc0;
		array_init_size(&_zc0, 1);
		zephir_declare_property_array(stub_issue2799exception_ce, SL("errors"), &_zc0, ZEND_ACC_PROTECTED);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 3);
		add_next_index_long(&_zc0, 1);
		add_next_index_long(&_zc0, 2);
		zephir_declare_property_array(stub_issue2799exception_ce, SL("codes"), &_zc0, ZEND_ACC_PROTECTED);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2799Exception, getErrors)
{

	RETURN_MEMBER(getThis(), "errors");
}

PHP_METHOD(Stub_Issue2799Exception, getCodes)
{

	RETURN_MEMBER(getThis(), "codes");
}

