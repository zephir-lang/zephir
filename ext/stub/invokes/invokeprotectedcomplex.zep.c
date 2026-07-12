
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


ZEPHIR_INIT_CLASS(Stub_Invokes_InvokeProtectedComplex)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub\\Invokes, InvokeProtectedComplex, stub, invokes_invokeprotectedcomplex, stub_invokes_abstractinvokercomplex_ce, stub_invokes_invokeprotectedcomplex_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Invokes_InvokeProtectedComplex, __construct)
{
	zval text_zv;
	zend_string *text = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&text_zv);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("text", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(text)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&text_zv, text);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 39, &text_zv);
}

