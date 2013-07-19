
/** Class Retrieving/Checking */
extern int zephir_class_exists(const zval *class_name, int autoload TSRMLS_DC);
extern void zephir_get_class(zval *result, zval *object, int lower TSRMLS_DC);
extern void zephir_get_class_ns(zval *result, zval *object, int lower TSRMLS_DC);
extern void zephir_get_ns_class(zval *result, zval *object, int lower TSRMLS_DC);
extern void zephir_get_called_class(zval *return_value TSRMLS_DC);
extern zend_class_entry *zephir_fetch_class(const zval *class_name TSRMLS_DC);

/** Class constants */
extern int zephir_get_class_constant(zval *return_value, zend_class_entry *ce, char *constant_name, unsigned int constant_length TSRMLS_DC);

/** Cloning/Instance of*/
extern int zephir_clone(zval *destiny, zval *obj TSRMLS_DC);
extern int zephir_instance_of(zval *result, const zval *object, const zend_class_entry *ce TSRMLS_DC);
extern int zephir_is_instance_of(zval *object, const char *class_name, unsigned int class_length TSRMLS_DC);

/** Method exists */
extern int zephir_method_exists(const zval *object, const zval *method_name TSRMLS_DC);
extern int zephir_method_exists_ex(const zval *object, const char *method_name, unsigned int method_len TSRMLS_DC);
extern int zephir_method_quick_exists_ex(const zval *object, const char *method_name, unsigned int method_len, unsigned long hash TSRMLS_DC);

/** Isset properties */
extern int zephir_isset_property(zval *object, const char *property_name, unsigned int property_length TSRMLS_DC);
extern int zephir_isset_property_quick(zval *object, const char *property_name, unsigned int property_length, unsigned long hash TSRMLS_DC);
extern int zephir_isset_property_zval(zval *object, const zval *property TSRMLS_DC);

/** Reading properties */
extern int zephir_read_property_this(zval **result, zval *object, char *property_name, unsigned int property_length, int silent TSRMLS_DC);
extern int zephir_read_property_this_quick(zval **result, zval *object, char *property_name, unsigned int property_length, unsigned long key, int silent TSRMLS_DC);
extern int zephir_read_property(zval **result, zval *object, char *property_name, unsigned int property_length, int silent TSRMLS_DC);
extern int zephir_read_property_zval(zval **result, zval *object, zval *property, int silent TSRMLS_DC);
extern int zephir_return_property(zval *return_value, zval *object, char *property_name, unsigned int property_length TSRMLS_DC);
extern int zephir_return_property_quick(zval *return_value, zval *object, char *property_name, unsigned int property_length, unsigned long key TSRMLS_DC);

/** Updating properties */
extern int zephir_update_property_this(zval *object, char *property_name, unsigned int property_length, zval *value TSRMLS_DC);
extern int zephir_update_property_long(zval *obj, char *property_name, unsigned int property_length, long value TSRMLS_DC);
extern int zephir_update_property_string(zval *object, char *property_name, unsigned int property_length, char *str, unsigned int str_length TSRMLS_DC);
extern int zephir_update_property_bool(zval *obj, char *property_name, unsigned int property_length, int value TSRMLS_DC);
extern int zephir_update_property_null(zval *obj, char *property_name, unsigned int property_length TSRMLS_DC);
extern int zephir_update_property_zval(zval *obj, char *property_name, unsigned int property_length, zval *value TSRMLS_DC);
extern int zephir_update_property_zval_zval(zval *obj, zval *property, zval *value TSRMLS_DC);
extern int zephir_update_property_empty_array(zend_class_entry *ce, zval *object, char *property, unsigned int property_length TSRMLS_DC);

/** Updating array properties */
extern int zephir_update_property_array(zval *object, char *property, unsigned int property_length, zval *index, zval *value TSRMLS_DC);
extern int zephir_update_property_array_string(zval *object, char *property, unsigned int property_length, char *index, unsigned int index_length, zval *value TSRMLS_DC);
extern int zephir_update_property_array_append(zval *object, char *property, unsigned int property_length, zval *value TSRMLS_DC);

/** Increment/Decrement properties */
extern int zephir_property_incr(zval *object, char *property_name, unsigned int property_length TSRMLS_DC);
extern int zephir_property_decr(zval *object, char *property_name, unsigned int property_length TSRMLS_DC);

/** Unset Array properties */
extern int zephir_unset_property_array(zval *object, char *property, unsigned int property_length, zval *index TSRMLS_DC);

/** Static properties */
extern int zephir_read_static_property(zval **result, const char *class_name, unsigned int class_length, char *property_name, unsigned int property_length TSRMLS_DC);
extern int zephir_update_static_property(const char *class_name, unsigned int class_length, char *name, unsigned int name_length, zval *value TSRMLS_DC);
extern int zephir_update_static_property_nts(zend_class_entry *ce, char *name, unsigned int name_length, zval *value TSRMLS_DC);

/** Create instances */
extern int zephir_create_instance(zval *return_value, const zval *class_name TSRMLS_DC);
extern int zephir_create_instance_params(zval *return_value, const zval *class_name, zval *params TSRMLS_DC);
