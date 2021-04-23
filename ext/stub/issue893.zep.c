
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


/**
 * @issue https://github.com/zephir-lang/zephir/issues/893
 */
ZEPHIR_INIT_CLASS(Stub_Issue893)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue893, stub, issue893, zephir_get_internal_ce(SL("domdocument")), NULL, 0);

	return SUCCESS;
}

