
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
#include "kernel/memory.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"


ZEPHIR_INIT_CLASS(Stub_Closures) {

	ZEPHIR_REGISTER_CLASS(Stub, Closures, stub, closures, stub_closures_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_Closures, simple1) {

	zval *this_ptr = getThis();


	zephir_create_closure_ex(return_value, NULL, stub_1__closure_ce, SL("__invoke"));
	return;

}

PHP_METHOD(Stub_Closures, simple2) {

	zval *this_ptr = getThis();


	zephir_create_closure_ex(return_value, NULL, stub_2__closure_ce, SL("__invoke"));
	return;

}

PHP_METHOD(Stub_Closures, simple3) {

	zval *this_ptr = getThis();


	zephir_create_closure_ex(return_value, NULL, stub_3__closure_ce, SL("__invoke"));
	return;

}

PHP_METHOD(Stub_Closures, simple4) {

	zval *this_ptr = getThis();


	zephir_create_closure_ex(return_value, NULL, stub_4__closure_ce, SL("__invoke"));
	return;

}

PHP_METHOD(Stub_Closures, simple5) {

	zval *this_ptr = getThis();


	zephir_create_closure_ex(return_value, NULL, stub_5__closure_ce, SL("__invoke"));
	return;

}

PHP_METHOD(Stub_Closures, arrow1) {

	zval *this_ptr = getThis();


	zephir_create_closure_ex(return_value, NULL, stub_6__closure_ce, SL("__invoke"));
	return;

}

PHP_METHOD(Stub_Closures, arrow2) {

	zval *this_ptr = getThis();


	zephir_create_closure_ex(return_value, NULL, stub_7__closure_ce, SL("__invoke"));
	return;

}

PHP_METHOD(Stub_Closures, testUseCommand) {

	zval _0;
	zend_long abc;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	abc = 1;
	zephir_create_closure_ex(return_value, NULL, stub_8__closure_ce, SL("__invoke"));
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, abc);
	zephir_update_static_property_ce(stub_8__closure_ce, ZEND_STRL("abc"), &_0);
	return;

}

PHP_METHOD(Stub_Closures, issue1860) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *abc_param = NULL;
	zval abc;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&abc);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &abc_param);

	ZEPHIR_OBS_COPY_OR_DUP(&abc, abc_param);


	zephir_create_closure_ex(return_value, NULL, stub_9__closure_ce, SL("__invoke"));
	zephir_update_static_property_ce(stub_9__closure_ce, ZEND_STRL("abc"), &abc);
	RETURN_MM();

}

