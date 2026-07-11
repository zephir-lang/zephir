
extern zend_class_entry *stub_propertycachechild_ce;

ZEPHIR_INIT_CLASS(Stub_PropertyCacheChild);

PHP_METHOD(Stub_PropertyCacheChild, getChildProp);
PHP_METHOD(Stub_PropertyCacheChild, getInheritedPub);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycachechild_getchildprop, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycachechild_getinheritedpub, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_propertycachechild_method_entry) {
PHP_ME(Stub_PropertyCacheChild, getChildProp, arginfo_stub_propertycachechild_getchildprop, ZEND_ACC_PUBLIC)
PHP_ME(Stub_PropertyCacheChild, getInheritedPub, arginfo_stub_propertycachechild_getinheritedpub, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
