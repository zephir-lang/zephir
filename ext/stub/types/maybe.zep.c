
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
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Types_MayBe)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Types, MayBe, stub, types_maybe, stub_types_maybe_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Types_MayBe, gc)
{
	zval *maxlifetime_param = NULL;
	zend_long maxlifetime;
	zval *this_ptr = getThis();

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(maxlifetime)
	ZEND_PARSE_PARAMETERS_END();


	zephir_fetch_params_without_memory_grow(1, 0, &maxlifetime_param);
	maxlifetime = zephir_get_intval(maxlifetime_param);


	RETURN_LONG(1);
}

PHP_METHOD(Stub_Types_MayBe, gcFalse)
{
	zval *this_ptr = getThis();



	RETURN_BOOL(0);
}

