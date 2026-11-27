
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
#include "ext/spl/spl_array.h"
#include "kernel/object.h"


/**
 * An \ArrayObject with an array default is still created by \ArrayObject,
 * which allocates the storage its methods use.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2799
 */
ZEPHIR_INIT_CLASS(Stub_Issue2799ArrayObject)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2799ArrayObject, stub, issue2799arrayobject, spl_ce_ArrayObject, stub_issue2799arrayobject_method_entry, 0);

	{
		zval _zc0;
		array_init_size(&_zc0, 1);
		zephir_declare_property_array(stub_issue2799arrayobject_ce, SL("extra"), &_zc0, ZEND_ACC_PROTECTED);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2799ArrayObject, getExtra)
{

	RETURN_MEMBER(getThis(), "extra");
}

