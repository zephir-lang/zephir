
#ifndef PHP_ZEPHIR_RUNTIME_SYMTABLE_H
#define PHP_ZEPHIR_RUNTIME_SYMTABLE_H 1

zephir_symtable *zephir_symtable_new();
zephir_variable *zephir_symtable_add(int type, const char *name, unsigned int name_length, zephir_context *context);
int _zephir_symtable_fetch_string(zephir_variable **return_value, HashTable *arr, const char *index, uint index_length TSRMLS_DC);
zephir_variable *zephir_symtable_get_variable_for_read(zephir_symtable *symtable, const char *name, unsigned int name_length);
zephir_variable *zephir_symtable_get_variable_for_write(zephir_symtable *symtable, const char *name, unsigned int name_length);
int zephir_symtable_has(const char *name, unsigned int name_length, zephir_context *context);

#endif