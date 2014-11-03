
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
#include "kernel/memory.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_References) {

	ZEPHIR_REGISTER_CLASS(Test, References, test, references, test_references_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_References, assignByRef) {

	zval *a = NULL, *b = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_LONG(a, 100);
	ZEPHIR_MAKE_REFERENCE(b, a);
	ZEPHIR_INIT_NVAR(a);
	ZVAL_LONG(a, 50);
	RETURN_MM_BOOL(ZEPHIR_IS_EQUAL(a, b));

}

