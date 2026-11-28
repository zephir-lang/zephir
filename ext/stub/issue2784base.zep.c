
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


/**
 * Properties a class outside this hierarchy cannot reach.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2784
 */
ZEPHIR_INIT_CLASS(Stub_Issue2784Base)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2784Base, stub, issue2784base, stub_issue2784base_method_entry, 0);

	zend_declare_property_string(stub_issue2784base_ce, SL("prot"), "Issue2784Base::prot", ZEND_ACC_PROTECTED);
	zend_declare_property_string(stub_issue2784base_ce, SL("priv"), "Issue2784Base::priv", ZEND_ACC_PRIVATE);
	return SUCCESS;
}

/**
 * `unset this->priv` reaches the private property of this class on an
 * instance of a subclass as well.
 */
PHP_METHOD(Stub_Issue2784Base, unsetOwnPrivate)
{
	zend_bool _0;
	zval *this_ptr = getThis();

	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("priv", 4, 1);
	}
	zephir_unset_property(this_ptr, "priv");
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	_0 = zephir_isset_property_value_fast(this_ptr, _zephir_prop_0);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

/**
 * A closure reaches what the class it is written in reaches.
 */
PHP_METHOD(Stub_Issue2784Base, closureReadingPrivate)
{

	zephir_create_closure_ex(return_value, NULL, stub_0__closure_ce, SL("__invoke"));
	return;
}

