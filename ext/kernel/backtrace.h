

#ifndef ZEPHIR_RELEASE

extern void zephir_print_backtrace(void);

#else

#ifndef zephir_print_backtrace
#define zephir_print_backtrace()
#endif

#endif
