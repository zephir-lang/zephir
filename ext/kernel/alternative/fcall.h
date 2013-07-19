
/* Executor Pointers */
#ifdef ZTS
# define PHALCON_EG(v) TSRMG(executor_globals_id, zend_executor_globals *, v)
# define PHALCON_VEG ((zend_executor_globals *) (*((void ***) tsrm_ls))[TSRM_UNSHUFFLE_RSRC_ID(executor_globals_id)])
#else
# define PHALCON_EG(v) (executor_globals.v)
# define PHALCON_VEG (&executor_globals)
#endif

extern int phalcon_alt_call_user_method(zend_class_entry *ce, zval **object_pp, char *method_name, unsigned int method_len, zval *retval_ptr, zend_uint param_count, zval *params[], unsigned long method_key TSRMLS_DC);
int phalcon_alt_call_user_method_ex(zend_class_entry *ce, zval **object_pp, char *method_name, unsigned int method_len, zval **retval_ptr_ptr, zend_uint param_count, zval **params[], unsigned long method_key TSRMLS_DC);
int phalcon_alt_call_method(zend_fcall_info *fci, zend_class_entry *ce, unsigned long hash_key, char *method_name, unsigned int method_len, unsigned long method_key TSRMLS_DC);