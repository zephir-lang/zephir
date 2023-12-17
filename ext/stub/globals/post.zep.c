
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
	zval *name_param = NULL, _POST;
	zval name;

	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&_POST);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	zephir_get_global(&_POST, SL("_POST"));
	zephir_fetch_params(1, 1, 0, &name_param);
	zephir_get_strval(&name, name_param);
	RETURN_MM_BOOL(zephir_array_isset(&_POST, &name));
}

