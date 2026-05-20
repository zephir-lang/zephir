
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
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Globals_Post)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Globals, Post, stub, globals_post, stub_globals_post_method_entry, 0);

	return SUCCESS;
}

/**
 * Tests "Invalid superglobal for _POST"
 *
 * @issue https://github.com/zephir-lang/zephir/issues/1623
 */
PHP_METHOD(Stub_Globals_Post, hasValue)
{
	zval name_zv, _POST;
	zend_string *name = NULL;

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&_POST);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	zephir_get_global(&_POST, SL("_POST"));
	ZVAL_STR(&name_zv, name);
	RETURN_BOOL(zephir_array_isset_value(&_POST, &name_zv));
}

