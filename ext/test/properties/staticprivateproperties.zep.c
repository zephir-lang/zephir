
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Test_Properties_StaticPrivateProperties) {

	ZEPHIR_REGISTER_CLASS(Test\\Properties, StaticPrivateProperties, test, properties_staticprivateproperties, test_properties_staticprivateproperties_method_entry, 0);

	zend_declare_property_null(test_properties_staticprivateproperties_ce, SL("instance"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Properties_StaticPrivateProperties, getInstance) {

	zval localInstance, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&localInstance);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(&localInstance);
	zephir_read_static_property_ce(&localInstance, test_properties_staticprivateproperties_ce, SL("instance"), PH_NOISY_CC);
	if (UNEXPECTED(Z_TYPE_P(&localInstance) != IS_OBJECT)) {
		ZEPHIR_INIT_NVAR(&localInstance);
		object_init_ex(&localInstance, test_properties_staticprivateproperties_ce);
		if (zephir_has_constructor(&localInstance TSRMLS_CC)) {
			ZEPHIR_CALL_METHOD(NULL, &localInstance, "__construct", NULL, 0);
			zephir_check_call_status();
		}
		zephir_update_static_property_ce(test_properties_staticprivateproperties_ce, ZEND_STRL("instance"), &localInstance);
	}
	zephir_read_static_property_ce(&_0, test_properties_staticprivateproperties_ce, SL("instance"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_0);

}

