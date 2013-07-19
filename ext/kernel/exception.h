

/** Exceptions */
#define ZEPHIR_THROW_EXCEPTION_STR(class_entry, message) zephir_throw_exception_string(class_entry, message, sizeof(message)-1, 1 TSRMLS_CC);
#define ZEPHIR_THROW_EXCEPTION_STRW(class_entry, message) zephir_throw_exception_string(class_entry, message, sizeof(message)-1, 0 TSRMLS_CC);
#define ZEPHIR_THROW_EXCEPTION_ZVAL(class_entry, message) zephir_throw_exception_zval(class_entry, message, 1 TSRMLS_CC);
#define ZEPHIR_THROW_EXCEPTION_ZVALW(class_entry, message) zephir_throw_exception_zval(class_entry, message, 0 TSRMLS_CC);

/** Throw Exceptions */
extern void zephir_throw_exception(zval *object TSRMLS_DC);
extern void zephir_throw_exception_string(zend_class_entry *ce, const char *message, zend_uint message_len, int restore_stack TSRMLS_DC);
extern void zephir_throw_exception_zval(zend_class_entry *ce, zval *message, int restore_stack TSRMLS_DC);
extern void zephir_throw_exception_internal(zval *exception TSRMLS_DC);

/** Catch Exceptions */
/* extern void zephir_try_execute(zval *success, zval *return_value, zval *call_object, zval *params, zval **exception TSRMLS_DC); */
