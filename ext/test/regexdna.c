
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
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/string.h"
#include "kernel/concat.h"


/**
 * RegexDNA
 *
 * @see http://www.haskell.org/haskellwiki/Shootout/Regex_DNA
 */
ZEPHIR_INIT_CLASS(Test_RegexDNA) {

	ZEPHIR_REGISTER_CLASS(Test, RegexDNA, test, regexdna, test_regexdna_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_RegexDNA, process) {

	HashTable *_26;
	HashPosition _25;
	zval *path, *variants, *vIUB, *vIUBnew, *stuffToRemove, *contents = NULL, *initialLength, *regex = NULL, *codeLength, *discard, *_0, *_1, *_2, *_3, *_4, *_5, *_6, *_7, *_8, *_9, *_10, *_11, *_12, *_13, *_14, *_15, *_16, *_17, *_18, *_19, *_20, *_21, *_22, _23, *_24 = NULL, **_27, *_28 = NULL, *_29;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &path);



	ZEPHIR_INIT_VAR(variants);
	array_init(variants);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "agggtaaa|tttaccct", 1);
	zephir_array_fast_append(variants, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "[cgt]gggtaaa|tttaccc[acg]", 1);
	zephir_array_fast_append(variants, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "a[act]ggtaaa|tttacc[agt]t", 1);
	zephir_array_fast_append(variants, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "ag[act]gtaaa|tttac[agt]ct", 1);
	zephir_array_fast_append(variants, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "agg[act]taaa|ttta[agt]cct", 1);
	zephir_array_fast_append(variants, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "aggg[acg]aaa|ttt[cgt]ccct", 1);
	zephir_array_fast_append(variants, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "agggt[cgt]aa|tt[acg]accct", 1);
	zephir_array_fast_append(variants, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "agggta[cgt]a|t[acg]taccct", 1);
	zephir_array_fast_append(variants, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "agggtaa[cgt]|[acg]ttaccct", 1);
	zephir_array_fast_append(variants, _0);
	ZEPHIR_INIT_VAR(vIUB);
	array_init(vIUB);
	ZEPHIR_INIT_VAR(vIUBnew);
	array_init(vIUBnew);
	ZEPHIR_INIT_BNVAR(_0);
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
	ZEPHIR_CONCAT_SVS(_22, "/", stuffToRemove, "/mS");
	ZEPHIR_SINIT_VAR(_23);
	ZVAL_STRING(&_23, "", 0);
	ZEPHIR_INIT_VAR(_24);
	zephir_call_func_p3(_24, "preg_replace", _22, &_23, contents);
	ZEPHIR_CPY_WRT(contents, _24);
	ZEPHIR_INIT_VAR(codeLength);
	ZVAL_LONG(codeLength, zephir_fast_strlen_ev(contents));
	zephir_is_iterable(variants, &_26, &_25, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_26, (void**) &_27, &_25) == SUCCESS
		; zend_hash_move_forward_ex(_26, &_25)
	) {
		ZEPHIR_GET_HVALUE(regex, _27);
		zend_print_zval(regex, 0);
		php_printf(" ");
		ZEPHIR_INIT_LNVAR(_28);
		ZEPHIR_CONCAT_SVS(_28, "/", regex, "/iS");
		Z_SET_ISREF_P(discard);
		ZEPHIR_INIT_NVAR(_24);
		zephir_call_func_p3(_24, "preg_match_all", _28, contents, discard);
		zend_print_zval(_24, 0);
		php_printf("%c", '\n');
	}
	ZEPHIR_INIT_VAR(_29);
	zephir_call_func_p3(_29, "preg_replace", vIUB, vIUBnew, contents);
	ZEPHIR_CPY_WRT(contents, _29);
	php_printf("%c", '\n');
	zend_print_zval(initialLength, 0);
	php_printf("%c", '\n');
	zend_print_zval(codeLength, 0);
	php_printf("%c", '\n');
	php_printf("%d", zephir_fast_strlen_ev(contents));
	php_printf("%c", '\n');
	ZEPHIR_MM_RESTORE();

}

