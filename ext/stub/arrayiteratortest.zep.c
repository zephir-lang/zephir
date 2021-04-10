
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
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_ArrayIteratorTest)
{
	ZEPHIR_REGISTER_CLASS(Stub, ArrayIteratorTest, stub, arrayiteratortest, stub_arrayiteratortest_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_ArrayIteratorTest, test)
{
	zend_string *_3;
	zend_ulong _2;
	zval arr, k, v, *_0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&arr);
	object_init_ex(&arr, stub_arrayiterator_ce);
	ZEPHIR_CALL_METHOD(NULL, &arr, "__construct", NULL, 4);
	zephir_check_call_status();
	zephir_is_iterable(&arr, 0, "stub/arrayiteratortest.zep", 12);
	if (Z_TYPE_P(&arr) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(&arr), _2, _3, _0)
		{
			ZEPHIR_INIT_NVAR(&k);
			if (_3 != NULL) { 
				ZVAL_STR_COPY(&k, _3);
			} else {
				ZVAL_LONG(&k, _2);
			}
			ZEPHIR_INIT_NVAR(&v);
			ZVAL_COPY(&v, _0);
			ZEPHIR_CONCAT_VV(return_value, &k, &v);
			RETURN_MM();
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &arr, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_1, &arr, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_1)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&k, &arr, "key", NULL, 0);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(&v, &arr, "current", NULL, 0);
			zephir_check_call_status();
				ZEPHIR_CONCAT_VV(return_value, &k, &v);
				RETURN_MM();
			ZEPHIR_CALL_METHOD(NULL, &arr, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_INIT_NVAR(&v);
	ZEPHIR_INIT_NVAR(&k);
}

