
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
 * Double-quoted string escape sequences must match PHP byte-for-byte.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2030
 */
ZEPHIR_INIT_CLASS(Stub_Issue2030)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2030, stub, issue2030, stub_issue2030_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2030, escapeChar)
{

	RETURN_STRING("\e[0;31m");
}

PHP_METHOD(Stub_Issue2030, unicodeAscii)
{

	RETURN_STRING("\101");
}

PHP_METHOD(Stub_Issue2030, unicode2Byte)
{

	RETURN_STRING("\303\251");
}

PHP_METHOD(Stub_Issue2030, unicode3Byte)
{

	RETURN_STRING("\342\202\254");
}

PHP_METHOD(Stub_Issue2030, unicode4Byte)
{

	RETURN_STRING("\360\237\230\200");
}

PHP_METHOD(Stub_Issue2030, hexFollowedByHex)
{

	RETURN_STRING("\101BC");
}

PHP_METHOD(Stub_Issue2030, maxCodepoint)
{

	RETURN_STRING("\364\217\277\277");
}

PHP_METHOD(Stub_Issue2030, quotesAndBackslashes)
{

	RETURN_STRING("a\"b\\c\360\237\222\251");
}

