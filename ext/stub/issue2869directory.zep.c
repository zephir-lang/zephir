
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
#include "ext/spl/spl_directory.h"
#include "kernel/object.h"


/**
 * `self::` and `parent::` read the `FilesystemIterator` flags a Zephir
 * subclass of `RecursiveDirectoryIterator` inherits.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2869
 */
ZEPHIR_INIT_CLASS(Stub_Issue2869Directory)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2869Directory, stub, issue2869directory, spl_ce_RecursiveDirectoryIterator, stub_issue2869directory_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2869Directory, symlinks)
{

	RETURN_LONG(SPL_FILE_DIR_FOLLOW_SYMLINKS);
}

PHP_METHOD(Stub_Issue2869Directory, mask)
{

	RETURN_LONG(SPL_FILE_DIR_OTHERS_MASK);
}

PHP_METHOD(Stub_Issue2869Directory, skipDots)
{

	zephir_get_class_constant(return_value, stub_issue2869directory_ce, SL("SKIP_DOTS"));
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

