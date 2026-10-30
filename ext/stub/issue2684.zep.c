
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
#include "kernel/string.h"
#include "kernel/memory.h"
#include "kernel/object.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/2684
 *
 * `zephir_fast_explode()` and `zephir_fast_explode_str()` accepted only
 * strings and answered anything else with a warning and an empty string.
 * PHP coerces int, float, bool, null and Stringable arguments and throws
 * `TypeError` for the rest; the limit follows `Z_PARAM_LONG` in the same way.
 *
 * One method per path through Zephir\Optimizers\FunctionCall\ExplodeOptimizer.
 */
ZEPHIR_INIT_CLASS(Stub_Issue2684)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2684, stub, issue2684, stub_issue2684_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2684, explodeNoLimit)
{
	zval *delimiter, delimiter_sub, *source, source_sub;

	ZVAL_UNDEF(&delimiter_sub);
	ZVAL_UNDEF(&source_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(delimiter)
		Z_PARAM_ZVAL(source)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &delimiter, &source);
	zephir_fast_explode(return_value, delimiter, source, NULL);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2684, explodeStr)
{
	zval *source, source_sub;

	ZVAL_UNDEF(&source_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(source)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &source);
	zephir_fast_explode_str(return_value, SL(","), source, NULL);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2684, explodeLimit)
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
	zephir_fast_explode(return_value, delimiter, source, limit);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2684, explodeStrLimit)
{
	zval *source, source_sub, *limit, limit_sub;

	ZVAL_UNDEF(&source_sub);
	ZVAL_UNDEF(&limit_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(source)
		Z_PARAM_ZVAL(limit)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &source, &limit);
	zephir_fast_explode_str(return_value, SL(","), source, limit);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2684, explodeConstLimit)
{
	zval *source, source_sub, _0;

	ZVAL_UNDEF(&source_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(source)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &source);
	ZVAL_LONG(&_0, 2);
	zephir_fast_explode_str(return_value, SL(","), source, &_0);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

/**
 * The statement after a throwing explode() must not run.
 */
PHP_METHOD(Stub_Issue2684, stopsAfterThrow)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *delimiter, delimiter_sub, *source, source_sub, parts;

	ZVAL_UNDEF(&delimiter_sub);
	ZVAL_UNDEF(&source_sub);
	ZVAL_UNDEF(&parts);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(delimiter)
		Z_PARAM_ZVAL(source)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &delimiter, &source);
	ZEPHIR_INIT_VAR(&parts);
	zephir_fast_explode(&parts, delimiter, source, NULL);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	php_printf("%s", "reached");
	RETURN_CCTOR(&parts);
}

/**
 * Inside a try the throw jumps to the catch.
 */
PHP_METHOD(Stub_Issue2684, catchesInTry)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *delimiter, delimiter_sub, *source, source_sub, parts, e, _0;

	ZVAL_UNDEF(&delimiter_sub);
	ZVAL_UNDEF(&source_sub);
	ZVAL_UNDEF(&parts);
	ZVAL_UNDEF(&e);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(delimiter)
		Z_PARAM_ZVAL(source)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &delimiter, &source);
	/* try_start_1: */

		ZEPHIR_INIT_VAR(&parts);
		zephir_fast_explode(&parts, delimiter, source, NULL);
		if (UNEXPECTED(EG(exception))) {
			goto try_end_1;
		}
		php_printf("%s", "reached");
		RETURN_CCTOR(&parts);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_0);
		ZVAL_OBJ(&_0, EG(exception));
		Z_ADDREF_P(&_0);
		if (zephir_is_instance_of(&_0, SL("TypeError"))) {
			zend_clear_exception();
			ZEPHIR_CPY_WRT(&e, &_0);
			zephir_get_class(return_value, &e, 0);
			RETURN_MM();
		} else {
			if (zephir_is_instance_of(&_0, SL("ValueError"))) {
				zend_clear_exception();
				ZEPHIR_CPY_WRT(&e, &_0);
				zephir_get_class(return_value, &e, 0);
				RETURN_MM();
			} else {
				ZEPHIR_MM_RESTORE();
				return;
			}
		}
	}
}

