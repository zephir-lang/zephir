
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
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Issue2635_Impl)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub\\Issue2635, Impl, stub, issue2635_impl, stub_issue2635_base_ce, stub_issue2635_impl_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2635_Impl, go)
{

	RETURN_STRING("inner");
}

PHP_METHOD(Stub_Issue2635_Impl, extra)
{

	RETURN_STRING("outer");
}

