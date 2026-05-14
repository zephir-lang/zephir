
#ifdef HAVE_CONFIG_H
#include "../../../../../ext_config.h"
#endif

#include <php.h>
#include "../../../../../php_ext.h"
#include "../../../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_Oo_Extend_Db_Query_Exception)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub\\Oo\\Extend\\Db\\Query, Exception, stub, oo_extend_db_query_exception, stub_oo_extend_db_exception_ce, NULL, 0);

	return SUCCESS;
}

