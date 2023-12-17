
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


ZEPHIR_INIT_CLASS(Stub_Openssl)
{
	ZEPHIR_REGISTER_CLASS(Stub, Openssl, stub, openssl, stub_openssl_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Openssl, randomPseudoBytes)
{
	zval *length_param = NULL, _0;
	zend_long length, ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(length)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &length_param);
	ZVAL_LONG(&_0, length);
	ZEPHIR_RETURN_CALL_FUNCTION("openssl_random_pseudo_bytes", NULL, 76, &_0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Openssl, randomPseudoBytesLoop)
{
	zend_bool _0;
	zval arr, i, _3$$3, _4$$3;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1, _2;
	zephir_fcall_cache_entry *_5 = NULL;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_NULL(&i);
	ZVAL_LONG(&i, 0);
	ZVAL_NULL(&arr);
	array_init(&arr);
	_2 = 10;
	_1 = 1;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			ZVAL_LONG(&_3$$3, 10);
			ZEPHIR_CALL_FUNCTION(&_4$$3, "openssl_random_pseudo_bytes", &_5, 76, &_3$$3);
			zephir_check_call_status();
			zephir_array_append(&arr, &_4$$3, PH_SEPARATE, "stub/openssl.zep", 16);
		}
	}
	RETURN_CCTOR(&arr);
}

PHP_METHOD(Stub_Openssl, opensslEncrypt)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *data_param = NULL, _0, _1, _2, _3;
	zval data;

	ZVAL_UNDEF(&data);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(data)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &data_param);
	zephir_get_strval(&data, data_param);
	ZVAL_NULL(&_0);
	ZVAL_STRING(&_0, "AES-256-CBC");
	ZVAL_NULL(&_1);
	ZVAL_STRING(&_1, "passphrase");
	ZVAL_LONG(&_2, 1);
	ZVAL_NULL(&_3);
	ZVAL_STRING(&_3, "1234567890123456");
	ZEPHIR_RETURN_CALL_FUNCTION("openssl_encrypt", NULL, 77, &data, &_0, &_1, &_2, &_3);
	zephir_check_call_status();
	return;
}

