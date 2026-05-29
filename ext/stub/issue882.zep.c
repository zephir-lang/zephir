
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
 * Dynamic instantiation must respect constructor visibility.
 *
 * @see https://github.com/zephir-lang/zephir/issues/882
 */
ZEPHIR_INIT_CLASS(Stub_Issue882)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue882, stub, issue882, stub_issue882_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue882, initClass)
{
	zend_class_entry *_1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval className_zv, _0;
	zend_string *className = NULL;

	ZVAL_UNDEF(&className_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(className)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&className_zv);
	ZVAL_STR_COPY(&className_zv, className);
	zephir_fetch_safe_class(&_0, &className_zv);
	_1 = zephir_fetch_class_str_ex(Z_STRVAL_P(&_0), Z_STRLEN_P(&_0), ZEND_FETCH_CLASS_AUTO);
	if(!_1) {
		RETURN_MM_NULL();
	}
	object_init_ex(return_value, _1);
	ZEPHIR_LAST_CALL_STATUS = zephir_check_constructor_access(return_value);
	zephir_check_call_status();
	if (zephir_has_constructor(return_value)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	RETURN_MM();
}

