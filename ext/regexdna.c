
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"


/**
 * RegexDNA
 *
 * @see http://www.haskell.org/haskellwiki/Shootout/Regex_DNA
 */
ZEPHIR_INIT_CLASS(Test_RegexDNA) {

	ZEPHIR_REGISTER_CLASS(Test, RegexDNA, regexdna, test_regexdna_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_RegexDNA, process) {

	HashTable *_27;
	HashPosition _26;
	zval *path, *variants, *vIUB, *vIUBnew, *stuffToRemove, *contents = NULL, *initialLength, *regex = NULL, *codeLength, *discard, *_0 = NULL, *_1, *_2, *_3, *_4, *_5, *_6, *_7, *_8, *_9, *_10, *_11, *_12, *_13, *_14, *_15, *_16, *_17, *_18, *_19, *_20, *_21, *_22, *_23, _24, *_25 = NULL, **_28, *_29 = NULL, *_30 = NULL, *_31;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &path);



	ZEPHIR_INIT_VAR(variants);
	array_init(variants);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "agggtaaa|tttaccct", 1);
	zephir_array_append(&variants, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "[cgt]gggtaaa|tttaccc[acg]", 1);
	zephir_array_append(&variants, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "a[act]ggtaaa|tttacc[agt]t", 1);
	zephir_array_append(&variants, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "ag[act]gtaaa|tttac[agt]ct", 1);
	zephir_array_append(&variants, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "agg[act]taaa|ttta[agt]cct", 1);
	zephir_array_append(&variants, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "aggg[acg]aaa|ttt[cgt]ccct", 1);
	zephir_array_append(&variants, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "agggt[cgt]aa|tt[acg]accct", 1);
	zephir_array_append(&variants, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "agggta[cgt]a|t[acg]taccct", 1);
	zephir_array_append(&variants, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "agggtaa[cgt]|[acg]ttaccct", 1);
	zephir_array_append(&variants, _0, 0);
	ZEPHIR_INIT_VAR(vIUB);
	array_init(vIUB);
	ZEPHIR_INIT_VAR(vIUBnew);
	array_init(vIUBnew);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "/B/S", 1);
	zephir_array_append(&vIUB, _0, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "(c|g|t)", 1);
	zephir_array_append(&vIUBnew, _1, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "/D/S", 1);
	zephir_array_append(&vIUB, _2, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_STRING(_3, "(a|g|t)", 1);
	zephir_array_append(&vIUBnew, _3, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_4);
	ZVAL_STRING(_4, "/H/S", 1);
	zephir_array_append(&vIUB, _4, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_5);
	ZVAL_STRING(_5, "(a|c|t)", 1);
	zephir_array_append(&vIUBnew, _5, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_6);
	ZVAL_STRING(_6, "/K/S", 1);
	zephir_array_append(&vIUB, _6, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_7);
	ZVAL_STRING(_7, "(g|t)", 1);
	zephir_array_append(&vIUBnew, _7, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_8);
	ZVAL_STRING(_8, "/M/S", 1);
	zephir_array_append(&vIUB, _8, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_9);
	ZVAL_STRING(_9, "(a|c)", 1);
	zephir_array_append(&vIUBnew, _9, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_10);
	ZVAL_STRING(_10, "/N/S", 1);
	zephir_array_append(&vIUB, _10, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_11);
	ZVAL_STRING(_11, "(a|c|g|t)", 1);
	zephir_array_append(&vIUBnew, _11, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_12);
	ZVAL_STRING(_12, "/R/S", 1);
	zephir_array_append(&vIUB, _12, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_13);
	ZVAL_STRING(_13, "(a|g)", 1);
	zephir_array_append(&vIUBnew, _13, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_14);
	ZVAL_STRING(_14, "/S/S", 1);
	zephir_array_append(&vIUB, _14, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_15);
	ZVAL_STRING(_15, "(c|g)", 1);
	zephir_array_append(&vIUBnew, _15, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_16);
	ZVAL_STRING(_16, "/V/S", 1);
	zephir_array_append(&vIUB, _16, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_17);
	ZVAL_STRING(_17, "(a|c|g)", 1);
	zephir_array_append(&vIUBnew, _17, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_18);
	ZVAL_STRING(_18, "/W/S", 1);
	zephir_array_append(&vIUB, _18, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_19);
	ZVAL_STRING(_19, "(a|t)", 1);
	zephir_array_append(&vIUBnew, _19, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_20);
	ZVAL_STRING(_20, "/Y/S", 1);
	zephir_array_append(&vIUB, _20, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_21);
	ZVAL_STRING(_21, "(c|t)", 1);
	zephir_array_append(&vIUBnew, _21, PH_SEPARATE);
	ZEPHIR_INIT_VAR(stuffToRemove);
	ZVAL_STRING(stuffToRemove, "^>.*$|\n", 1);
	ZEPHIR_INIT_VAR(discard);
	ZVAL_NULL(discard);
	ZEPHIR_INIT_VAR(contents);
	zephir_call_func_p1(contents, "file_get_contents", path);
	ZEPHIR_INIT_VAR(initialLength);
	ZVAL_LONG(initialLength, zephir_fast_strlen_ev(contents));
	ZEPHIR_INIT_VAR(_22);
	ZEPHIR_CONCAT_SV(_22, "/", stuffToRemove);
	ZEPHIR_INIT_VAR(_23);
	ZEPHIR_CONCAT_VS(_23, _22, "/mS");
	ZEPHIR_SINIT_VAR(_24);
	ZVAL_STRING(&_24, "", 0);
	ZEPHIR_INIT_VAR(_25);
	zephir_call_func_p3(_25, "preg_replace", _23, &_24, contents);
	ZEPHIR_CPY_WRT(contents, _25);
	ZEPHIR_INIT_VAR(codeLength);
	ZVAL_LONG(codeLength, zephir_fast_strlen_ev(contents));
	zephir_is_iterable(variants, &_27, &_26, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_27, (void**) &_28, &_26) == SUCCESS
		; zend_hash_move_forward_ex(_27, &_26)
	) {
		ZEPHIR_GET_HVALUE(regex, _28);
		zend_print_zval(regex, 0);
		php_printf(" ");
		ZEPHIR_INIT_LNVAR(_29);
		ZEPHIR_CONCAT_SV(_29, "/", regex);
		ZEPHIR_INIT_LNVAR(_30);
		ZEPHIR_CONCAT_VS(_30, _29, "/iS");
		Z_SET_ISREF_P(discard);
		ZEPHIR_INIT_NVAR(_25);
		zephir_call_func_p3(_25, "preg_match_all", _30, contents, discard);
		zend_print_zval(_25, 0);
		php_printf("%c", '\n');
	}
	ZEPHIR_INIT_VAR(_31);
	zephir_call_func_p3(_31, "preg_replace", vIUB, vIUBnew, contents);
	ZEPHIR_CPY_WRT(contents, _31);
	php_printf("%c", '\n');
	zend_print_zval(initialLength, 0);
	php_printf("%c", '\n');
	zend_print_zval(codeLength, 0);
	php_printf("%c", '\n');
	php_printf("%d", zephir_fast_strlen_ev(contents));
	php_printf("%c", '\n');
	ZEPHIR_MM_RESTORE();

}

