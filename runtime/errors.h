
#define zephir_error(location, ...) { \
	char *message = emalloc(sizeof(char) * 64); \
	snprintf(message, 64, __VA_ARGS__); \
	zephir_error_ex(message, location); \
}

void zephir_error_ex(char *message, zval *location TSRMLS_DC);