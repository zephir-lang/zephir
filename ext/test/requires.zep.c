
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
#include "kernel/memory.h"
#include "kernel/require.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Test_Requires) {

	ZEPHIR_REGISTER_CLASS(Test, Requires, test, requires, test_requires_method_entry, 0);

	zend_declare_property_null(test_requires_ce, SL("content"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Requires, requireExternal1) {

	zval *path, path_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &path);



	ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(&_0);
	if (zephir_require_zval_ret(&_0, path TSRMLS_CC) == FAILURE) {
		RETURN_MM_NULL();
	}
	RETURN_CCTOR(&_0);

}

PHP_METHOD(Test_Requires, requireExternal2) {

	zval *path, path_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&path_sub);

	zephir_fetch_params(0, 1, 0, &path);



	if (zephir_require_zval(path TSRMLS_CC) == FAILURE) {
		RETURN_NULL();
	}
	RETURN_BOOL(1);

}

PHP_METHOD(Test_Requires, requireExternal3) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *path, path_sub, external3, _0, _1$$3;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&external3);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1$$3);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &path);



	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_GET_CONSTANT(&_0, "PHP_MAJOR_VERSION");
	if (ZEPHIR_IS_LONG(&_0, 5)) {
		ZEPHIR_INIT_VAR(&_1$$3);
		zephir_create_symbol_table(TSRMLS_C);
		
	}
	ZEPHIR_INIT_VAR(&external3);
	object_init_ex(&external3, test_requires_external3_ce);
	if (zephir_has_constructor(&external3 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, &external3, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	ZEPHIR_CALL_METHOD(NULL, &external3, "req", NULL, 57, path, this_ptr);
	zephir_check_call_status();
	RETURN_MM_MEMBER(getThis(), "content");

}

PHP_METHOD(Test_Requires, setContent) {

	zval *content, content_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&content_sub);

	zephir_fetch_params(0, 1, 0, &content);



	zephir_update_property_zval(this_ptr, SL("content"), content);

}

