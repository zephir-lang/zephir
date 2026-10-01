
extern zend_class_entry *stub_issue2676_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2676);

PHP_METHOD(Stub_Issue2676, modLongLong);
PHP_METHOD(Stub_Issue2676, modLongVar);
PHP_METHOD(Stub_Issue2676, modVarLong);
PHP_METHOD(Stub_Issue2676, modVarVar);
PHP_METHOD(Stub_Issue2676, modLongDouble);
PHP_METHOD(Stub_Issue2676, modDoubleLong);
PHP_METHOD(Stub_Issue2676, modDoubleDouble);
PHP_METHOD(Stub_Issue2676, modVarDouble);
PHP_METHOD(Stub_Issue2676, modDoubleVar);
PHP_METHOD(Stub_Issue2676, modVarLiteralDouble);
PHP_METHOD(Stub_Issue2676, modVarLiteralLong);
PHP_METHOD(Stub_Issue2676, modLiteral);
PHP_METHOD(Stub_Issue2676, modLongBool);
PHP_METHOD(Stub_Issue2676, modBoolLong);
PHP_METHOD(Stub_Issue2676, modVarBool);
PHP_METHOD(Stub_Issue2676, modBoolVar);
PHP_METHOD(Stub_Issue2676, modDoubleBool);
PHP_METHOD(Stub_Issue2676, modBoolDouble);
PHP_METHOD(Stub_Issue2676, modBoolBool);
PHP_METHOD(Stub_Issue2676, modLongByTrue);
PHP_METHOD(Stub_Issue2676, modTrueByLong);
PHP_METHOD(Stub_Issue2676, modVarByTrue);
PHP_METHOD(Stub_Issue2676, modInferredLocal);
PHP_METHOD(Stub_Issue2676, modTypedLong);
PHP_METHOD(Stub_Issue2676, modAssignVarLong);
PHP_METHOD(Stub_Issue2676, modAssignVarVar);
PHP_METHOD(Stub_Issue2676, modAssignInferredLocal);
PHP_METHOD(Stub_Issue2676, modAssignTypedLong);
PHP_METHOD(Stub_Issue2676, modAssignTypedLongVar);
PHP_METHOD(Stub_Issue2676, modAssignTypedDouble);
PHP_METHOD(Stub_Issue2676, modAssignProperty);
PHP_METHOD(Stub_Issue2676, modAssignPropertyVar);
PHP_METHOD(Stub_Issue2676, modAssignPropertyLiteral);
PHP_METHOD(Stub_Issue2676, divAssignTypedLong);
PHP_METHOD(Stub_Issue2676, divAssignTypedDouble);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modlonglong, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modlongvar, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modvarlong, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modvarvar, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modlongdouble, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_moddoublelong, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_moddoubledouble, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modvardouble, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_moddoublevar, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modvarliteraldouble, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modvarliterallong, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modliteral, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modlongbool, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modboollong, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modvarbool, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, b, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modboolvar, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_moddoublebool, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modbooldouble, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modboolbool, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, b, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modlongbytrue, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modtruebylong, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modvarbytrue, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modinferredlocal, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modtypedlong, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modassignvarlong, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modassignvarvar, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modassigninferredlocal, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modassigntypedlong, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modassigntypedlongvar, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modassigntypeddouble, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modassignproperty, 0, 0, 2)
	ZEND_ARG_INFO(0, obj)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modassignpropertyvar, 0, 0, 2)
	ZEND_ARG_INFO(0, obj)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_modassignpropertyliteral, 0, 0, 1)
	ZEND_ARG_INFO(0, obj)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_divassigntypedlong, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2676_divassigntypeddouble, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2676_method_entry) {
	PHP_ME(Stub_Issue2676, modLongLong, arginfo_stub_issue2676_modlonglong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modLongVar, arginfo_stub_issue2676_modlongvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modVarLong, arginfo_stub_issue2676_modvarlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modVarVar, arginfo_stub_issue2676_modvarvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modLongDouble, arginfo_stub_issue2676_modlongdouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modDoubleLong, arginfo_stub_issue2676_moddoublelong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modDoubleDouble, arginfo_stub_issue2676_moddoubledouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modVarDouble, arginfo_stub_issue2676_modvardouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modDoubleVar, arginfo_stub_issue2676_moddoublevar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modVarLiteralDouble, arginfo_stub_issue2676_modvarliteraldouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modVarLiteralLong, arginfo_stub_issue2676_modvarliterallong, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2676, modLiteral, arginfo_stub_issue2676_modliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modLongBool, arginfo_stub_issue2676_modlongbool, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modBoolLong, arginfo_stub_issue2676_modboollong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modVarBool, arginfo_stub_issue2676_modvarbool, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modBoolVar, arginfo_stub_issue2676_modboolvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modDoubleBool, arginfo_stub_issue2676_moddoublebool, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modBoolDouble, arginfo_stub_issue2676_modbooldouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modBoolBool, arginfo_stub_issue2676_modboolbool, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modLongByTrue, arginfo_stub_issue2676_modlongbytrue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modTrueByLong, arginfo_stub_issue2676_modtruebylong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modVarByTrue, arginfo_stub_issue2676_modvarbytrue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modInferredLocal, arginfo_stub_issue2676_modinferredlocal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modTypedLong, arginfo_stub_issue2676_modtypedlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modAssignVarLong, arginfo_stub_issue2676_modassignvarlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modAssignVarVar, arginfo_stub_issue2676_modassignvarvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modAssignInferredLocal, arginfo_stub_issue2676_modassigninferredlocal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modAssignTypedLong, arginfo_stub_issue2676_modassigntypedlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modAssignTypedLongVar, arginfo_stub_issue2676_modassigntypedlongvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modAssignTypedDouble, arginfo_stub_issue2676_modassigntypeddouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modAssignProperty, arginfo_stub_issue2676_modassignproperty, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modAssignPropertyVar, arginfo_stub_issue2676_modassignpropertyvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, modAssignPropertyLiteral, arginfo_stub_issue2676_modassignpropertyliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, divAssignTypedLong, arginfo_stub_issue2676_divassigntypedlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2676, divAssignTypedDouble, arginfo_stub_issue2676_divassigntypeddouble, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
