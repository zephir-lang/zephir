
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


ZEPHIR_INIT_CLASS(Stub_Properties_StaticPrivateProperties)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Properties, StaticPrivateProperties, stub, properties_staticprivateproperties, stub_properties_staticprivateproperties_method_entry, 0);

	zend_declare_property_null(stub_properties_staticprivateproperties_ce, SL("instance"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Properties_StaticPrivateProperties, getInstance)
{
	zval localInstance, _1, _0$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&localInstance);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0$$3);
	ZEND_TLS zephir_static_property_cache _zephir_sprop_cache[3];
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_memory_observe(&localInstance);
	zephir_read_static_property_cached(&localInstance, stub_properties_staticprivateproperties_ce, stub_properties_staticprivateproperties_ce, SL("instance"), &_zephir_sprop_cache[0], PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (UNEXPECTED(Z_TYPE_P(&localInstance) != IS_OBJECT)) {
		ZEPHIR_INIT_VAR(&_0$$3);
		object_init_ex(&_0$$3, stub_properties_staticprivateproperties_ce);
		if (zephir_has_constructor(&_0$$3)) {
			zephir_constructor_start(&_0$$3);
			ZEPHIR_CALL_METHOD_CACHED(NULL, &_0$$3, "__construct", &_zephir_call_cache[0]);
			zephir_check_call_status();
			zephir_constructor_finish(&_0$$3);
		}

		ZEPHIR_CPY_WRT(&localInstance, &_0$$3);
		ZEPHIR_INIT_NVAR(&_0$$3);
		zephir_update_static_property_cached(stub_properties_staticprivateproperties_ce, stub_properties_staticprivateproperties_ce, SL("instance"), &_zephir_sprop_cache[1], &localInstance);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	zephir_read_static_property_cached(&_1, stub_properties_staticprivateproperties_ce, stub_properties_staticprivateproperties_ce, SL("instance"), &_zephir_sprop_cache[2], PH_NOISY_CC | PH_READONLY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&_1);
}

