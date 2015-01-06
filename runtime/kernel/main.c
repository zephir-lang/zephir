
/*
  +------------------------------------------------------------------------+
  | Zephir Language                                                        |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2015 Zephir Team (http://www.zephir-lang.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@zephir-lang.com so we can send you a copy immediately.      |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@zephir-lang.com>                     |
  |          Eduar Carvajal <eduar@zephir-lang.com>                        |
  +------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include "php_zephir.h"
#include "zephir.h"

#include "kernel/main.h"
#include "kernel/exceptions.h"

/**
 * Parses method parameters with minimum overhead
 */
int zephirt_fetch_parameters(int num_args TSRMLS_DC, int required_args, int optional_args, ...)
{
	va_list va;
	int arg_count = (int) (zend_uintptr_t) *(zend_vm_stack_top(TSRMLS_C) - 1);
	zval **arg, **p;
	int i;

	if (num_args < required_args || (num_args > (required_args + optional_args))) {
		zephirt_throw_exception_string(spl_ce_BadMethodCallException, SL("Wrong number of parameters") TSRMLS_CC);
		return FAILURE;
	}

	if (num_args > arg_count) {
		zephirt_throw_exception_string(spl_ce_BadMethodCallException, SL("Could not obtain parameters for parsing") TSRMLS_CC);
		return FAILURE;
	}

	if (!num_args) {
		return SUCCESS;
	}

	va_start(va, optional_args);

	i = 0;
	while (num_args-- > 0) {

		arg = (zval **) (zend_vm_stack_top(TSRMLS_C) - 1 - (arg_count - i));

		p = va_arg(va, zval **);
		*p = *arg;

		i++;
	}

	va_end(va);

	return SUCCESS;
}
