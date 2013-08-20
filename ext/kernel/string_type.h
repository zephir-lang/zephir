
#ifndef ZEPHIR_STRING_TYPE_H
#define ZEPHIR_STRING_TYPE_H 1

typedef struct {
	char *str;
	size_t len;
	size_t size;
} zephir_str;

#define zephir_str_init_assign(str_p, d_length) { \
	if (str_p) { \
		efree((str_p)->str); \
	} \
	(str_p) = emalloc(sizeof(zephir_str)); \
	(str_p)->str = emalloc(d_length); \
	(str_p)->len = d_length; \
	(str_p)->size = d_length; \
}

#define zephir_str_init_append(str_p, d_length) { \
	if (!str_p) { \
		(str_p) = emalloc(sizeof(zephir_str)); \
		(str_p)->str = emalloc(48); \
		(str_p)->len = 0; \
		(str_p)->size = 48; \
	} else { \
		if (str_p->size <= ((str_p)->len + d_length + 1)) { \
			(str_p)->str = erealloc((str_p)->str, (str_p)->size + 48); \
			(str_p)->size += 48; \
		} \
	} \
}

#define zephir_str_assign(str_p, d_str, d_length) { \
	zephir_str_init_assign(str_p, d_length); \
	memcpy((str_p)->str, d_str, d_length); \
	(str_p)->str[d_length] = '\0'; \
}

#define zephir_str_append(str_p, d_str, d_length)

#define zephir_str_assign_long(str_p, lvalue)

#define zephir_str_append_long(str_p, lvalue)

#define zephir_str_append_char(str_p, cvalue) { \
	zephir_str_init_append(str_p, 1); \
	(str_p)->str[(str_p)->len] = cvalue; \
	(str_p)->str[(str_p)->len + 1] = '\0'; \
	(str_p)->len++; \
}


#endif