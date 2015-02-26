
#ifdef HAVE_CONFIG_H
#include "../../../../../../ext_config.h"
#endif

#include <php.h>
#include "../../../../../../php_ext.h"
#include "../../../../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Test_Oo_Extend_Db_Query_Placeholder_Exception) {

	ZEPHIR_REGISTER_CLASS_EX(Test\\Oo\\Extend\\Db\\Query\\Placeholder, Exception, test, oo_extend_db_query_placeholder_exception, test_oo_extend_db_query_exception_ce, NULL, 0);

	return SUCCESS;

}

