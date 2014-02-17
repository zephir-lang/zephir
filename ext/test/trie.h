
extern zend_class_entry *test_trie_ce;

ZEPHIR_INIT_CLASS(Test_Trie);

PHP_METHOD(Test_Trie, getTrieNodeByKey);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_trie_gettrienodebykey, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, create)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_trie_method_entry) {
	PHP_ME(Test_Trie, getTrieNodeByKey, arginfo_test_trie_gettrienodebykey, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
