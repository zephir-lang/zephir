
#ifndef ZEPHIR_ARRAY_H
#define ZEPHIR_ARRAY_H 1

/** Combined isset/fetch */
int zephir_array_isset_fetch(zval **fetched, const zval *arr, zval *index);
int zephir_array_isset_quick_string_fetch(zval **fetched, zval *arr, char *index, uint index_length, unsigned long key);
int zephir_array_isset_string_fetch(zval **fetched, zval *arr, char *index, uint index_length);
int zephir_array_isset_long_fetch(zval **fetched, zval *arr, unsigned long index);

/** Check for index existence */
extern int ZEPHIR_FASTCALL zephir_array_isset(const zval *arr, zval *index);
extern int ZEPHIR_FASTCALL zephir_array_isset_long(const zval *arr, unsigned long index);
extern int ZEPHIR_FASTCALL zephir_array_isset_string(const zval *arr, char *index, uint index_length);

/** Fast index existence checking */
extern int ZEPHIR_FASTCALL zephir_array_isset_quick_string(const zval *arr, char *index, uint index_length, unsigned long key);

/** Unset existing indexes */
extern int ZEPHIR_FASTCALL zephir_array_unset(zval **arr, zval *index, int flags);
extern int ZEPHIR_FASTCALL zephir_array_unset_long(zval **arr, unsigned long index, int flags);
extern int ZEPHIR_FASTCALL zephir_array_unset_string(zval **arr, char *index, uint index_length, int flags);

/** Append elements to arrays */
extern int zephir_array_append(zval **arr, zval *value, int separate);
extern int zephir_array_append_long(zval **arr, long value, int separate);
extern int zephir_array_append_string(zval **arr, char *value, uint value_length, int separate);

/** Modify arrays */
extern int zephir_array_update_zval(zval **arr, zval *index, zval **value, int flags);
extern int zephir_array_update_zval_bool(zval **arr, zval *index, int value, int flags);
extern int zephir_array_update_zval_long(zval **arr, zval *index, long value, int flags);
extern int zephir_array_update_zval_string(zval **arr, zval *index, char *value, uint value_length, int flags);

extern int zephir_array_update_string(zval **arr, char *index, uint index_length, zval **value, int flags);
extern int zephir_array_update_string_bool(zval **arr, char *index, uint index_length, int value, int flags);
extern int zephir_array_update_string_long(zval **arr, char *index, uint index_length, long value, int flags);
extern int zephir_array_update_string_string(zval **arr, char *index, uint index_length, char *value, uint value_length, int flags);

extern int zephir_array_update_long(zval **arr, unsigned long index, zval **value, int flags);
extern int zephir_array_update_long_string(zval **arr, unsigned long index, char *value, uint value_length, int flags);
extern int zephir_array_update_long_long(zval **arr, unsigned long index, long value, int flags);
extern int zephir_array_update_long_bool(zval **arr, unsigned long index, int value, int flags);

/** Update/Append two dimension arrays */
extern void zephir_array_update_multi_2(zval **config, zval *index1, zval *index2, zval **value, int flags);
extern void zephir_array_update_string_multi_2(zval **arr, zval *index1, char *index2, uint index2_length, zval **value, int flags);
extern void zephir_array_update_long_long_multi_2(zval **arr, long index1, long index2, zval **value, int flags);
extern void zephir_array_update_long_string_multi_2(zval **arr, long index1, char *index2, uint index2_length, zval **value, int flags);
extern void zephir_array_update_append_multi_2(zval **arr, zval *index1, zval *value, int flags);

/** Update/Append three dimension arrays */
extern void zephir_array_update_zval_string_append_multi_3(zval **arr, zval *index1, char *index2, uint index2_length, zval **value, int flags);
extern void zephir_array_update_zval_zval_zval_multi_3(zval **arr, zval *index1, zval *index2, zval *index3, zval **value, int flags);
extern void zephir_array_update_string_zval_zval_multi_3(zval **arr, zval *index1, zval *index2, char *index3, uint index3_length, zval **value, int flags);
extern void zephir_array_update_zval_string_string_multi_3(zval **arr, zval *index1, char *index2, uint index2_length, char *index3, uint index3_length, zval **value, int flags);

/** Fetch items from arrays */
extern int zephir_array_fetch(zval **return_value, zval *arr, zval *index, int silent);
extern int zephir_array_fetch_string(zval **return_value, zval *arr, char *index, uint index_length, int silent);
extern int zephir_array_fetch_long(zval **return_value, zval *arr, unsigned long index, int silent);

/** Merge+Append */
extern void zephir_merge_append(zval *left, zval *values);

/* Traversing Arays */
extern void zephir_array_get_current(zval *return_value, zval *array);
extern void zephir_array_next(zval *array);

/* In Array */
extern int zephir_fast_in_array(zval *needle, zval *haystack TSRMLS_DC);

/** Fast Array Merge */
extern void zephir_fast_array_merge(zval *return_value, zval **array1, zval **array2 TSRMLS_DC);

/** Recursive merge */
extern void zephir_array_merge_recursive_n(zval **a1, zval *a2);

#endif
