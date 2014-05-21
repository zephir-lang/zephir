
#ifndef PHP_ZEPHIR_RUNTIME_VARIABLE_H
#define PHP_ZEPHIR_RUNTIME_VARIABLE_H

void zephir_variable_incr_uses(zephir_variable *variable);
void zephir_variable_incr_mutations(zephir_variable *variable);
void zephir_variable_init_variant(zephir_variable *variable, zephir_context *context);

#endif