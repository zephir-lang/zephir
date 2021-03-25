
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
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Stub_Globals_Server) {

	ZEPHIR_REGISTER_CLASS(Stub\\Globals, Server, stub, globals_server, stub_globals_server_method_entry, 0);

	return SUCCESS;

}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1961
 */
PHP_METHOD(Stub_Globals_Server, f1) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval _SERVER, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_SERVER);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();
	zephir_get_global(&_SERVER, SL("_SERVER"));

	zephir_array_fetch_string(&_0, &_SERVER, SL("PHP_SELF"), PH_NOISY | PH_READONLY, "stub/globals/server.zep", 10);
	zend_print_zval(&_0, 0);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "f2", NULL, 0);
	zephir_check_call_status();
	zephir_array_fetch_string(&_1, &_SERVER, SL("PHP_SELF"), PH_NOISY | PH_READONLY, "stub/globals/server.zep", 12);
	zend_print_zval(&_1, 0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Stub_Globals_Server, f2) {

	zval _SERVER, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_SERVER);
	ZVAL_UNDEF(&_0);


	zephir_get_global(&_SERVER, SL("_SERVER"));

	zephir_array_fetch_string(&_0, &_SERVER, SL("SCRIPT_NAME"), PH_NOISY | PH_READONLY, "stub/globals/server.zep", 17);
	zend_print_zval(&_0, 0);

}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1970
 */
PHP_METHOD(Stub_Globals_Server, check) {

	zval _SERVER, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_SERVER);
	ZVAL_UNDEF(&_0);


	zephir_get_global(&_SERVER, SL("_SERVER"));

	zephir_array_fetch_string(&_0, &_SERVER, SL("HTTP_USER_AGENT"), PH_NOISY | PH_READONLY, "stub/globals/server.zep", 25);
	RETURN_CTORW(&_0);

}

