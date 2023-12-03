
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
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Stub_ArraySearch)
{
	ZEPHIR_REGISTER_CLASS(Stub, ArraySearch, stub, arraysearch, stub_arraysearch_method_entry, 0);

	return SUCCESS;
}

/** @issue https://github.com/zephir-lang/zephir/issues/1609 */
PHP_METHOD(Stub_ArraySearch, simpleSearch)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zend_bool strict;
	zval haystack;
	zval *needle, needle_sub, *haystack_param = NULL, *strict_param = NULL, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&needle_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&haystack);
	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_ZVAL(needle)
		Z_PARAM_ARRAY(haystack)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(strict)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 1, &needle, &haystack_param, &strict_param);
	zephir_get_arrval(&haystack, haystack_param);
	if (!strict_param) {
		strict = 0;
	} else {
		strict = zephir_get_boolval(strict_param);
	}


	ZVAL_BOOL(&_0, (strict ? 1 : 0));
	ZEPHIR_RETURN_CALL_FUNCTION("array_search", NULL, 6, needle, &haystack, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

/** @issue https://github.com/zephir-lang/zephir/issues/1609 */
PHP_METHOD(Stub_ArraySearch, searchUsingArrayInsideZephir)
{
	zval _0, pos;
	zval itoA64;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&itoA64);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&pos);


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&itoA64);
	zephir_create_array(&itoA64, 9, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, ".");
	zephir_array_fast_append(&itoA64, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "/");
	zephir_array_fast_append(&itoA64, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "0");
	zephir_array_fast_append(&itoA64, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "1");
	zephir_array_fast_append(&itoA64, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "2");
	zephir_array_fast_append(&itoA64, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "3");
	zephir_array_fast_append(&itoA64, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "4");
	zephir_array_fast_append(&itoA64, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "value");
	zephir_array_fast_append(&itoA64, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "OtherValue");
	zephir_array_fast_append(&itoA64, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "value");
	ZEPHIR_CALL_FUNCTION(&pos, "array_search", NULL, 6, &_0, &itoA64);
	zephir_check_call_status();
	RETURN_CCTOR(&pos);
}

