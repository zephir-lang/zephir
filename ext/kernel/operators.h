
/** Strict comparing */
#define ZEPHIR_IS_LONG(op1, op2) zephir_compare_strict_long(op1, op2 TSRMLS_CC)
#define ZEPHIR_IS_STRING(op1, op2) zephir_compare_strict_string(op1, op2, strlen(op2))

/** strict boolean comparison */
#define ZEPHIR_IS_FALSE(var) Z_TYPE_P(var) == IS_BOOL && !Z_BVAL_P(var)
#define ZEPHIR_IS_TRUE(var) Z_TYPE_P(var) == IS_BOOL && Z_BVAL_P(var)

#define ZEPHIR_IS_NOT_FALSE(var) Z_TYPE_P(var) != IS_BOOL || (Z_TYPE_P(var) == IS_BOOL && Z_BVAL_P(var))
#define ZEPHIR_IS_NOT_TRUE(var) Z_TYPE_P(var) != IS_BOOL || (Z_TYPE_P(var) == IS_BOOL && !Z_BVAL_P(var))

/** SQL null empty **/
#define ZEPHIR_IS_EMPTY(var) Z_TYPE_P(var) == IS_NULL || (Z_TYPE_P(var) == IS_STRING && !Z_STRLEN_P(var))
#define ZEPHIR_IS_NOT_EMPTY(var) !(Z_TYPE_P(var) == IS_NULL || (Z_TYPE_P(var) == IS_STRING && !Z_STRLEN_P(var)))

/** Is scalar */
#define ZEPHIR_IS_SCALAR(var) !(Z_TYPE_P(var) == IS_NULL || Z_TYPE_P(var) == IS_ARRAY || Z_TYPE_P(var) == IS_OBJECT || Z_TYPE_P(var) == IS_RESOURCE)
#define ZEPHIR_IS_NOT_SCALAR(var) Z_TYPE_P(var) == IS_NULL || Z_TYPE_P(var) == IS_ARRAY || Z_TYPE_P(var) == IS_OBJECT || Z_TYPE_P(var) == IS_RESOURCE

/** Equals/Identical */
#define ZEPHIR_IS_EQUAL(op1, op2) zephir_is_equal(op1, op2 TSRMLS_CC)
#define ZEPHIR_IS_IDENTICAL(op1, op2) zephir_is_identical(op1, op2 TSRMLS_CC)

/** Greater/Smaller equals */
#define ZEPHIR_LE(op1, op2) zephir_less_equal(op1, op2 TSRMLS_CC)
#define ZEPHIR_LE_LONG(op1, op2) zephir_less_equal_long(op1, op2 TSRMLS_CC)
#define ZEPHIR_GE(op1, op2) zephir_greater_equal(op1, op2 TSRMLS_CC)
#define ZEPHIR_GE_LONG(op1, op2) zephir_greater_equal_long(op1, op2 TSRMLS_CC)
#define ZEPHIR_LT(op1, op2) zephir_less(op1, op2 TSRMLS_CC)
#define ZEPHIR_LT_LONG(op1, op2) zephir_less_long(op1, op2 TSRMLS_CC)
#define ZEPHIR_GT(op1, op2) zephir_greater(op1, op2 TSRMLS_CC)
#define ZEPHIR_GT_LONG(op1, op2) zephir_greater_long(op1, op2 TSRMLS_CC)

#if PHP_VERSION_ID < 50400
#define zephir_increment(var) increment_function(var)
#else
#define zephir_increment(var) fast_increment_function(var)
#endif

#if PHP_VERSION_ID < 50400
#define zephir_decrement(var) decrement_function(var)
#else
#define zephir_decrement(var) fast_decrement_function(var)
#endif

/** Operator functions */
extern int zephir_add_function(zval *result, zval *op1, zval *op2 TSRMLS_DC);
extern int zephir_and_function(zval *result, zval *left, zval *right);

extern void zephir_concat_self(zval **left, zval *right TSRMLS_DC);
extern void zephir_concat_self_str(zval **left, const char *right, int right_length TSRMLS_DC);

/** Strict comparing */
extern int zephir_compare_strict_string(zval *op1, const char *op2, int op2_length);
extern int zephir_compare_strict_long(zval *op1, long op2 TSRMLS_DC);

extern void zephir_cast(zval *result, zval *var, zend_uint type);

extern long zephir_get_intval(const zval *op);
extern double zephir_get_doubleval(const zval *op);
extern zend_bool zephir_get_boolval(const zval *op);
//extern void zephir_get_strval(zval *op, zephir_str **str_p);

extern int zephir_is_numeric(const zval *op);

extern int zephir_is_equal(zval *op1, zval *op2 TSRMLS_DC);
extern int zephir_is_identical(zval *op1, zval *op2 TSRMLS_DC);

extern int zephir_less(zval *op1, zval *op2 TSRMLS_DC);
extern int zephir_less_long(zval *op1, long op2 TSRMLS_DC);

extern int zephir_greater(zval *op1, zval *op2 TSRMLS_DC);
extern int zephir_greater_long(zval *op1, long op2 TSRMLS_DC);

extern int zephir_less_equal(zval *op1, zval *op2 TSRMLS_DC);
extern int zephir_less_equal_long(zval *op1, long op2 TSRMLS_DC);

extern int zephir_greater_equal(zval *op1, zval *op2 TSRMLS_DC);
extern int zephir_greater_equal_long(zval *op1, long op2 TSRMLS_DC);

#define zephir_get_numberval(z) (Z_TYPE_P(z) == IS_LONG ? Z_LVAL_P(z) : zephir_get_doubleval(z))

#define ZEPHIR_ADD_ASSIGN(z, v)  \
	{  \
		zval tmp;  \
		ZEPHIR_SEPARATE(z);  \
		if (Z_TYPE_P(z) == IS_LONG && Z_TYPE_P(v) == IS_LONG) {  \
			Z_LVAL_P(z) += Z_LVAL_P(v);  \
		} else {  \
			if (Z_TYPE_P(z) == IS_LONG && Z_TYPE_P(v) == IS_DOUBLE) {  \
				Z_LVAL_P(z) += Z_DVAL_P(v);  \
			} else {  \
				zephir_add_function(&tmp, z, v TSRMLS_CC);  \
				if (Z_TYPE(tmp) == IS_LONG) {  \
					Z_LVAL_P(z) = Z_LVAL(tmp);  \
				} else {  \
					if (Z_TYPE(tmp) == IS_DOUBLE) {  \
						Z_DVAL_P(z) = Z_DVAL(tmp);  \
					}  \
				}  \
			}  \
		}  \
	}

#define ZEPHIR_SUB_ASSIGN(z, v)  \
	{  \
		zval tmp;  \
		ZEPHIR_SEPARATE(z);  \
		if (Z_TYPE_P(z) == IS_LONG && Z_TYPE_P(v) == IS_LONG) {  \
			Z_LVAL_P(z) -= Z_LVAL_P(v);  \
		} else {  \
			if (Z_TYPE_P(z) == IS_LONG && Z_TYPE_P(v) == IS_DOUBLE) {  \
				Z_LVAL_P(z) -= Z_DVAL_P(v);  \
			} else {  \
				sub_function(&tmp, z, v TSRMLS_CC);  \
				if (Z_TYPE(tmp) == IS_LONG) {  \
					Z_LVAL_P(z) = Z_LVAL(tmp);  \
				} else {  \
					if (Z_TYPE(tmp) == IS_DOUBLE) {  \
						Z_DVAL_P(z) = Z_DVAL(tmp);  \
					}  \
				}  \
			}  \
		}  \
	}
