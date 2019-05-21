
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


ZEPHIR_INIT_CLASS(Test_ArrayIteratorTest) {

	ZEPHIR_REGISTER_CLASS(Test, ArrayIteratorTest, test, arrayiteratortest, test_arrayiteratortest_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_ArrayIteratorTest, test) {

	zend_string *_3;
	zend_ulong _2;
	zval arr, k, v, *_0, _1;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_1);


	object_init_ex(&arr, test_arrayiterator_ce);ZEPHIR_MM_ADD_ENTRY(&arr);
	ZEPHIR_CALL_METHOD(NULL, &arr, "__construct", NULL, 4);
	zephir_check_call_status();
	zephir_is_iterable(&arr, 0, "test/arrayiteratortest.zep", 12);
	if (Z_TYPE_P(&arr) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(&arr), _2, _3, _0)
		{
			if (_3 != NULL) { 
				ZVAL_STR_COPY(&k, _3);
			} else {
				ZVAL_LONG(&k, _2);
			}
			ZEPHIR_CPY_WRT(&v, _0);
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

}

