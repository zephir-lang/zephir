
extern zend_class_entry *stub_issue2522_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2522);

PHP_METHOD(Stub_Issue2522, inlineClosureIsCallable);
PHP_METHOD(Stub_Issue2522, inlineClosureReturnsClosureInstance);
PHP_METHOD(Stub_Issue2522, inlineFnKeywordIsCallable);
PHP_METHOD(Stub_Issue2522, twoStepClosureIsCallable);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2522_inlineclosureiscallable, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2522_inlineclosurereturnsclosureinstance, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2522_inlinefnkeywordiscallable, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2522_twostepclosureiscallable, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2522_method_entry) {
	PHP_ME(Stub_Issue2522, inlineClosureIsCallable, arginfo_stub_issue2522_inlineclosureiscallable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2522, inlineClosureReturnsClosureInstance, arginfo_stub_issue2522_inlineclosurereturnsclosureinstance, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2522, inlineFnKeywordIsCallable, arginfo_stub_issue2522_inlinefnkeywordiscallable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2522, twoStepClosureIsCallable, arginfo_stub_issue2522_twostepclosureiscallable, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
