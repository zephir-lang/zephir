
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/object.h"


/**
 * The `parent::` call-site workload of the inline cache benchmark (#2790).
 */
ZEPHIR_INIT_CLASS(Stub_Bench_SiteChild)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub\\Bench, SiteChild, stub, bench_sitechild, stub_bench_ce, stub_bench_sitechild_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Bench_SiteChild, siteParentLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *n_param = NULL, _0$$3;
	zend_long n, i, sum;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$3);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	i = 0;
	sum = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_CALL_METHOD_CACHED(&_0$$3, this_ptr, "siteParentOnce", &_zephir_call_cache[0]);
		zephir_check_call_status();
		sum += zephir_get_numberval(&_0$$3);
		i++;
	}
	RETURN_MM_LONG(sum);
}

PHP_METHOD(Stub_Bench_SiteChild, siteParentOnce)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_RETURN_CALL_PARENT_CACHED(stub_bench_sitechild_ce, getThis(), "returnConst", &_zephir_call_cache[0]);
	zephir_check_call_status();
	RETURN_MM();
}

