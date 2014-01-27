
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

	zend_class_entry *_3;
	int i;
	zend_bool create;
	zval *key_param = NULL, *create_param = NULL, *trieNode = NULL, *keyLen, *character = NULL, _0 = zval_used_for_init, *_1 = NULL, *_2 = NULL, *_4 = NULL;
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
		ZEPHIR_INIT_NVAR(character);
		zephir_substr(character, key, zephir_get_intval(&_0) , 1 );
		ZEPHIR_OBS_NVAR(_1);
		zephir_read_property(&_1, trieNode, SL("children"), PH_NOISY_CC);
		if (!(zephir_array_isset(_1, character))) {
			if (create) {
				ZEPHIR_INIT_LNVAR(_2);
				_3 = zend_fetch_class(SL("tries\\trienode"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
				object_init_ex(_2, _3);
				if (zephir_has_constructor(_2 TSRMLS_CC)) {
					zephir_call_method_noret(_2, "__construct");
				}
				zephir_update_property_array(trieNode, SL("children"), character, _2 TSRMLS_CC);
			} else {
				RETURN_MM_BOOL(0);
			}
		}
		ZEPHIR_OBS_NVAR(_4);
		zephir_read_property(&_4, trieNode, SL("children"), PH_NOISY_CC);
		ZEPHIR_OBS_NVAR(trieNode);
		zephir_array_fetch(&trieNode, _4, character, PH_NOISY TSRMLS_CC);
		i += 1;
	}
	RETURN_CCTOR(trieNode);

}

