
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
#include "kernel/object.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Stub_Issue2564)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2564, stub, issue2564, stub_issue2564_method_entry, 0);

	zend_declare_property_null(stub_issue2564_ce, SL("cipher"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_issue2564_ce, SL("useSigning"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_issue2564_ce, SL("padFactory"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2564, __construct)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_bool useSigning;
	zval cipher_zv, *useSigning_param = NULL, *padFactory = NULL, padFactory_sub, __$true, __$false, __$null;
	zend_string *cipher = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&cipher_zv);
	ZVAL_UNDEF(&padFactory_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_NULL(&__$null);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 3)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(cipher)
		Z_PARAM_BOOL(useSigning)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(padFactory, stub_issue2564padfactory_ce)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	if (ZEND_NUM_ARGS() > 1) {
		useSigning_param = ZEND_CALL_ARG(execute_data, 2);
	}
	if (ZEND_NUM_ARGS() > 2) {
		padFactory = ZEND_CALL_ARG(execute_data, 3);
	}
	if (!cipher) {
		cipher = zend_string_init(ZEND_STRL("aes-256-cfb"), 0);
		zephir_memory_observe(&cipher_zv);
		ZVAL_STR(&cipher_zv, cipher);
	} else {
		zephir_memory_observe(&cipher_zv);
	ZVAL_STR_COPY(&cipher_zv, cipher);
	}
	if (!useSigning_param) {
		useSigning = 1;
	} else {
		}
	if (!padFactory) {
		padFactory = &padFactory_sub;
		padFactory = &__$null;
	}
	zephir_update_property_zval(this_ptr, ZEND_STRL("cipher"), &cipher_zv);
	if (useSigning) {
		zephir_update_property_zval(this_ptr, ZEND_STRL("useSigning"), &__$true);
	} else {
		zephir_update_property_zval(this_ptr, ZEND_STRL("useSigning"), &__$false);
	}
	zephir_update_property_zval(this_ptr, ZEND_STRL("padFactory"), padFactory);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2564, getCipher)
{

	RETURN_MEMBER_TYPED(getThis(), "cipher", IS_STRING);
}

