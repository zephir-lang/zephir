
#ifdef HAVE_CONFIG_H
#include "../../../../ext_config.h"
#endif

#include <php.h>
#include "../../../../php_ext.h"
#include "../../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "ext/spl/spl_dllist.h"


ZEPHIR_INIT_CLASS(Test_Oo_Extend_Spl_Queue) {

	ZEPHIR_REGISTER_CLASS_EX(Test\\Oo\\Extend\\Spl, Queue, test, oo_extend_spl_queue, spl_ce_SplQueue, NULL, 0);

	return SUCCESS;

}

