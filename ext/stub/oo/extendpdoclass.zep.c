
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
#include "ext/pdo/php_pdo_driver.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Oo_ExtendPdoClass)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub\\Oo, ExtendPdoClass, stub, oo_extendpdoclass, php_pdo_get_dbh_ce(), stub_oo_extendpdoclass_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Oo_ExtendPdoClass, __construct)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval dsn_zv, username_zv, password_zv, *attrs = NULL, attrs_sub, _1;
	zend_string *dsn = NULL, *username = NULL, *password = NULL;

	ZVAL_UNDEF(&dsn_zv);
	ZVAL_UNDEF(&username_zv);
	ZVAL_UNDEF(&password_zv);
	ZVAL_UNDEF(&attrs_sub);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 4)
		Z_PARAM_STR(dsn)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR_OR_NULL(username)
		Z_PARAM_STR_OR_NULL(password)
		Z_PARAM_ZVAL(attrs)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	if (ZEND_NUM_ARGS() > 3) {
		attrs = ZEND_CALL_ARG(execute_data, 4);
	}
	zephir_memory_observe(&dsn_zv);
	ZVAL_STR_COPY(&dsn_zv, dsn);
	if (!username) {
		ZEPHIR_INIT_VAR(&username_zv);
	} else {
		zephir_memory_observe(&username_zv);
	ZVAL_STR_COPY(&username_zv, username);
	}
	if (!password) {
		ZEPHIR_INIT_VAR(&password_zv);
	} else {
		zephir_memory_observe(&password_zv);
	ZVAL_STR_COPY(&password_zv, password);
	}
	if (!attrs) {
		attrs = &attrs_sub;
		ZEPHIR_INIT_VAR(attrs);
		array_init(attrs);
	} else {
		ZEPHIR_SEPARATE_PARAM(attrs);
	}
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "Stub\\PdoStatement");
	zephir_array_fast_append(&_0, &_1);
	zephir_array_update_long(attrs, 13, &_0, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_CALL_PARENT(NULL, stub_oo_extendpdoclass_ce, getThis(), "__construct", NULL, 0, &dsn_zv, &username_zv, &password_zv, attrs);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();
}

