
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
#include "kernel/operators.h"


/**
 * Only scalar defaults, so the engine allocates the static members of this
 * class on their first access in each request.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2805
 */
ZEPHIR_INIT_CLASS(Stub_Issue2805Request)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2805Request, stub, issue2805request, stub_issue2805request_method_entry, 0);

	zend_declare_property_string(stub_issue2805request_ce, SL("counter"), "", ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	zend_declare_property_long(stub_issue2805request_ce, SL("pad"), 0, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2805Request, bump)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_TLS zephir_static_property_cache _zephir_sprop_cache[3];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_static_property_cached(&_0, stub_issue2805request_ce, stub_issue2805request_ce, SL("counter"), &_zephir_sprop_cache[0], PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_concat_self_str(&_0, SL("x"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_static_property_cached(stub_issue2805request_ce, stub_issue2805request_ce, SL("counter"), &_zephir_sprop_cache[1], &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_read_static_property_cached(&_1, stub_issue2805request_ce, stub_issue2805request_ce, SL("counter"), &_zephir_sprop_cache[2], PH_NOISY_CC | PH_READONLY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&_1);
}

