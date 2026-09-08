
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
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/memory.h"
#include "kernel/object.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/2674
 *
 * `zephir_fast_explode()` and `zephir_fast_explode_str()` handed every limit
 * straight to `php_explode()`, which PHP itself calls only when `limit > 1`.
 * A negative limit belongs to `php_explode_negative_limit()` and a limit of 0
 * or 1 returns the whole subject as a single element, so every limit <= 1
 * produced the `limit == 2` result. An empty separator is a `ValueError` in
 * PHP; here it spun forever, because `zend_memnstr()` answers a zero-length
 * needle with the haystack instead of NULL.
 *
 * One method per path through Zephir\Optimizers\FunctionCall\ExplodeOptimizer.
 */
ZEPHIR_INIT_CLASS(Stub_Issue2674)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2674, stub, issue2674, stub_issue2674_method_entry, 0);

	return SUCCESS;
}

/**
 * Literal separator, runtime limit: `zephir_fast_explode_str()`.
 */
PHP_METHOD(Stub_Issue2674, explodeStrLimit)
{
	zval *source, source_sub, *limit, limit_sub;

	ZVAL_UNDEF(&source_sub);
	ZVAL_UNDEF(&limit_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(source)
		Z_PARAM_ZVAL(limit)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &source, &limit);
	zephir_fast_explode_str(return_value, SL(","), source, zephir_get_intval(limit) );
	return;
}

/**
 * Runtime separator, runtime limit: `zephir_fast_explode()`.
 */
PHP_METHOD(Stub_Issue2674, explodeLimit)
{
	zval *delimiter, delimiter_sub, *source, source_sub, *limit, limit_sub;

	ZVAL_UNDEF(&delimiter_sub);
	ZVAL_UNDEF(&source_sub);
	ZVAL_UNDEF(&limit_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(delimiter)
		Z_PARAM_ZVAL(source)
		Z_PARAM_ZVAL(limit)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(3, 0, &delimiter, &source, &limit);
	zephir_fast_explode(return_value, delimiter, source, zephir_get_intval(limit) );
	return;
}

/**
 * No limit argument at all, so the optimizer supplies `ZEND_LONG_MAX`.
 */
PHP_METHOD(Stub_Issue2674, explodeNoLimit)
{
	zval *delimiter, delimiter_sub, *source, source_sub;

	ZVAL_UNDEF(&delimiter_sub);
	ZVAL_UNDEF(&source_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(delimiter)
		Z_PARAM_ZVAL(source)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &delimiter, &source);
	zephir_fast_explode(return_value, delimiter, source, ZEND_LONG_MAX);
	return;
}

/**
 * Literal separator and a compile-time constant limit, which the
 * optimizer emits as a C literal instead of `zephir_get_intval()`.
 */
PHP_METHOD(Stub_Issue2674, explodeConstLimit)
{
	zval *source, source_sub;

	ZVAL_UNDEF(&source_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(source)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &source);
	zephir_fast_explode_str(return_value, SL(","), source, -2 );
	return;
}

