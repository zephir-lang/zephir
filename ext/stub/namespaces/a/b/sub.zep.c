
#ifdef HAVE_CONFIG_H
#include "../../../../ext_config.h"
#endif

#include <php.h>
#include "../../../../php_ext.h"
#include "../../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_Namespaces_A_B_Sub) {

	ZEPHIR_REGISTER_CLASS(Stub\\Namespaces\\A\\B, Sub, stub, namespaces_a_b_sub, NULL, 0);

	zend_declare_property_null(stub_namespaces_a_b_sub_ce, SL("value"), ZEND_ACC_PUBLIC);

	return SUCCESS;

}

