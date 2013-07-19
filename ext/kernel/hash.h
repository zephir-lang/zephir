
int zephir_hash_exists(const HashTable *ht, const char *arKey, uint nKeyLength);
int zephir_hash_quick_exists(const HashTable *ht, const char *arKey, uint nKeyLength, ulong h);
int zephir_hash_find(const HashTable *ht, const char *arKey, uint nKeyLength, void **pData);
int zephir_hash_quick_find(const HashTable *ht, const char *arKey, uint nKeyLength, ulong h, void **pData);
void zephir_get_current_key(zval **key, const HashTable *hash_table, HashPosition *hash_position TSRMLS_DC);
zval zephir_get_current_key_w(const HashTable *hash_table, HashPosition *hash_position);
int zephir_has_numeric_keys(const zval *data);
