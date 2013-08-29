
#ifndef ZEPHIR_MEMORY_H
#define ZEPHIR_MEMORY_H 1

#define ZEPHIR_MEMORY_FRAME_CHUNK 16

/* Variable Tracking */
extern void zephir_init_nvar(zval **var TSRMLS_DC);
extern void zephir_cpy_wrt(zval **dest, zval *var TSRMLS_DC);
extern void zephir_cpy_wrt_ctor(zval **dest, zval *var TSRMLS_DC);

/* Memory Frames */
extern int ZEPHIR_FASTCALL zephir_memory_grow_stack(TSRMLS_D);
extern int ZEPHIR_FASTCALL zephir_memory_restore_stack(TSRMLS_D);

extern int ZEPHIR_FASTCALL zephir_memory_observe(zval **var TSRMLS_DC);
extern int ZEPHIR_FASTCALL zephir_memory_remove(zval **var TSRMLS_DC);
extern int ZEPHIR_FASTCALL zephir_memory_alloc(zval **var TSRMLS_DC);
extern int ZEPHIR_FASTCALL zephir_memory_alloc_pnull(zval **var TSRMLS_DC);

extern int ZEPHIR_FASTCALL zephir_clean_shutdown_stack(TSRMLS_D);
extern int ZEPHIR_FASTCALL zephir_clean_restore_stack(TSRMLS_D);

extern void zephir_value_dtor(zval *zvalue ZEND_FILE_LINE_DC);

/* Virtual symbol tables */
extern void zephir_create_symbol_table(TSRMLS_D);
/*extern void zephir_restore_symbol_table(TSRMLS_D);*/
extern void zephir_clean_symbol_tables(TSRMLS_D);

/** Export symbols to active symbol table */
extern int zephir_set_symbol(zval *key_name, zval *value TSRMLS_DC);
extern int zephir_set_symbol_str(char *key_name, unsigned int key_length, zval *value TSRMLS_DC);

extern void ZEPHIR_FASTCALL zephir_copy_ctor(zval *destiny, zval *origin);

#define ZEPHIR_MM_GROW() zephir_memory_grow_stack(TSRMLS_C)
#define ZEPHIR_MM_RESTORE() zephir_memory_restore_stack(TSRMLS_C)

/* Memory macros */
#define ZEPHIR_ALLOC_ZVAL(z) \
	ALLOC_ZVAL(z); INIT_PZVAL(z); ZVAL_NULL(z);

#define ZEPHIR_INIT_VAR(z) \
	zephir_memory_alloc(&z TSRMLS_CC);

#define ZEPHIR_SINIT_VAR(z) \
	INIT_PZVAL(&z); \
	ZVAL_NULL(&z);

#define ZEPHIR_SINIT_NVAR(z)

#define ZEPHIR_INIT_NVAR(z)\
	if (z) { \
		if (Z_REFCOUNT_P(z) > 1) { \
			Z_DELREF_P(z); \
			ALLOC_ZVAL(z); \
			Z_SET_REFCOUNT_P(z, 1); \
			Z_UNSET_ISREF_P(z); \
			ZVAL_NULL(z); \
		} else {\
			zval_ptr_dtor(&z); \
			ZEPHIR_ALLOC_ZVAL(z); \
		} \
	} else { \
		zephir_memory_alloc(&z TSRMLS_CC); \
	}

#define ZEPHIR_INIT_NVAR_PNULL(z)\
	if (z) { \
		if (Z_REFCOUNT_P(z) > 1) { \
			Z_DELREF_P(z); \
			if (Z_REFCOUNT_P(z) >= 1) { \
				zval_copy_ctor(z); \
			} \
			ALLOC_ZVAL(z); \
			Z_SET_REFCOUNT_P(z, 1); \
			Z_UNSET_ISREF_P(z); \
			ZVAL_NULL(z); \
		} else {\
			ZVAL_NULL(z); \
			zval_ptr_dtor(&z); \
			ZEPHIR_ALLOC_ZVAL(z); \
		} \
	} else { \
		zephir_memory_alloc_pnull(&z TSRMLS_CC); \
	}

/* only removes the value body of the zval */
#define ZEPHIR_INIT_LNVAR(z)\
	if (z) { \
		if (Z_REFCOUNT_P(z) > 1) { \
			Z_DELREF_P(z); \
			ALLOC_ZVAL(z); \
			Z_SET_REFCOUNT_P(z, 1); \
			Z_UNSET_ISREF_P(z); \
			ZVAL_NULL(z); \
		} else {\
			zephir_value_dtor(z ZEND_FILE_LINE_CC); \
			ZVAL_NULL(z); \
		} \
	} else { \
		zephir_memory_alloc(&z TSRMLS_CC); \
	}

#define ZEPHIR_CPY_WRT(d, v) \
	if (d) { \
		if (Z_REFCOUNT_P(d) > 0) { \
			zval_ptr_dtor(&d); \
		} \
	} else { \
		zephir_memory_observe(&d TSRMLS_CC); \
	} \
	Z_ADDREF_P(v); \
	d = v;

#define ZEPHIR_CPY_WRT_CTOR(d, v) \
	if (d) { \
		if (Z_REFCOUNT_P(d) > 0) { \
			zval_ptr_dtor(&d); \
		} \
	} else { \
		zephir_memory_observe(&d TSRMLS_CC); \
	} \
	ALLOC_ZVAL(d); \
	*d = *v; \
	zval_copy_ctor(d); \
	Z_SET_REFCOUNT_P(d, 1); \
	Z_UNSET_ISREF_P(d);

/* */
#define ZEPHIR_OBS_VAR(z) \
	zephir_memory_observe(&z TSRMLS_CC);

#define ZEPHIR_OBS_NVAR(z)\
	if (z) { \
		if (Z_REFCOUNT_P(z) > 1) { \
			Z_DELREF_P(z); \
		} else {\
			zval_ptr_dtor(&z); \
		} \
	} else { \
		zephir_memory_observe(&z TSRMLS_CC); \
	}

#define ZEPHIR_ALLOC_ZVAL_MM(z) \
	ZEPHIR_ALLOC_ZVAL(z); \
	zephir_memory_observe(&z TSRMLS_CC);

#define ZEPHIR_SEPARATE_ARRAY(a) \
	{ \
		if (Z_REFCOUNT_P(a) > 1) { \
			zval *new_zv; \
			Z_DELREF_P(a); \
			ALLOC_ZVAL(new_zv); \
			INIT_PZVAL_COPY(new_zv, a); \
			a = new_zv; \
			zval_copy_ctor(new_zv); \
		} \
	}

#define ZEPHIR_SEPARATE(z) \
	{ \
		zval *orig_ptr = z; \
		if (Z_REFCOUNT_P(orig_ptr) > 1) { \
			Z_DELREF_P(orig_ptr); \
			ALLOC_ZVAL(z); \
			*z = *orig_ptr; \
			zval_copy_ctor(z); \
			Z_SET_REFCOUNT_P(z, 1); \
			Z_UNSET_ISREF_P(z); \
		} \
	}

#define ZEPHIR_SEPARATE_NMO(z) \
	{\
		zval *orig_ptr = z;\
		if (Z_REFCOUNT_P(orig_ptr) > 1) {\
			Z_DELREF_P(orig_ptr);\
			ALLOC_ZVAL(z);\
			*z = *orig_ptr;\
			zval_copy_ctor(z);\
			Z_SET_REFCOUNT_P(z, 1);\
			Z_UNSET_ISREF_P(z);\
		}\
	}

#define ZEPHIR_SEPARATE_PARAM(z) \
	{\
		zval *orig_ptr = z;\
		if (Z_REFCOUNT_P(orig_ptr) > 1) {\
			ALLOC_ZVAL(z);\
			zephir_memory_observe(&z TSRMLS_CC);\
			*z = *orig_ptr;\
			zval_copy_ctor(z);\
			Z_SET_REFCOUNT_P(z, 1);\
			Z_UNSET_ISREF_P(z);\
		}\
	}

#define ZEPHIR_SEPARATE_PARAM_NMO(z) { \
		zval *orig_ptr = z; \
		if (Z_REFCOUNT_P(orig_ptr) > 1) { \
			ALLOC_ZVAL(z); \
			*z = *orig_ptr; \
			zval_copy_ctor(z); \
			Z_SET_REFCOUNT_P(z, 1); \
			Z_UNSET_ISREF_P(z); \
		} \
	}

#define ZEPHIR_OBSERVE_VAR(var) \
	if (!var) { \
		zephir_memory_observe(&var TSRMLS_CC); \
	} else { \
		zval_ptr_dtor(&var); \
	}

#endif
