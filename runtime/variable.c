
/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include "php_zephir.h"
#include "zephir.h"
#include "builder.h"
#include "utils.h"

void zephir_variable_incr_uses(zephir_variable *variable)
{

}

void zephir_variable_incr_mutations(zephir_variable *variable)
{

}

/**
 * Initializes a variant variable
 */
void zephir_variable_init_variant(zephir_variable *variable, zephir_context *context)
{

    if (variable->variant_inits > 0 || context->inside_cycle > 0) {
        zephir_build_memory_nalloc(context, variable->value_ref);
    } else {
    	zephir_build_memory_alloc(context, variable->value_ref);
    }

    variable->variant_inits++;
}
