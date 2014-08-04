
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
#include "kernel/file.h"
#include "kernel/string.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/hash.h"


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

	HashTable *_6;
	HashPosition _5;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_4 = NULL, *_9 = NULL;
	zval *path, *variants, *vIUB, *vIUBnew, *stuffToRemove, *contents = NULL, *initialLength, *regex = NULL, *codeLength, *discard = NULL, *_0, *_1, _2, *_3 = NULL, **_7, *_8 = NULL, *_10 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &path);

	ZEPHIR_INIT_VAR(discard);
	ZVAL_NULL(discard);


	ZEPHIR_INIT_VAR(variants);
	array_init_size(variants, 13);
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
	zephir_array_append(&vIUB, _0, PH_SEPARATE, "test/regexdna.zep", 30);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "(c|g|t)", 1);
	zephir_array_append(&vIUBnew, _0, PH_SEPARATE, "test/regexdna.zep", 30);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "/D/S", 1);
	zephir_array_append(&vIUB, _0, PH_SEPARATE, "test/regexdna.zep", 31);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "(a|g|t)", 1);
	zephir_array_append(&vIUBnew, _0, PH_SEPARATE, "test/regexdna.zep", 31);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "/H/S", 1);
	zephir_array_append(&vIUB, _0, PH_SEPARATE, "test/regexdna.zep", 32);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "(a|c|t)", 1);
	zephir_array_append(&vIUBnew, _0, PH_SEPARATE, "test/regexdna.zep", 32);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "/K/S", 1);
	zephir_array_append(&vIUB, _0, PH_SEPARATE, "test/regexdna.zep", 33);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "(g|t)", 1);
	zephir_array_append(&vIUBnew, _0, PH_SEPARATE, "test/regexdna.zep", 33);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "/M/S", 1);
	zephir_array_append(&vIUB, _0, PH_SEPARATE, "test/regexdna.zep", 34);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "(a|c)", 1);
	zephir_array_append(&vIUBnew, _0, PH_SEPARATE, "test/regexdna.zep", 34);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "/N/S", 1);
	zephir_array_append(&vIUB, _0, PH_SEPARATE, "test/regexdna.zep", 35);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "(a|c|g|t)", 1);
	zephir_array_append(&vIUBnew, _0, PH_SEPARATE, "test/regexdna.zep", 35);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "/R/S", 1);
	zephir_array_append(&vIUB, _0, PH_SEPARATE, "test/regexdna.zep", 36);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "(a|g)", 1);
	zephir_array_append(&vIUBnew, _0, PH_SEPARATE, "test/regexdna.zep", 36);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "/S/S", 1);
	zephir_array_append(&vIUB, _0, PH_SEPARATE, "test/regexdna.zep", 37);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "(c|g)", 1);
	zephir_array_append(&vIUBnew, _0, PH_SEPARATE, "test/regexdna.zep", 37);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "/V/S", 1);
	zephir_array_append(&vIUB, _0, PH_SEPARATE, "test/regexdna.zep", 38);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "(a|c|g)", 1);
	zephir_array_append(&vIUBnew, _0, PH_SEPARATE, "test/regexdna.zep", 38);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "/W/S", 1);
	zephir_array_append(&vIUB, _0, PH_SEPARATE, "test/regexdna.zep", 39);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "(a|t)", 1);
	zephir_array_append(&vIUBnew, _0, PH_SEPARATE, "test/regexdna.zep", 39);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "/Y/S", 1);
	zephir_array_append(&vIUB, _0, PH_SEPARATE, "test/regexdna.zep", 40);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "(c|t)", 1);
	zephir_array_append(&vIUBnew, _0, PH_SEPARATE, "test/regexdna.zep", 40);
	ZEPHIR_INIT_VAR(stuffToRemove);
	ZVAL_STRING(stuffToRemove, "^>.*$|\n", 1);
	ZEPHIR_INIT_VAR(discard);
	ZVAL_NULL(discard);
	ZEPHIR_INIT_VAR(contents);
	zephir_file_get_contents(contents, path TSRMLS_CC);
	ZEPHIR_INIT_VAR(initialLength);
	ZVAL_LONG(initialLength, zephir_fast_strlen_ev(contents));
	ZEPHIR_INIT_VAR(_1);
	ZEPHIR_CONCAT_SVS(_1, "/", stuffToRemove, "/mS");
	ZEPHIR_SINIT_VAR(_2);
	ZVAL_STRING(&_2, "", 0);
	ZEPHIR_CALL_FUNCTION(&_3, "preg_replace", &_4, _1, &_2, contents);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(contents, _3);
	ZEPHIR_INIT_VAR(codeLength);
	ZVAL_LONG(codeLength, zephir_fast_strlen_ev(contents));
	zephir_is_iterable(variants, &_6, &_5, 0, 0);
	for (
	  ; zephir_hash_get_current_data_ex(_6, (void**) &_7, &_5) == SUCCESS
	  ; zephir_hash_move_forward_ex(_6, &_5)
	) {
		ZEPHIR_GET_HVALUE(regex, _7);
		zend_print_zval(regex, 0);
		php_printf(" ");
		ZEPHIR_INIT_LNVAR(_8);
		ZEPHIR_CONCAT_SVS(_8, "/", regex, "/iS");
		Z_SET_ISREF_P(discard);
		ZEPHIR_CALL_FUNCTION(&_3, "preg_match_all", &_9, _8, contents, discard);
		Z_UNSET_ISREF_P(discard);
		zephir_check_call_status();
		zend_print_zval(_3, 0);
		php_printf("%c", '\n');
	}
	ZEPHIR_CALL_FUNCTION(&_10, "preg_replace", &_4, vIUB, vIUBnew, contents);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(contents, _10);
	php_printf("%c", '\n');
	zend_print_zval(initialLength, 0);
	php_printf("%c", '\n');
	zend_print_zval(codeLength, 0);
	php_printf("%c", '\n');
	php_printf("%d", zephir_fast_strlen_ev(contents));
	php_printf("%c", '\n');
	ZEPHIR_MM_RESTORE();

}

