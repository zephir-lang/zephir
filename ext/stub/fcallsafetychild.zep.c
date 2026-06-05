
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


ZEPHIR_INIT_CLASS(Stub_FcallSafetyChild)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, FcallSafetyChild, stub, fcallsafetychild, stub_fcallsafetybase_ce, stub_fcallsafetychild_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_FcallSafetyChild, poly)
{

	RETURN_STRING("child");
}

