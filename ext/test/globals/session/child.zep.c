
#ifdef HAVE_CONFIG_H
#include "../../../ext_config.h"
#endif

#include <php.h>
#include "../../../php_ext.h"
#include "../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Test_Globals_Session_Child) {

	ZEPHIR_REGISTER_CLASS_EX(Test\\Globals\\Session, Child, test, globals_session_child, test_globals_session_base_ce, test_globals_session_child_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Globals_Session_Child, destroy) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();


	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "removesessiondata", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

