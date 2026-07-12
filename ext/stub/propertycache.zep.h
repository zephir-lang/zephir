
extern zend_class_entry *stub_propertycache_ce;

ZEPHIR_INIT_CLASS(Stub_PropertyCache);

PHP_METHOD(Stub_PropertyCache, getPub);
PHP_METHOD(Stub_PropertyCache, setPub);
PHP_METHOD(Stub_PropertyCache, getProt);
PHP_METHOD(Stub_PropertyCache, setProt);
PHP_METHOD(Stub_PropertyCache, getPriv);
PHP_METHOD(Stub_PropertyCache, setPriv);
PHP_METHOD(Stub_PropertyCache, incPub);
PHP_METHOD(Stub_PropertyCache, sumPubTwice);
PHP_METHOD(Stub_PropertyCache, swapArrayScalar);
PHP_METHOD(Stub_PropertyCache, branchedRead);
PHP_METHOD(Stub_PropertyCache, readLoop);
PHP_METHOD(Stub_PropertyCache, mutateInLoop);
PHP_METHOD(Stub_PropertyCache, accumulateInLoop);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycache_getpub, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycache_setpub, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycache_getprot, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycache_setprot, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycache_getpriv, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycache_setpriv, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycache_incpub, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycache_sumpubtwice, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycache_swaparrayscalar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycache_branchedread, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycache_readloop, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycache_mutateinloop, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycache_accumulateinloop, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_propertycache_method_entry) {
PHP_ME(Stub_PropertyCache, getPub, arginfo_stub_propertycache_getpub, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_PropertyCache, setPub, arginfo_stub_propertycache_setpub, ZEND_ACC_PUBLIC)
PHP_ME(Stub_PropertyCache, getProt, arginfo_stub_propertycache_getprot, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_PropertyCache, setProt, arginfo_stub_propertycache_setprot, ZEND_ACC_PUBLIC)
PHP_ME(Stub_PropertyCache, getPriv, arginfo_stub_propertycache_getpriv, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_PropertyCache, setPriv, arginfo_stub_propertycache_setpriv, ZEND_ACC_PUBLIC)
PHP_ME(Stub_PropertyCache, incPub, arginfo_stub_propertycache_incpub, ZEND_ACC_PUBLIC)
PHP_ME(Stub_PropertyCache, sumPubTwice, arginfo_stub_propertycache_sumpubtwice, ZEND_ACC_PUBLIC)
PHP_ME(Stub_PropertyCache, swapArrayScalar, arginfo_stub_propertycache_swaparrayscalar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_PropertyCache, branchedRead, arginfo_stub_propertycache_branchedread, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_PropertyCache, readLoop, arginfo_stub_propertycache_readloop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_PropertyCache, mutateInLoop, arginfo_stub_propertycache_mutateinloop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_PropertyCache, accumulateInLoop, arginfo_stub_propertycache_accumulateinloop, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
