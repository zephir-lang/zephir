

int zephir_initialize_zval_struct(zephir_context *context);
LLVMValueRef zephir_build_zend_is_true(zephir_context *context, LLVMValueRef value_ref);
void zephir_build_return_long(zephir_context *context, LLVMValueRef value_ref);