
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
#include "kernel/concat.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Test_Fasta) {

	ZEPHIR_REGISTER_CLASS(Test, Fasta, test, fasta, test_fasta_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Fasta, fastaRepeat) {

	zval *_2 = NULL, *_5 = NULL;
	int ZEPHIR_LAST_CALL_STATUS, seqi, i = 0;
	zval *seq = NULL;
	zval *n, *seq_param = NULL, *len = NULL, *j = NULL, *k = NULL, *l = NULL, *block = NULL, *str = NULL, *lines = NULL, *_0, _1, *_3, _4, _9, _10, _6$$3 = zval_used_for_init, _7$$3 = zval_used_for_init, *_8$$3 = NULL, *_11$$6, *_12$$7, _13$$7, _14$$7, *_15$$7;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &n, &seq_param);

	zephir_get_strval(seq, seq_param);


	seqi = 0;
	ZEPHIR_INIT_VAR(len);
	ZVAL_LONG(len, zephir_fast_strlen_ev(seq));
	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_LONG(&_1, ((zephir_get_numberval(len) * 60) + 1));
	zephir_create_array(_0, zephir_get_intval(&_1), 1 TSRMLS_CC);
	zephir_get_arrval(_2, _0);
	ZEPHIR_INIT_VAR(str);
	zephir_fast_join(str, seq, _2 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_3);
	ZEPHIR_SINIT_VAR(_4);
	mul_function(&_4, len, len TSRMLS_CC);
	zephir_create_array(_3, zephir_get_intval(&_4), 1 TSRMLS_CC);
	zephir_get_arrval(_5, _3);
	ZEPHIR_CPY_WRT(lines, _5);
	ZEPHIR_INIT_VAR(j);
	mul_function(j, len, len TSRMLS_CC);
	i = zephir_get_numberval(j);
	while (1) {
		if (ZEPHIR_LE_LONG(j, -1)) {
			break;
		}
		ZEPHIR_SEPARATE(j);
		zephir_decrement(j);
		ZEPHIR_SINIT_NVAR(_6$$3);
		ZVAL_LONG(&_6$$3, (60 * zephir_get_numberval(j)));
		ZEPHIR_SINIT_NVAR(_7$$3);
		ZVAL_LONG(&_7$$3, 60);
		ZEPHIR_INIT_NVAR(_8$$3);
		zephir_substr(_8$$3, str, zephir_get_intval(&_6$$3), 60 , 0);
		zephir_array_update_zval(&lines, j, &_8$$3, PH_COPY | PH_SEPARATE);
	}
	ZEPHIR_INIT_NVAR(j);
	ZVAL_LONG(j, 0);
	ZEPHIR_SINIT_VAR(_9);
	ZVAL_DOUBLE(&_9, zephir_safe_div_zval_long(n, 60 TSRMLS_CC));
	ZEPHIR_INIT_VAR(l);
	ZVAL_DOUBLE(l, zephir_floor(&_9 TSRMLS_CC));
	ZEPHIR_SINIT_VAR(_10);
	ZVAL_DOUBLE(&_10, zephir_safe_div_zval_long(l, i TSRMLS_CC));
	ZEPHIR_INIT_VAR(k);
	ZVAL_DOUBLE(k, zephir_floor(&_10 TSRMLS_CC));
	ZEPHIR_INIT_VAR(block);
	zephir_fast_join_str(block, SL("\n"), lines TSRMLS_CC);
	while (1) {
		if (!(ZEPHIR_LT(j, k))) {
			break;
		}
		zend_print_zval(block, 0);
		ZEPHIR_SEPARATE(j);
		zephir_increment(j);
	}
	ZEPHIR_INIT_NVAR(j);
	ZVAL_LONG(j, 0);
	ZEPHIR_INIT_NVAR(k);
	ZVAL_DOUBLE(k, zephir_safe_mod_zval_long(l, i TSRMLS_CC));
	while (1) {
		if (!(ZEPHIR_LT(j, k))) {
			break;
		}
		zephir_array_fetch(&_11$$6, lines, j, PH_NOISY | PH_READONLY, "test/fasta.zep", 38 TSRMLS_CC);
		zend_print_zval(_11$$6, 0);
		ZEPHIR_SEPARATE(j);
		zephir_increment(j);
	}
	if (zephir_safe_mod_zval_long(n, 60 TSRMLS_CC) > 0) {
		zephir_array_fetch(&_12$$7, lines, k, PH_NOISY | PH_READONLY, "test/fasta.zep", 43 TSRMLS_CC);
		ZEPHIR_SINIT_VAR(_13$$7);
		ZVAL_LONG(&_13$$7, 0);
		ZEPHIR_SINIT_VAR(_14$$7);
		ZVAL_DOUBLE(&_14$$7, zephir_safe_mod_zval_long(n, 60 TSRMLS_CC));
		ZEPHIR_INIT_VAR(_15$$7);
		zephir_substr(_15$$7, _12$$7, 0 , zephir_get_intval(&_14$$7), 0);
		zend_print_zval(_15$$7, 0);
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Fasta, fastRandom) {

	


}

PHP_METHOD(Test_Fasta, main) {

	zval *_0;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *n, *alu = NULL, *iub = NULL, *homoSap = NULL, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_CONCAT_SSSSSSS(_0, "GGCCGGGCGCGGTGGCTCACGCCTGTAATCCCAGCACTTTGG", "GAGGCCGAGGCGGGCGGATCACCTGAGGTCAGGAGTTCGAGA", "CCAGCCTGGCCAACATGGTGAAACCCCGTCTCTACTAAAAAT", "ACAAAAATTAGCCGGGCGTGGTGGCGCGCGCCTGTAATCCCA", "GCTACTCGGGAGGCTGAGGCAGGAGAATCGCTTGAACCCGGG", "AGGCGGAGGTTGCAGTGAGCCGAGATCGCGCCACTGCACTCC", "AGCCTGGGCGACAGAGCGAGACTCCGTCTCAAAAA");
	ZEPHIR_CPY_WRT(alu, _0);
	ZEPHIR_INIT_VAR(iub);
	zephir_create_array(iub, 15, 0 TSRMLS_CC);
	add_assoc_double_ex(iub, SS("a"), 0.27);
	add_assoc_double_ex(iub, SS("c"), 0.12);
	add_assoc_double_ex(iub, SS("g"), 0.12);
	add_assoc_double_ex(iub, SS("t"), 0.27);
	add_assoc_double_ex(iub, SS("B"), 0.02);
	add_assoc_double_ex(iub, SS("D"), 0.02);
	add_assoc_double_ex(iub, SS("H"), 0.02);
	add_assoc_double_ex(iub, SS("K"), 0.02);
	add_assoc_double_ex(iub, SS("M"), 0.02);
	add_assoc_double_ex(iub, SS("N"), 0.02);
	add_assoc_double_ex(iub, SS("R"), 0.02);
	add_assoc_double_ex(iub, SS("S"), 0.02);
	add_assoc_double_ex(iub, SS("V"), 0.02);
	add_assoc_double_ex(iub, SS("W"), 0.02);
	add_assoc_double_ex(iub, SS("Y"), 0.02);
	ZEPHIR_INIT_VAR(homoSap);
	zephir_create_array(homoSap, 4, 0 TSRMLS_CC);
	add_assoc_double_ex(homoSap, SS("a"), 0.3029549426680);
	add_assoc_double_ex(homoSap, SS("c"), 0.1979883004921);
	add_assoc_double_ex(homoSap, SS("g"), 0.1975473066391);
	add_assoc_double_ex(homoSap, SS("t"), 0.3015094502008);
	php_printf("%s", ">ONE Homo sapiens alu");
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, (2 * zephir_get_numberval(n)));
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "fastarepeat", NULL, 0, _1, alu);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

