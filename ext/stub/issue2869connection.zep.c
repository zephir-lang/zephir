
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
#include "ext/pdo/php_pdo_driver.h"
#include "kernel/object.h"


/**
 * `self::` and `parent::` read the PDO flags a Zephir subclass inherits.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2869
 */
ZEPHIR_INIT_CLASS(Stub_Issue2869Connection)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2869Connection, stub, issue2869connection, php_pdo_get_dbh_ce(), stub_issue2869connection_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2869Connection, unique)
{

	RETURN_LONG(((zend_long) PDO_FETCH_UNIQUE));
}

PHP_METHOD(Stub_Issue2869Connection, group)
{

	RETURN_LONG(((zend_long) PDO_FETCH_GROUP));
}

PHP_METHOD(Stub_Issue2869Connection, assoc)
{

	zephir_get_class_constant(return_value, stub_issue2869connection_ce, SL("FETCH_ASSOC"));
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

