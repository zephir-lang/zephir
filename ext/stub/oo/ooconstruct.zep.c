
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


/**
 * Class with constructor
 */
ZEPHIR_INIT_CLASS(Stub_Oo_OoConstruct) {

	ZEPHIR_REGISTER_CLASS(Stub\\Oo, OoConstruct, stub, oo_ooconstruct, stub_oo_ooconstruct_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_Oo_OoConstruct, __construct) {

	zval *this_ptr = getThis();




}

