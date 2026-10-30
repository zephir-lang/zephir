
extern zend_class_entry *stub_issue2723_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2723);

PHP_METHOD(Stub_Issue2723, target);
PHP_METHOD(Stub_Issue2723, calledClass);
PHP_METHOD(Stub_Issue2723, withArgument);
PHP_METHOD(Stub_Issue2723, instanceTarget);
PHP_METHOD(Stub_Issue2723, viaParam);
PHP_METHOD(Stub_Issue2723, viaLocal);
PHP_METHOD(Stub_Issue2723, viaStringParam);
PHP_METHOD(Stub_Issue2723, viaMixedParam);
PHP_METHOD(Stub_Issue2723, viaMixedClass);
PHP_METHOD(Stub_Issue2723, viaParamWithArgument);
PHP_METHOD(Stub_Issue2723, assignViaParam);
PHP_METHOD(Stub_Issue2723, statementDynamicName);
PHP_METHOD(Stub_Issue2723, statementLiteralName);
PHP_METHOD(Stub_Issue2723, missingClassSkipsArguments);
PHP_METHOD(Stub_Issue2723, assignToName);
PHP_METHOD(Stub_Issue2723, assignToMethodName);
PHP_METHOD(Stub_Issue2723, methodViaParam);
PHP_METHOD(Stub_Issue2723, methodViaMixedParam);
PHP_METHOD(Stub_Issue2723, selfDynamic);
PHP_METHOD(Stub_Issue2723, selfDynamicStatic);
PHP_METHOD(Stub_Issue2723, selfDynamicStatement);
PHP_METHOD(Stub_Issue2723, parentDynamic);
PHP_METHOD(Stub_Issue2723, parentDynamicWithArgument);
PHP_METHOD(Stub_Issue2723, namedDynamic);
PHP_METHOD(Stub_Issue2723, namedDynamicStatic);
PHP_METHOD(Stub_Issue2723, internalDynamic);
PHP_METHOD(Stub_Issue2723, selfString);
PHP_METHOD(Stub_Issue2723, staticString);
PHP_METHOD(Stub_Issue2723, namedString);
PHP_METHOD(Stub_Issue2723, constantString);
PHP_METHOD(Stub_Issue2723, dynamicClassString);
PHP_METHOD(Stub_Issue2723, parentResultIntoParameter);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2723_target, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2723_calledclass, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2723_withargument, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2723_instancetarget, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_viaparam, 0, 0, 2)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_vialocal, 0, 0, 2)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_viastringparam, 0, 0, 2)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_viamixedparam, 0, 0, 2)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_viamixedclass, 0, 0, 1)
	ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_viaparamwithargument, 0, 0, 3)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_assignviaparam, 0, 0, 2)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_statementdynamicname, 0, 0, 3)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, counter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_statementliteralname, 0, 0, 2)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_INFO(0, counter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_missingclassskipsarguments, 0, 0, 2)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_INFO(0, counter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_assigntoname, 0, 0, 2)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_assigntomethodname, 0, 0, 2)
	ZEND_ARG_INFO(0, receiver)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_methodviaparam, 0, 0, 2)
	ZEND_ARG_INFO(0, receiver)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_methodviamixedparam, 0, 0, 2)
	ZEND_ARG_INFO(0, receiver)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_selfdynamic, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_selfdynamicstatic, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_selfdynamicstatement, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, counter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_parentdynamic, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_parentdynamicwithargument, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_nameddynamic, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_nameddynamicstatic, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_internaldynamic, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_selfstring, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_staticstring, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_namedstring, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_constantstring, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_dynamicclassstring, 0, 0, 1)
	ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723_parentresultintoparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, result)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2723_method_entry) {
	PHP_ME(Stub_Issue2723, target, arginfo_stub_issue2723_target, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2723, calledClass, arginfo_stub_issue2723_calledclass, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2723, withArgument, arginfo_stub_issue2723_withargument, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2723, instanceTarget, arginfo_stub_issue2723_instancetarget, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, viaParam, arginfo_stub_issue2723_viaparam, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, viaLocal, arginfo_stub_issue2723_vialocal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, viaStringParam, arginfo_stub_issue2723_viastringparam, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, viaMixedParam, arginfo_stub_issue2723_viamixedparam, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, viaMixedClass, arginfo_stub_issue2723_viamixedclass, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, viaParamWithArgument, arginfo_stub_issue2723_viaparamwithargument, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, assignViaParam, arginfo_stub_issue2723_assignviaparam, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, statementDynamicName, arginfo_stub_issue2723_statementdynamicname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, statementLiteralName, arginfo_stub_issue2723_statementliteralname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, missingClassSkipsArguments, arginfo_stub_issue2723_missingclassskipsarguments, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, assignToName, arginfo_stub_issue2723_assigntoname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, assignToMethodName, arginfo_stub_issue2723_assigntomethodname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, methodViaParam, arginfo_stub_issue2723_methodviaparam, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, methodViaMixedParam, arginfo_stub_issue2723_methodviamixedparam, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, selfDynamic, arginfo_stub_issue2723_selfdynamic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, selfDynamicStatic, arginfo_stub_issue2723_selfdynamicstatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2723, selfDynamicStatement, arginfo_stub_issue2723_selfdynamicstatement, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, parentDynamic, arginfo_stub_issue2723_parentdynamic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, parentDynamicWithArgument, arginfo_stub_issue2723_parentdynamicwithargument, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, namedDynamic, arginfo_stub_issue2723_nameddynamic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, namedDynamicStatic, arginfo_stub_issue2723_nameddynamicstatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2723, internalDynamic, arginfo_stub_issue2723_internaldynamic, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2723, selfString, arginfo_stub_issue2723_selfstring, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2723, staticString, arginfo_stub_issue2723_staticstring, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2723, namedString, arginfo_stub_issue2723_namedstring, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2723, constantString, arginfo_stub_issue2723_constantstring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, dynamicClassString, arginfo_stub_issue2723_dynamicclassstring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723, parentResultIntoParameter, arginfo_stub_issue2723_parentresultintoparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
