
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


ZEPHIR_INIT_CLASS(Test_ArrayObject) {

	ZEPHIR_REGISTER_CLASS_EX(Test, ArrayObject, test, arrayobject, spl_ce_ArrayObject, NULL, 0);

	return SUCCESS;

}

