
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"


/**
 * Class with constructor + params
 */
ZEPHIR_INIT_CLASS(Test_Oo_OoConstructParams) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, OoConstructParams, oo_ooconstructparams, test_oo_ooconstructparams_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Oo_OoConstructParams, __construct) {

	zval *a, *b;


	zephir_fetch_params(0, 2, 0, &a, &b);


}

