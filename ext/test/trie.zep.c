
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

	zephir_nts_static zend_class_entry *_4 = NULL;
	int i, ZEPHIR_LAST_CALL_STATUS;
	zend_bool create;
	zval *key_param = NULL, *create_param = NULL, *trieNode = NULL, *keyLen, *character = NULL, _0 = zval_used_for_init, _1 = zval_used_for_init, *_2 = NULL, *_3 = NULL, *_5 = NULL, *_6;
	zval *key = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &key_param, &create_param);

	zephir_get_strval(key, key_param);
	if (!create_param) {
		create = 0;
	} else {
		create = zephir_get_boolval(create_param);
	}


	ZEPHIR_OBS_VAR(trieNode);
	zephir_read_property_this(&trieNode, this_ptr, SL("trie"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(keyLen);
	ZVAL_LONG(keyLen, zephir_fast_strlen_ev(key));
	i = 0;
	while (1) {
		if (!(ZEPHIR_GT_LONG(keyLen, i))) {
			break;
		}
		ZEPHIR_SINIT_NVAR(_0);
		ZVAL_LONG(&_0, i);
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_LONG(&_1, 1);
		ZEPHIR_INIT_NVAR(character);
		zephir_substr(character, key, zephir_get_intval(&_0), 1 , 0);
		ZEPHIR_OBS_NVAR(_2);
		zephir_read_property(&_2, trieNode, SL("children"), PH_NOISY_CC);
		if (!(zephir_array_isset(_2, character))) {
			if (create) {
				ZEPHIR_INIT_NVAR(_3);
				if (!_4) {
					_4 = zend_fetch_class(SL("tries\\trienode"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
				}
				object_init_ex(_3, _4);
				if (zephir_has_constructor(_3 TSRMLS_CC)) {
					ZEPHIR_CALL_METHOD(NULL, _3, "__construct", NULL);
					zephir_check_call_status();
				}
				zephir_update_property_array(trieNode, SL("children"), character, _3 TSRMLS_CC);
			} else {
				RETURN_MM_BOOL(0);
			}
		}
		ZEPHIR_OBS_NVAR(_5);
		zephir_read_property(&_5, trieNode, SL("children"), PH_NOISY_CC);
		zephir_array_fetch(&_6, _5, character, PH_NOISY | PH_READONLY, "test/trie.zep", 24 TSRMLS_CC);
		ZEPHIR_CPY_WRT(trieNode, _6);
		i += 1;
	}
	RETURN_CCTOR(trieNode);

}

