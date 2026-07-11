
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


/**
 * Subclass of PropertyCache. Inherits getPub()/setPub() unchanged so the
 * parent's cached access site runs with this_ptr of a different class entry
 * (the engine must re-validate ce, not blindly reuse the parent's cache).
 *
 * @see https://github.com/zephir-lang/zephir/issues/1884 (property access)
 */
ZEPHIR_INIT_CLASS(Stub_PropertyCacheChild)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, PropertyCacheChild, stub, propertycachechild, stub_propertycache_ce, stub_propertycachechild_method_entry, 0);

	zend_declare_property_long(stub_propertycachechild_ce, SL("childProp"), 100, ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(Stub_PropertyCacheChild, getChildProp)
{

	RETURN_MEMBER(getThis(), "childProp");
}

/**
 * Reads an inherited property through a method declared on the child.
 */
PHP_METHOD(Stub_PropertyCacheChild, getInheritedPub)
{

	RETURN_MEMBER(getThis(), "pub");
}

