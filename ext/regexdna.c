
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

	HashTable *_36;
	HashPosition _35;
	zval *path, *variants, *vIUB, *vIUBnew, *stuffToRemove, *contents = NULL, *initialLength, *regex = NULL, *codeLength, *discard, *_0, *_1, *_2, *_3, *_4, *_5, *_6, *_7, *_8, *_9, *_10, *_11, *_12, *_13, *_14, *_15, *_16, *_17, *_18, *_19, *_20, *_21, *_22, *_23, *_24, *_25, *_26, *_27, *_28, *_29, *_30, *_31, *_32, _33 = zval_used_for_init, *_34, **_37, *_38 = NULL, *_39 = NULL, *_40 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &path);



	ZEPHIR_INIT_VAR(variants);
	array_init(variants);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "agggtaaa|tttaccct", 1);
	zephir_array_append(&variants, _0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "[cgt]gggtaaa|tttaccc[acg]", 1);
	zephir_array_append(&variants, _1, 0);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "a[act]ggtaaa|tttacc[agt]t", 1);
	zephir_array_append(&variants, _2, 0);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_STRING(_3, "ag[act]gtaaa|tttac[agt]ct", 1);
	zephir_array_append(&variants, _3, 0);
	ZEPHIR_INIT_VAR(_4);
	ZVAL_STRING(_4, "agg[act]taaa|ttta[agt]cct", 1);
	zephir_array_append(&variants, _4, 0);
	ZEPHIR_INIT_VAR(_5);
	ZVAL_STRING(_5, "aggg[acg]aaa|ttt[cgt]ccct", 1);
	zephir_array_append(&variants, _5, 0);
	ZEPHIR_INIT_VAR(_6);
	ZVAL_STRING(_6, "agggt[cgt]aa|tt[acg]accct", 1);
	zephir_array_append(&variants, _6, 0);
	ZEPHIR_INIT_VAR(_7);
	ZVAL_STRING(_7, "agggta[cgt]a|t[acg]taccct", 1);
	zephir_array_append(&variants, _7, 0);
	ZEPHIR_INIT_VAR(_8);
	ZVAL_STRING(_8, "agggtaa[cgt]|[acg]ttaccct", 1);
	zephir_array_append(&variants, _8, 0);
	ZEPHIR_INIT_VAR(vIUB);
	array_init(vIUB);
	ZEPHIR_INIT_VAR(vIUBnew);
	array_init(vIUBnew);
	ZEPHIR_INIT_VAR(_9);
	ZVAL_STRING(_9, "/B/S", 1);
	zephir_array_append(&vIUB, _9, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_10);
	ZVAL_STRING(_10, "(c|g|t)", 1);
	zephir_array_append(&vIUBnew, _10, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_11);
	ZVAL_STRING(_11, "/D/S", 1);
	zephir_array_append(&vIUB, _11, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_12);
	ZVAL_STRING(_12, "(a|g|t)", 1);
	zephir_array_append(&vIUBnew, _12, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_13);
	ZVAL_STRING(_13, "/H/S", 1);
	zephir_array_append(&vIUB, _13, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_14);
	ZVAL_STRING(_14, "(a|c|t)", 1);
	zephir_array_append(&vIUBnew, _14, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_15);
	ZVAL_STRING(_15, "/K/S", 1);
	zephir_array_append(&vIUB, _15, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_16);
	ZVAL_STRING(_16, "(g|t)", 1);
	zephir_array_append(&vIUBnew, _16, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_17);
	ZVAL_STRING(_17, "/M/S", 1);
	zephir_array_append(&vIUB, _17, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_18);
	ZVAL_STRING(_18, "(a|c)", 1);
	zephir_array_append(&vIUBnew, _18, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_19);
	ZVAL_STRING(_19, "/N/S", 1);
	zephir_array_append(&vIUB, _19, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_20);
	ZVAL_STRING(_20, "(a|c|g|t)", 1);
	zephir_array_append(&vIUBnew, _20, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_21);
	ZVAL_STRING(_21, "/R/S", 1);
	zephir_array_append(&vIUB, _21, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_22);
	ZVAL_STRING(_22, "(a|g)", 1);
	zephir_array_append(&vIUBnew, _22, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_23);
	ZVAL_STRING(_23, "/S/S", 1);
	zephir_array_append(&vIUB, _23, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_24);
	ZVAL_STRING(_24, "(c|g)", 1);
	zephir_array_append(&vIUBnew, _24, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_25);
	ZVAL_STRING(_25, "/V/S", 1);
	zephir_array_append(&vIUB, _25, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_26);
	ZVAL_STRING(_26, "(a|c|g)", 1);
	zephir_array_append(&vIUBnew, _26, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_27);
	ZVAL_STRING(_27, "/W/S", 1);
	zephir_array_append(&vIUB, _27, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_28);
	ZVAL_STRING(_28, "(a|t)", 1);
	zephir_array_append(&vIUBnew, _28, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_29);
	ZVAL_STRING(_29, "/Y/S", 1);
	zephir_array_append(&vIUB, _29, PH_SEPARATE);
	ZEPHIR_INIT_VAR(_30);
	ZVAL_STRING(_30, "(c|t)", 1);
	zephir_array_append(&vIUBnew, _30, PH_SEPARATE);
	ZEPHIR_INIT_VAR(stuffToRemove);
	ZVAL_STRING(stuffToRemove, "^>.*$|\\n", 1);
	ZEPHIR_INIT_VAR(discard);
	ZVAL_NULL(discard);
	ZEPHIR_INIT_VAR(contents);
	zephir_call_func_p1(contents, "file_get_contents", path);
	ZEPHIR_INIT_VAR(initialLength);
	ZVAL_LONG(initialLength, zephir_fast_strlen_ev(contents));
	ZEPHIR_INIT_VAR(_31);
	ZEPHIR_CONCAT_SV(_31, "/", stuffToRemove);
	ZEPHIR_INIT_VAR(_32);
	ZEPHIR_CONCAT_VS(_32, _31, "/mS");
	ZEPHIR_SINIT_VAR(_33);
	ZVAL_STRING(&_33, "", 0);
	ZEPHIR_INIT_VAR(_34);
	zephir_call_func_p3(_34, "preg_replace", _32, &_33, contents);
	ZEPHIR_CPY_WRT(contents, _34);
	ZEPHIR_INIT_VAR(codeLength);
	ZVAL_LONG(codeLength, zephir_fast_strlen_ev(contents));
	zephir_is_iterable(variants, &_36, &_35, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_36, (void**) &_37, &_35) == SUCCESS
		; zend_hash_move_forward_ex(_36, &_35)
	) {
		ZEPHIR_GET_HVALUE(regex, _37);
		zend_print_zval(regex, 0);
		php_printf(" ");
		ZEPHIR_INIT_LNVAR(_38);
		ZEPHIR_CONCAT_SV(_38, "/", regex);
		ZEPHIR_INIT_LNVAR(_39);
		ZEPHIR_CONCAT_VS(_39, _38, "/iS");
		Z_SET_ISREF_P(discard);
		ZEPHIR_INIT_NVAR(_40);
		zephir_call_func_p3(_40, "preg_match_all", _39, contents, discard);
		zend_print_zval(_40, 0);
		php_printf("%c", '\n');
	}
	ZEPHIR_INIT_NVAR(_40);
	zephir_call_func_p3(_40, "preg_replace", vIUB, vIUBnew, contents);
	ZEPHIR_CPY_WRT(contents, _40);
	php_printf("%c", '\n');
	zend_print_zval(initialLength, 0);
	php_printf("%c", '\n');
	zend_print_zval(codeLength, 0);
	php_printf("%c", '\n');
	php_printf("%d", zephir_fast_strlen_ev(contents));
	php_printf("%c", '\n');
	ZEPHIR_MM_RESTORE();

}

