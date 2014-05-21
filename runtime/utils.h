
#ifndef PHP_ZEPHIR_RUNTIME_UTILS_H
#define PHP_ZEPHIR_RUNTIME_UTILS_H

int _zephir_array_isset_quick_string(const zval *arr, const char *index, uint index_length, unsigned long key);
int _zephir_array_fetch_string(zval **return_value, zval *arr, const char *index, uint index_length TSRMLS_DC);

#endif
