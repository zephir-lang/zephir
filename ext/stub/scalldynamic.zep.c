
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
#include "kernel/fcall.h"
#include "kernel/memory.h"


/**
 * Static Function calls
 */
ZEPHIR_INIT_CLASS(Stub_ScallDynamic)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, ScallDynamic, stub, scalldynamic, stub_scallparent_ce, stub_scalldynamic_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_ScallDynamic, testMethod1)
{

	RETURN_STRING("hello public");
}

PHP_METHOD(Stub_ScallDynamic, testMethod2)
{

	RETURN_STRING("hello protected");
}

PHP_METHOD(Stub_ScallDynamic, testMethod3)
{

	RETURN_STRING("hello private");
}

PHP_METHOD(Stub_ScallDynamic, selfDynamicCall1)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *methodName, methodName_sub;

	ZVAL_UNDEF(&methodName_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(methodName)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &methodName);
	return;
}

