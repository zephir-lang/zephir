
#ifndef ZEPHIR_RELEASE

#define PHV(v) zephir_vdump(v)
#define PHPR(v) zephir_print_r(v)

typedef struct _zephir_debug_entry {
	char *class_name;
	char *method_name;
	int lineno;
	struct _zephir_debug_entry *prev;
	struct _zephir_debug_entry *next;
} zephir_debug_entry;

extern int zephir_start_debug();
extern int zephir_stop_debug();

extern int zephir_print_r(zval *userval TSRMLS_DC);
extern int zephir_vdump(zval *uservar TSRMLS_DC);
extern int zephir_debug_assign(char *name, zval *value TSRMLS_DC);
extern int zephir_vpdump(const zval **uservar TSRMLS_DC);
extern int zephir_dump_ce(zend_class_entry *ce TSRMLS_DC);
extern int zephir_class_debug(zval *val TSRMLS_DC);

extern int zephir_debug_backtrace_internal();
extern int zephir_debug_str(char *what, char *message);
extern int zephir_debug_long(char *what, uint vlong);
extern int zephir_debug_screen(char *message);

extern int zephir_step_over(char *message);
extern int zephir_step_into(char *message);
extern int zephir_step_out(char *message);

extern int zephir_step_into_entry(char *class_name, char *method_name, int lineno);
extern int zephir_step_out_entry();

extern int zephir_debug_method_call(zval *obj, char *method_name TSRMLS_DC);
extern int zephir_debug_vdump(char *preffix, zval *value TSRMLS_DC);
extern int zephir_debug_param(zval *param TSRMLS_DC);

extern int zephir_error_space();
extern int zephir_debug_space();

extern FILE *zephir_log;
extern int zephir_debug_trace;
extern zephir_debug_entry *start;
extern zephir_debug_entry *active;

#endif
