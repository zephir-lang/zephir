
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


ZEPHIR_INIT_CLASS(Stub_Issue1404)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue1404, stub, issue1404, stub_issue1404_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsingInteger70000)
{

	RETURN_BOOL(zephir_is_php_version(70000));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsingDouble70100)
{

	RETURN_BOOL(zephir_is_php_version(70100));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsingLong70000)
{

	RETURN_BOOL(zephir_is_php_version(21474836470000));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsingString70000)
{

	RETURN_BOOL(zephir_is_php_version(70000));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsingZero)
{

	RETURN_BOOL(zephir_is_php_version(0));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing50000)
{

	RETURN_BOOL(zephir_is_php_version(50000));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing50500)
{

	RETURN_BOOL(zephir_is_php_version(50500));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing50600)
{

	RETURN_BOOL(zephir_is_php_version(50600));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70101)
{

	RETURN_BOOL(zephir_is_php_version(70101));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70102)
{

	RETURN_BOOL(zephir_is_php_version(70102));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70103)
{

	RETURN_BOOL(zephir_is_php_version(70103));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70104)
{

	RETURN_BOOL(zephir_is_php_version(70104));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70105)
{

	RETURN_BOOL(zephir_is_php_version(70105));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70106)
{

	RETURN_BOOL(zephir_is_php_version(70106));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70107)
{

	RETURN_BOOL(zephir_is_php_version(70107));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70108)
{

	RETURN_BOOL(zephir_is_php_version(70108));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70109)
{

	RETURN_BOOL(zephir_is_php_version(70109));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70110)
{

	RETURN_BOOL(zephir_is_php_version(70110));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70111)
{

	RETURN_BOOL(zephir_is_php_version(70111));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70112)
{

	RETURN_BOOL(zephir_is_php_version(70112));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70113)
{

	RETURN_BOOL(zephir_is_php_version(70113));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70114)
{

	RETURN_BOOL(zephir_is_php_version(70114));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70115)
{

	RETURN_BOOL(zephir_is_php_version(70115));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70116)
{

	RETURN_BOOL(zephir_is_php_version(70116));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70117)
{

	RETURN_BOOL(zephir_is_php_version(70117));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70100)
{

	RETURN_BOOL(zephir_is_php_version(70100));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70200)
{

	RETURN_BOOL(zephir_is_php_version(70200));
}

PHP_METHOD(Stub_Issue1404, testIsPhpVersionUsing70300)
{

	RETURN_BOOL(zephir_is_php_version(70300));
}

