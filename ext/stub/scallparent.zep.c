
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
ZEPHIR_INIT_CLASS(Stub_ScallParent)
{
	ZEPHIR_REGISTER_CLASS(Stub, ScallParent, stub, scallparent, stub_scallparent_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_ScallParent, testMethod1)
{

	RETURN_STRING("hello parent public");
}

PHP_METHOD(Stub_ScallParent, testMethod2)
{

	RETURN_STRING("hello parent protected");
}

PHP_METHOD(Stub_ScallParent, testCallStatic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_RETURN_CALL_STATIC("testmethodstatic", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_ScallParent, testMethodStatic)
{

	RETURN_STRING("hello ScallParent");
}

