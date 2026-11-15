
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
#include "kernel/concat.h"
#include "kernel/memory.h"
#include "kernel/object.h"


/**
 * Renames the parameters it overrides, which PHP allows.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
ZEPHIR_INIT_CLASS(Stub_Issue2824Renamed)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2824Renamed, stub, issue2824renamed, stub_issue2824pair_ce, stub_issue2824renamed_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2824Renamed, pair)
{
	zval *first, first_sub, *second, second_sub;

	ZVAL_UNDEF(&first_sub);
	ZVAL_UNDEF(&second_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(first)
		Z_PARAM_ZVAL(second)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &first, &second);
	ZEPHIR_CONCAT_VSV(return_value, first, ";", second);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

