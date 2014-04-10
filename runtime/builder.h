

int zephir_initialize_zval_struct(zephir_context *context);
LLVMValueRef zephir_build_zend_is_true(zephir_context *context, LLVMValueRef value_ref);

void zephir_build_return_long(zephir_context *context, LLVMValueRef value_ref);
void zephir_build_return_double(zephir_context *context, LLVMValueRef value_ref);

void zephir_build_zval_long(zephir_context *context, LLVMValueRef symbol_ref, LLVMValueRef value_ref);
void zephir_build_zval_double(zephir_context *context, LLVMValueRef symbol_ref, LLVMValueRef value_ref);

LLVMValueRef zephir_build_get_intval(zephir_context *context, LLVMValueRef symbol_ref);