
extern zend_class_entry *stub_issue2808counter_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2808Counter);

PHP_METHOD(Stub_Issue2808Counter, increment);
PHP_METHOD(Stub_Issue2808Counter, decrement);
PHP_METHOD(Stub_Issue2808Counter, addCounter);
PHP_METHOD(Stub_Issue2808Counter, appendText);
PHP_METHOD(Stub_Issue2808Counter, addPlain);
PHP_METHOD(Stub_Issue2808Counter, appendPlain);
PHP_METHOD(Stub_Issue2808Counter, incrementPlain);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808counter_increment, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808counter_decrement, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808counter_addcounter, 0, 0, 1)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808counter_appendtext, 0, 0, 1)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808counter_addplain, 0, 0, 1)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808counter_appendplain, 0, 0, 1)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808counter_incrementplain, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2808counter_method_entry) {
PHP_ME(Stub_Issue2808Counter, increment, arginfo_stub_issue2808counter_increment, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2808Counter, decrement, arginfo_stub_issue2808counter_decrement, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2808Counter, addCounter, arginfo_stub_issue2808counter_addcounter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2808Counter, appendText, arginfo_stub_issue2808counter_appendtext, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2808Counter, addPlain, arginfo_stub_issue2808counter_addplain, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2808Counter, appendPlain, arginfo_stub_issue2808counter_appendplain, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2808Counter, incrementPlain, arginfo_stub_issue2808counter_incrementplain, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
