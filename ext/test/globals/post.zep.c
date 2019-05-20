
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
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Test_Globals_Post) {

	ZEPHIR_REGISTER_CLASS(Test\\Globals, Post, test, globals_post, test_globals_post_method_entry, 0);

	return SUCCESS;

}

/**
 * Tests "Invalid superglobal for _POST"
 *
 * @issue https://github.com/phalcon/zephir/issues/1623
 */
PHP_METHOD(Test_Globals_Post, hasValue) {

	zval *name_param = NULL, _POST;
	zval name;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&_POST);

	zephir_get_global(&_POST, SL("_POST"));
	zephir_fetch_params(1, 1, 0, &name_param);

	zephir_get_strval(&name, name_param);


	RETURN_MM_BOOL(zephir_array_isset(&_POST, &name));

}

