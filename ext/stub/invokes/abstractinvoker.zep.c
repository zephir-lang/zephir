
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
#include "kernel/fcall.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Invokes_AbstractInvoker)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub\\Invokes, AbstractInvoker, stub, invokes_abstractinvoker, stub_invokes_abstractprotected_ce, stub_invokes_abstractinvoker_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	return SUCCESS;
}

PHP_METHOD(Stub_Invokes_AbstractInvoker, __toString)
{
	zval _0;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("text"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "renderarrayelements", NULL, 0, &_0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Invokes_AbstractInvoker, __invoke)
{
	zval *this_ptr = getThis();
	RETURN_THISW();
}

