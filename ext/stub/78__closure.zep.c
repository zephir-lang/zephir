
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


ZEPHIR_INIT_CLASS(stub_78__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 78__closure, stub, 78__closure, stub_78__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	return SUCCESS;
}

PHP_METHOD(stub_78__closure, __invoke)
{
	zval *n, n_sub;

	ZVAL_UNDEF(&n_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &n);
	RETURN_BOOL(zephir_instance_of_ev(n, stub_namespaces_a_b_sub_ce));
}

