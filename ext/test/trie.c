
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
#include "kernel/object.h"
#include "kernel/string.h"
#include "kernel/operators.h"
#include "kernel/array.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Test_Trie) {

	ZEPHIR_REGISTER_CLASS(Test, Trie, test, trie, test_trie_method_entry, 0);

	zend_declare_property_null(test_trie_ce, SL("trie"), ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Trie, getTrieNodeByKey) {

	zend_class_entry *_7;
	int i;
	zval *key_param = NULL, *create = NULL, *trieNode = NULL, *keyLen, *character = NULL, *_0, *_1, *_2, *_3 = NULL, _4 = zval_used_for_init, *_5 = NULL, *_6 = NULL, *_8 = NULL;
	zval *key = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &key_param, &create);

		zephir_get_strval(key, key_param);
	if (!create) {
		create = ZEPHIR_GLOBAL(global_false);
	}


	ZEPHIR_OBS_VAR(trieNode);
	zephir_read_property_this(&trieNode, this_ptr, SL("trie"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(keyLen);
	ZVAL_LONG(keyLen, zephir_fast_strlen_ev(key));
	i = 0;
	php_printf("key = [");
	zend_print_zval(key, 0);
	php_printf("]");
	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_GET_CONSTANT(_0, "PHP_EOL");
	zend_print_zval(_0, 0);
	php_printf("keyLen = [");
	zend_print_zval(keyLen, 0);
	php_printf("]");
	ZEPHIR_INIT_VAR(_1);
	ZEPHIR_GET_CONSTANT(_1, "PHP_EOL");
	zend_print_zval(_1, 0);
	php_printf("i = [");
	php_printf("%d", i);
	php_printf("]");
	ZEPHIR_INIT_VAR(_2);
	ZEPHIR_GET_CONSTANT(_2, "PHP_EOL");
	zend_print_zval(_2, 0);
	while (1) {
		if (!(ZEPHIR_GE_LONG(keyLen, i))) {
			break;
		}
		php_printf("In while loop");
		ZEPHIR_INIT_NVAR(_3);
		ZEPHIR_GET_CONSTANT(_3, "PHP_EOL");
		zend_print_zval(_3, 0);
		ZEPHIR_SINIT_NVAR(_4);
		ZVAL_LONG(&_4, i);
		ZEPHIR_INIT_NVAR(character);
		zephir_substr(character, key, zephir_get_intval(&_4) , 1 );
		ZEPHIR_OBS_NVAR(_5);
		zephir_read_property(&_5, trieNode, SL("children"), PH_NOISY_CC);
		if (!zephir_array_isset(_5, character)) {
			if (zephir_is_true(create)) {
				ZEPHIR_INIT_LNVAR(_6);
				_7 = zend_fetch_class(SL("tries\\trienode"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
				object_init_ex(_6, _7);
				if (zephir_has_constructor(_6 TSRMLS_CC)) {
					zephir_call_method_noret(_6, "__construct");
				}
				zephir_update_property_array(trieNode, SL("children"), character, _6 TSRMLS_CC);
			} else {
				RETURN_MM_BOOL(0);
			}
		}
		ZEPHIR_OBS_NVAR(_8);
		zephir_read_property(&_8, trieNode, SL("children"), PH_NOISY_CC);
		ZEPHIR_OBS_NVAR(trieNode);
		zephir_array_fetch(&trieNode, _8, character, PH_NOISY TSRMLS_CC);
		i += 1;
	}
	RETURN_CCTOR(trieNode);

}

