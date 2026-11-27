
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


ZEPHIR_INIT_CLASS(stub_0__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 0__closure, stub, 0__closure, stub_0__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	return SUCCESS;
}

PHP_METHOD(stub_0__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, _0;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("priv", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(o, zephir_get_internal_ce(SL("stub\\issue2784base")))
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &o);
	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, o, _zephir_prop_0, 0, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_0);
}

