
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
 * Class with dynamic new
 */
ZEPHIR_INIT_CLASS(Stub_Oo_OoDynamicB) {

	ZEPHIR_REGISTER_CLASS_EX(Stub\\Oo, OoDynamicB, stub, oo_oodynamicb, stub_oo_oodynamica_ce, stub_oo_oodynamicb_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_Oo_OoDynamicB, execute) {

	zval *this_ptr = getThis();



	RETURN_STRING("B");

}

PHP_METHOD(Stub_Oo_OoDynamicB, execute2) {

	zval *this_ptr = getThis();



	RETURN_STRING("BB");

}

