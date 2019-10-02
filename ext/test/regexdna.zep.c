
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
#include "kernel/object.h"


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

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_3 = NULL;
	zval *path, path_sub, variants, vIUB, vIUBnew, stuffToRemove, contents, initialLength, regex, codeLength, discard, _0, _1, _2, *_4, _5, _6$$3, _7$$3, _8$$4, _9$$4;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&variants);
	ZVAL_UNDEF(&vIUB);
	ZVAL_UNDEF(&vIUBnew);
	ZVAL_UNDEF(&stuffToRemove);
	ZVAL_UNDEF(&contents);
	ZVAL_UNDEF(&initialLength);
	ZVAL_UNDEF(&regex);
	ZVAL_UNDEF(&codeLength);
	ZVAL_UNDEF(&discard);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6$$3);
	ZVAL_UNDEF(&_7$$3);
	ZVAL_UNDEF(&_8$$4);
	ZVAL_UNDEF(&_9$$4);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &path);



	ZEPHIR_INIT_VAR(&discard);
	ZVAL_NULL(&discard);
	ZEPHIR_INIT_VAR(&variants);
	zephir_create_array(&variants, 9, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "agggtaaa|tttaccct");
	zephir_array_fast_append(&variants, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "[cgt]gggtaaa|tttaccc[acg]");
	zephir_array_fast_append(&variants, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "a[act]ggtaaa|tttacc[agt]t");
	zephir_array_fast_append(&variants, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "ag[act]gtaaa|tttac[agt]ct");
	zephir_array_fast_append(&variants, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "agg[act]taaa|ttta[agt]cct");
	zephir_array_fast_append(&variants, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "aggg[acg]aaa|ttt[cgt]ccct");
	zephir_array_fast_append(&variants, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "agggt[cgt]aa|tt[acg]accct");
	zephir_array_fast_append(&variants, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "agggta[cgt]a|t[acg]taccct");
	zephir_array_fast_append(&variants, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "agggtaa[cgt]|[acg]ttaccct");
	zephir_array_fast_append(&variants, &_0);
	ZEPHIR_INIT_VAR(&vIUB);
	array_init(&vIUB);
	ZEPHIR_INIT_VAR(&vIUBnew);
	array_init(&vIUBnew);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "/B/S");
	zephir_array_append(&vIUB, &_0, PH_SEPARATE, "test/regexdna.zep", 30);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "(c|g|t)");
	zephir_array_append(&vIUBnew, &_0, PH_SEPARATE, "test/regexdna.zep", 30);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "/D/S");
	zephir_array_append(&vIUB, &_0, PH_SEPARATE, "test/regexdna.zep", 31);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "(a|g|t)");
	zephir_array_append(&vIUBnew, &_0, PH_SEPARATE, "test/regexdna.zep", 31);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "/H/S");
	zephir_array_append(&vIUB, &_0, PH_SEPARATE, "test/regexdna.zep", 32);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "(a|c|t)");
	zephir_array_append(&vIUBnew, &_0, PH_SEPARATE, "test/regexdna.zep", 32);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "/K/S");
	zephir_array_append(&vIUB, &_0, PH_SEPARATE, "test/regexdna.zep", 33);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "(g|t)");
	zephir_array_append(&vIUBnew, &_0, PH_SEPARATE, "test/regexdna.zep", 33);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "/M/S");
	zephir_array_append(&vIUB, &_0, PH_SEPARATE, "test/regexdna.zep", 34);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "(a|c)");
	zephir_array_append(&vIUBnew, &_0, PH_SEPARATE, "test/regexdna.zep", 34);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "/N/S");
	zephir_array_append(&vIUB, &_0, PH_SEPARATE, "test/regexdna.zep", 35);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "(a|c|g|t)");
	zephir_array_append(&vIUBnew, &_0, PH_SEPARATE, "test/regexdna.zep", 35);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "/R/S");
	zephir_array_append(&vIUB, &_0, PH_SEPARATE, "test/regexdna.zep", 36);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "(a|g)");
	zephir_array_append(&vIUBnew, &_0, PH_SEPARATE, "test/regexdna.zep", 36);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "/S/S");
	zephir_array_append(&vIUB, &_0, PH_SEPARATE, "test/regexdna.zep", 37);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "(c|g)");
	zephir_array_append(&vIUBnew, &_0, PH_SEPARATE, "test/regexdna.zep", 37);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "/V/S");
	zephir_array_append(&vIUB, &_0, PH_SEPARATE, "test/regexdna.zep", 38);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "(a|c|g)");
	zephir_array_append(&vIUBnew, &_0, PH_SEPARATE, "test/regexdna.zep", 38);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "/W/S");
	zephir_array_append(&vIUB, &_0, PH_SEPARATE, "test/regexdna.zep", 39);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "(a|t)");
	zephir_array_append(&vIUBnew, &_0, PH_SEPARATE, "test/regexdna.zep", 39);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "/Y/S");
	zephir_array_append(&vIUB, &_0, PH_SEPARATE, "test/regexdna.zep", 40);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "(c|t)");
	zephir_array_append(&vIUBnew, &_0, PH_SEPARATE, "test/regexdna.zep", 40);
	ZEPHIR_INIT_VAR(&stuffToRemove);
	ZVAL_STRING(&stuffToRemove, "^>.*$|\n");
	ZEPHIR_INIT_NVAR(&discard);
	ZVAL_NULL(&discard);
	ZEPHIR_INIT_VAR(&contents);
	zephir_file_get_contents(&contents, path TSRMLS_CC);
	ZEPHIR_INIT_VAR(&initialLength);
	ZVAL_LONG(&initialLength, zephir_fast_strlen_ev(&contents));
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_CONCAT_SVS(&_1, "/", &stuffToRemove, "/mS");
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "");
	ZEPHIR_CALL_FUNCTION(&_2, "preg_replace", &_3, 72, &_1, &_0, &contents);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(&contents, &_2);
	ZEPHIR_INIT_VAR(&codeLength);
	ZVAL_LONG(&codeLength, zephir_fast_strlen_ev(&contents));
	zephir_is_iterable(&variants, 0, "test/regexdna.zep", 59);
	if (Z_TYPE_P(&variants) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&variants), _4)
		{
			ZEPHIR_INIT_NVAR(&regex);
			ZVAL_COPY(&regex, _4);
			zend_print_zval(&regex, 0);
			php_printf("%s", " ");
			ZEPHIR_INIT_NVAR(&_6$$3);
			ZEPHIR_CONCAT_SVS(&_6$$3, "/", &regex, "/iS");
			ZEPHIR_INIT_NVAR(&_7$$3);
			zephir_preg_match(&_7$$3, &_6$$3, &contents, &discard, 1, 0 , 0  TSRMLS_CC);
			zend_print_zval(&_7$$3, 0);
			php_printf("%c", '\n');
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &variants, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_5, &variants, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_5)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&regex, &variants, "current", NULL, 0);
			zephir_check_call_status();
				zend_print_zval(&regex, 0);
				php_printf("%s", " ");
				ZEPHIR_INIT_NVAR(&_8$$4);
				ZEPHIR_CONCAT_SVS(&_8$$4, "/", &regex, "/iS");
				ZEPHIR_INIT_NVAR(&_9$$4);
				zephir_preg_match(&_9$$4, &_8$$4, &contents, &discard, 1, 0 , 0  TSRMLS_CC);
				zend_print_zval(&_9$$4, 0);
				php_printf("%c", '\n');
			ZEPHIR_CALL_METHOD(NULL, &variants, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_INIT_NVAR(&regex);
	ZEPHIR_CALL_FUNCTION(&_2, "preg_replace", &_3, 72, &vIUB, &vIUBnew, &contents);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(&contents, &_2);
	php_printf("%c", '\n');
	zend_print_zval(&initialLength, 0);
	php_printf("%c", '\n');
	zend_print_zval(&codeLength, 0);
	php_printf("%c", '\n');
	php_printf("%d", zephir_fast_strlen_ev(&contents));
	php_printf("%c", '\n');
	ZEPHIR_MM_RESTORE();

}

