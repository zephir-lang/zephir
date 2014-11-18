
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
#include "kernel/fcall.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Test_Trie) {

	ZEPHIR_REGISTER_CLASS(Test, Trie, test, trie, test_trie_method_entry, 0);

	zend_declare_property_null(test_trie_ce, SL("trie"), ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Trie, getTrieNodeByKey) {

	zephir_nts_static zend_class_entry *_5 = NULL;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	int i, ZEPHIR_LAST_CALL_STATUS;
	zend_bool create;
	zval *key_param = NULL, *create_param = NULL, *trieNode = NULL, *keyLen, *character = NULL, _0 = zval_used_for_init, _1 = zval_used_for_init, *_3 = NULL, *_4 = NULL, *_6 = NULL, *_7;
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
		ZEPHIR_CALL_FUNCTION(&character, "substr", &_2, key, &_0, &_1);
		zephir_check_call_status();
		ZEPHIR_OBS_NVAR(_3);
		zephir_read_property(&_3, trieNode, SL("children"), PH_NOISY_CC);
		if (!(zephir_array_isset(_3, character))) {
			if (create) {
				ZEPHIR_INIT_NVAR(_4);
				if (!_5) {
					_5 = zend_fetch_class(SL("tries\\trienode"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
				}
				object_init_ex(_4, _5);
				if (zephir_has_constructor(_4 TSRMLS_CC)) {
					ZEPHIR_CALL_METHOD(NULL, _4, "__construct", NULL);
					zephir_check_call_status();
				}
				zephir_update_property_array(trieNode, SL("children"), character, _4 TSRMLS_CC);
			} else {
				RETURN_MM_BOOL(0);
			}
		}
		ZEPHIR_OBS_NVAR(_6);
		zephir_read_property(&_6, trieNode, SL("children"), PH_NOISY_CC);
		zephir_array_fetch(&_7, _6, character, PH_NOISY | PH_READONLY, "test/trie.zep", 24 TSRMLS_CC);
		ZEPHIR_CPY_WRT(trieNode, _7);
		i += 1;
	}
	RETURN_CCTOR(trieNode);

}

