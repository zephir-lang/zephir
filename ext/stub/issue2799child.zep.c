
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


/**
 * Redeclares one of its parent's array defaults and adds its own.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2799
 */
ZEPHIR_INIT_CLASS(Stub_Issue2799Child)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2799Child, stub, issue2799child, stub_issue2799_ce, NULL, 0);

	{
		zval _zc0;
		array_init_size(&_zc0, 2);
		add_next_index_stringl(&_zc0, SL("child"));
		zephir_declare_property_array(stub_issue2799child_ce, SL("literal"), &_zc0, ZEND_ACC_PROTECTED);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 2);
		zval _zc1;
		array_init_size(&_zc1, 1);
		add_assoc_zval_ex(&_zc0, SL("x"), &_zc1);
		zephir_declare_property_array(stub_issue2799child_ce, SL("extra"), &_zc0, ZEND_ACC_PUBLIC);
	}

	return SUCCESS;
}

