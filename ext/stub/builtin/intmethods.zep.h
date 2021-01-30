
extern zend_class_entry *stub_builtin_intmethods_ce;

ZEPHIR_INIT_CLASS(Stub_BuiltIn_IntMethods);

PHP_METHOD(Stub_BuiltIn_IntMethods, getAbs);
PHP_METHOD(Stub_BuiltIn_IntMethods, getAbs1);
PHP_METHOD(Stub_BuiltIn_IntMethods, getBinary);
PHP_METHOD(Stub_BuiltIn_IntMethods, getHex);
PHP_METHOD(Stub_BuiltIn_IntMethods, getOctal);
PHP_METHOD(Stub_BuiltIn_IntMethods, getPow);
PHP_METHOD(Stub_BuiltIn_IntMethods, getSqrt);
PHP_METHOD(Stub_BuiltIn_IntMethods, getExp);
PHP_METHOD(Stub_BuiltIn_IntMethods, getSin);
PHP_METHOD(Stub_BuiltIn_IntMethods, getCos);
PHP_METHOD(Stub_BuiltIn_IntMethods, getTan);
PHP_METHOD(Stub_BuiltIn_IntMethods, getAsin);
PHP_METHOD(Stub_BuiltIn_IntMethods, getAcos);
PHP_METHOD(Stub_BuiltIn_IntMethods, getAtan);
PHP_METHOD(Stub_BuiltIn_IntMethods, getLog);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_getabs, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_getabs1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_getbinary, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_gethex, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_getoctal, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_getpow, 0, 0, 2)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, exp, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, exp)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_getsqrt, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_getexp, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_getsin, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_getcos, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_gettan, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_getasin, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_getacos, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_getatan, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_intmethods_getlog, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, base, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, base)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_builtin_intmethods_method_entry) {
	PHP_ME(Stub_BuiltIn_IntMethods, getAbs, arginfo_stub_builtin_intmethods_getabs, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_IntMethods, getAbs1, arginfo_stub_builtin_intmethods_getabs1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_IntMethods, getBinary, arginfo_stub_builtin_intmethods_getbinary, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_IntMethods, getHex, arginfo_stub_builtin_intmethods_gethex, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_IntMethods, getOctal, arginfo_stub_builtin_intmethods_getoctal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_IntMethods, getPow, arginfo_stub_builtin_intmethods_getpow, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_IntMethods, getSqrt, arginfo_stub_builtin_intmethods_getsqrt, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_IntMethods, getExp, arginfo_stub_builtin_intmethods_getexp, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_IntMethods, getSin, arginfo_stub_builtin_intmethods_getsin, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_IntMethods, getCos, arginfo_stub_builtin_intmethods_getcos, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_IntMethods, getTan, arginfo_stub_builtin_intmethods_gettan, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_IntMethods, getAsin, arginfo_stub_builtin_intmethods_getasin, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_IntMethods, getAcos, arginfo_stub_builtin_intmethods_getacos, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_IntMethods, getAtan, arginfo_stub_builtin_intmethods_getatan, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_IntMethods, getLog, arginfo_stub_builtin_intmethods_getlog, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
