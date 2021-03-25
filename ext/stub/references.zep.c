
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
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_References) {

	ZEPHIR_REGISTER_CLASS(Stub, References, stub, references, stub_references_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_References, assignByRef) {

	zval *this_ptr = getThis();




}

