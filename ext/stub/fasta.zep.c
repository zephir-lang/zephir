
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/string.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/math.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_Fasta)
{
	ZEPHIR_REGISTER_CLASS(Stub, Fasta, stub, fasta, stub_fasta_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Fasta, fastaRepeat)
{
	zval _2, _5;
	zend_long ZEPHIR_LAST_CALL_STATUS, seqi, i = 0;
	zval seq;
	zval *n, n_sub, *seq_param = NULL, len, j, k, l, block, str, lines, _0, _1, _3, _4, _9, _10, _6$$3, _7$$3, _8$$3, _11$$6, _12$$7, _13$$7, _14$$7, _15$$7;

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&len);
	ZVAL_UNDEF(&j);
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&l);
	ZVAL_UNDEF(&block);
	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&lines);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_9);
	ZVAL_UNDEF(&_10);
	ZVAL_UNDEF(&_6$$3);
	ZVAL_UNDEF(&_7$$3);
	ZVAL_UNDEF(&_8$$3);
	ZVAL_UNDEF(&_11$$6);
	ZVAL_UNDEF(&_12$$7);
	ZVAL_UNDEF(&_13$$7);
	ZVAL_UNDEF(&_14$$7);
	ZVAL_UNDEF(&_15$$7);
	ZVAL_UNDEF(&seq);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_5);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(n)
		Z_PARAM_STR(seq)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &n, &seq_param);
	zephir_get_strval(&seq, seq_param);
	seqi = 0;
	ZVAL_NULL(&len);
	ZVAL_LONG(&len, zephir_fast_strlen_ev(&seq));
	ZVAL_NULL(&_0);
	ZVAL_LONG(&_1, ((zephir_get_numberval(&len) * 60) + 1));
	zephir_create_array(&_0, zephir_get_intval(&_1), 1);
	zephir_get_arrval(&_2, &_0);
	ZVAL_NULL(&str);
	zephir_fast_join(&str, &seq, &_2);
	ZVAL_NULL(&_3);
	mul_function(&_4, &len, &len);
	zephir_create_array(&_3, zephir_get_intval(&_4), 1);
	zephir_get_arrval(&_5, &_3);
	ZEPHIR_CPY_WRT(&lines, &_5);
	ZVAL_NULL(&j);
	mul_function(&j, &len, &len);
	i = zephir_get_numberval(&j);
	while (1) {
		if (ZEPHIR_LE_LONG(&j, -1)) {
			break;
		}
		SEPARATE_ZVAL(&j);
		zephir_decrement(&j);
		ZVAL_LONG(&_6$$3, (60 * zephir_get_numberval(&j)));
		ZVAL_LONG(&_7$$3, 60);
		ZEPHIR_INIT_NVAR(&_8$$3);
		zephir_substr(&_8$$3, &str, zephir_get_intval(&_6$$3), 60 , 0);
		zephir_array_update_zval(&lines, &j, &_8$$3, PH_COPY | PH_SEPARATE);
	}
	ZEPHIR_INIT_NVAR(&j);
	ZVAL_LONG(&j, 0);
	ZVAL_DOUBLE(&_9, zephir_safe_div_zval_long(n, 60));
	ZVAL_NULL(&l);
	ZVAL_DOUBLE(&l, zephir_floor(&_9));
	ZVAL_DOUBLE(&_10, zephir_safe_div_zval_long(&l, i));
	ZVAL_NULL(&k);
	ZVAL_DOUBLE(&k, zephir_floor(&_10));
	ZVAL_NULL(&block);
	zephir_fast_join_str(&block, SL("\n"), &lines);
	while (1) {
		if (!(ZEPHIR_LT(&j, &k))) {
			break;
		}
		zend_print_zval(&block, 0);
		SEPARATE_ZVAL(&j);
		zephir_increment(&j);
	}
	ZEPHIR_INIT_NVAR(&j);
	ZVAL_LONG(&j, 0);
	ZEPHIR_INIT_NVAR(&k);
	ZVAL_DOUBLE(&k, zephir_safe_mod_zval_long(&l, i));
	while (1) {
		if (!(ZEPHIR_LT(&j, &k))) {
			break;
		}
		zephir_array_fetch(&_11$$6, &lines, &j, PH_NOISY | PH_READONLY, "stub/fasta.zep", 38);
		zend_print_zval(&_11$$6, 0);
		SEPARATE_ZVAL(&j);
		zephir_increment(&j);
	}
	if (zephir_safe_mod_zval_long(n, 60) > 0) {
		zephir_array_fetch(&_12$$7, &lines, &k, PH_NOISY | PH_READONLY, "stub/fasta.zep", 43);
		ZVAL_LONG(&_13$$7, 0);
		ZVAL_DOUBLE(&_14$$7, zephir_safe_mod_zval_long(n, 60));
		ZVAL_NULL(&_15$$7);
		zephir_substr(&_15$$7, &_12$$7, 0 , zephir_get_intval(&_14$$7), 0);
		zend_print_zval(&_15$$7, 0);
	}
}

PHP_METHOD(Stub_Fasta, fastRandom)
{

}

PHP_METHOD(Stub_Fasta, main)
{
	zval _0;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *n, n_sub, alu, iub, homoSap, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&alu);
	ZVAL_UNDEF(&iub);
	ZVAL_UNDEF(&homoSap);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &n);
	ZVAL_NULL(&_0);
	ZEPHIR_CONCAT_SSSSSSS(&_0, "GGCCGGGCGCGGTGGCTCACGCCTGTAATCCCAGCACTTTGG", "GAGGCCGAGGCGGGCGGATCACCTGAGGTCAGGAGTTCGAGA", "CCAGCCTGGCCAACATGGTGAAACCCCGTCTCTACTAAAAAT", "ACAAAAATTAGCCGGGCGTGGTGGCGCGCGCCTGTAATCCCA", "GCTACTCGGGAGGCTGAGGCAGGAGAATCGCTTGAACCCGGG", "AGGCGGAGGTTGCAGTGAGCCGAGATCGCGCCACTGCACTCC", "AGCCTGGGCGACAGAGCGAGACTCCGTCTCAAAAA");
	ZEPHIR_CPY_WRT(&alu, &_0);
	ZVAL_NULL(&iub);
	zephir_create_array(&iub, 15, 0);
	add_assoc_double_ex(&iub, SL("a"), 0.27);
	add_assoc_double_ex(&iub, SL("c"), 0.12);
	add_assoc_double_ex(&iub, SL("g"), 0.12);
	add_assoc_double_ex(&iub, SL("t"), 0.27);
	add_assoc_double_ex(&iub, SL("B"), 0.02);
	add_assoc_double_ex(&iub, SL("D"), 0.02);
	add_assoc_double_ex(&iub, SL("H"), 0.02);
	add_assoc_double_ex(&iub, SL("K"), 0.02);
	add_assoc_double_ex(&iub, SL("M"), 0.02);
	add_assoc_double_ex(&iub, SL("N"), 0.02);
	add_assoc_double_ex(&iub, SL("R"), 0.02);
	add_assoc_double_ex(&iub, SL("S"), 0.02);
	add_assoc_double_ex(&iub, SL("V"), 0.02);
	add_assoc_double_ex(&iub, SL("W"), 0.02);
	add_assoc_double_ex(&iub, SL("Y"), 0.02);
	ZVAL_NULL(&homoSap);
	zephir_create_array(&homoSap, 4, 0);
	add_assoc_double_ex(&homoSap, SL("a"), 0.3029549426680);
	add_assoc_double_ex(&homoSap, SL("c"), 0.1979883004921);
	add_assoc_double_ex(&homoSap, SL("g"), 0.1975473066391);
	add_assoc_double_ex(&homoSap, SL("t"), 0.3015094502008);
	php_printf("%s", ">ONE Homo sapiens alu");
	ZVAL_LONG(&_1, (2 * zephir_get_numberval(n)));
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "fastarepeat", NULL, 0, &_1, &alu);
	zephir_check_call_status();
}

