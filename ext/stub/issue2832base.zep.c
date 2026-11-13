
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
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/object.h"


/**
 * `fetch` from inside the class that declares the properties, where PHP's
 * `isset()` can see them.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2832
 */
ZEPHIR_INIT_CLASS(Stub_Issue2832Base)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2832Base, stub, issue2832base, stub_issue2832base_method_entry, 0);

	zend_declare_property_string(stub_issue2832base_ce, SL("prot"), "prot", ZEND_ACC_PROTECTED);
	zend_declare_property_string(stub_issue2832base_ce, SL("priv"), "priv", ZEND_ACC_PRIVATE);
	zend_declare_property_null(stub_issue2832base_ce, SL("nul"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2832Base, fetchName)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, *n, n_sub, __$true, __$false, c;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&n_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&c);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(o)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &o, &n);
	zephir_memory_observe(&c);
	_0 = zephir_fetch_property_zval(&c, o, n, PH_SILENT_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (_0) {
		zephir_create_array(return_value, 2, 0);
		zephir_array_fast_append(return_value, &__$true);
		zephir_array_fast_append(return_value, &c);
		RETURN_MM();
	}
	zephir_create_array(return_value, 2, 0);
	zephir_array_fast_append(return_value, &__$false);
	zephir_array_fast_append(return_value, &c);
	RETURN_MM();
}

