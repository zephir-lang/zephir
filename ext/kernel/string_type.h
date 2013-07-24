
typedef struct {
	char *str;
	size_t length;
	size_t size;
} zephir_str;

#define zephir_string_assign(str_p, str, length) { \
	if (!str_p) { \
		str_p = emalloc(48); \
		str_p->length = length; \
		str_p->size = 48; \
	} else  { \
		if (str_p->size < length) { \
			efree(str_p); \
		} \
	}
	memcpy(str_p->str, str, length);
	str_p->str[length] = '\0'; \
}