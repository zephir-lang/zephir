
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Oo_AbstractStatic) {

	ZEPHIR_REGISTER_CLASS(Stub\\Oo, AbstractStatic, stub, oo_abstractstatic, stub_oo_abstractstatic_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	return SUCCESS;

}

PHP_METHOD(Stub_Oo_AbstractStatic, getCalledClass) {

	zval *this_ptr = getThis();



	zephir_get_called_class(return_value);
	return;

}

