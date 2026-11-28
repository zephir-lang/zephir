
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
#include "kernel/object.h"
#include "kernel/memory.h"


/**
 * A subclass may unset a protected property of its parent's instances.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2784
 */
ZEPHIR_INIT_CLASS(Stub_Issue2784Child)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2784Child, stub, issue2784child, stub_issue2784base_ce, stub_issue2784child_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2784Child, unsetParentProtected)
{
	zend_bool _0;
	zval *o, o_sub;

	ZVAL_UNDEF(&o_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("prot", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(o)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &o);
	zephir_unset_property(o, "prot");
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	_0 = zephir_isset_property_value_fast(o, _zephir_prop_0);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

