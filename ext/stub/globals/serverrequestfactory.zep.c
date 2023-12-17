
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
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Globals_ServerRequestFactory)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Globals, ServerRequestFactory, stub, globals_serverrequestfactory, stub_globals_serverrequestfactory_method_entry, 0);

	return SUCCESS;
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/1934
 */
PHP_METHOD(Stub_Globals_ServerRequestFactory, load)
{
	zephir_fcall_cache_entry *_1 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *server_param = NULL, *get_param = NULL, *post_param = NULL, *cookies_param = NULL, *files_param = NULL, _COOKIE, _FILES, _GET, _POST, _SERVER, _0, _2, _3, _4, _5;
	zval server, get, post, cookies, files, globalCookies, globalFiles, globalGet, globalPost, globalServer;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&server);
	ZVAL_UNDEF(&get);
	ZVAL_UNDEF(&post);
	ZVAL_UNDEF(&cookies);
	ZVAL_UNDEF(&files);
	ZVAL_UNDEF(&globalCookies);
	ZVAL_UNDEF(&globalFiles);
	ZVAL_UNDEF(&globalGet);
	ZVAL_UNDEF(&globalPost);
	ZVAL_UNDEF(&globalServer);
	ZVAL_UNDEF(&_COOKIE);
	ZVAL_UNDEF(&_FILES);
	ZVAL_UNDEF(&_GET);
	ZVAL_UNDEF(&_POST);
	ZVAL_UNDEF(&_SERVER);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 5)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY_OR_NULL(server)
		Z_PARAM_ARRAY_OR_NULL(get)
		Z_PARAM_ARRAY_OR_NULL(post)
		Z_PARAM_ARRAY_OR_NULL(cookies)
		Z_PARAM_ARRAY_OR_NULL(files)
	ZEND_PARSE_PARAMETERS_END();
	zephir_get_global(&_SERVER, SL("_SERVER"));
	zephir_get_global(&_POST, SL("_POST"));
	zephir_get_global(&_GET, SL("_GET"));
	zephir_get_global(&_FILES, SL("_FILES"));
	zephir_get_global(&_COOKIE, SL("_COOKIE"));
	zephir_fetch_params(1, 0, 5, &server_param, &get_param, &post_param, &cookies_param, &files_param);
	if (!server_param) {
		ZVAL_NULL(&server);
	} else {
		zephir_get_arrval(&server, server_param);
	}
	if (!get_param) {
		ZVAL_NULL(&get);
	} else {
		zephir_get_arrval(&get, get_param);
	}
	if (!post_param) {
		ZVAL_NULL(&post);
	} else {
		zephir_get_arrval(&post, post_param);
	}
	if (!cookies_param) {
		ZVAL_NULL(&cookies);
	} else {
		zephir_get_arrval(&cookies, cookies_param);
	}
	if (!files_param) {
		ZVAL_NULL(&files);
	} else {
		zephir_get_arrval(&files, files_param);
	}
	ZVAL_NULL(&globalCookies);
	array_init(&globalCookies);
	ZVAL_NULL(&globalFiles);
	array_init(&globalFiles);
	ZVAL_NULL(&globalGet);
	array_init(&globalGet);
	ZVAL_NULL(&globalPost);
	array_init(&globalPost);
	ZVAL_NULL(&globalServer);
	array_init(&globalServer);
	if (!(ZEPHIR_IS_EMPTY(&_COOKIE))) {
		ZEPHIR_CPY_WRT(&globalCookies, &_COOKIE);
	}
	if (!(ZEPHIR_IS_EMPTY(&_FILES))) {
		ZEPHIR_CPY_WRT(&globalFiles, &_FILES);
	}
	if (!(ZEPHIR_IS_EMPTY(&_GET))) {
		ZEPHIR_CPY_WRT(&globalGet, &_GET);
	}
	if (!(ZEPHIR_IS_EMPTY(&_POST))) {
		ZEPHIR_CPY_WRT(&globalPost, &_POST);
	}
	if (!(ZEPHIR_IS_EMPTY(&_SERVER))) {
		ZEPHIR_CPY_WRT(&globalServer, &_SERVER);
	}
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "checknullarray", &_1, 47, &server, &globalServer);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(&server, &_0);
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "checknullarray", &_1, 47, &files, &globalFiles);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(&files, &_2);
	ZEPHIR_CALL_METHOD(&_3, this_ptr, "checknullarray", &_1, 47, &cookies, &globalCookies);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(&cookies, &_3);
	ZEPHIR_CALL_METHOD(&_4, this_ptr, "checknullarray", &_1, 47, &get, &globalGet);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(&get, &_4);
	ZEPHIR_CALL_METHOD(&_5, this_ptr, "checknullarray", &_1, 47, &post, &globalPost);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(&post, &_5);
	zephir_create_array(return_value, 5, 0);
	zephir_array_update_string(return_value, SL("server"), &server, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(return_value, SL("files"), &files, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(return_value, SL("cookies"), &cookies, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(return_value, SL("get"), &get, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(return_value, SL("post"), &post, PH_COPY | PH_SEPARATE);
	return;
}

/**
 * Checks the source if it null and returns the super, otherwise the source
 * array
 */
PHP_METHOD(Stub_Globals_ServerRequestFactory, checkNullArray)
{
	zval super;
	zval *source, source_sub, *super_param = NULL;

	ZVAL_UNDEF(&source_sub);
	ZVAL_UNDEF(&super);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(source)
		Z_PARAM_ARRAY(super)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &source, &super_param);
	zephir_get_arrval(&super, super_param);
	if (UNEXPECTED(Z_TYPE_P(source) == IS_NULL)) {
		RETURN_CTOR(&super);
	}
	RETVAL_ZVAL(source, 1, 0);
	return;
}

