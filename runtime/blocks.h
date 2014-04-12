
#ifndef PHP_ZEPHIR_RUNTIME_BLOCKS_H
#define PHP_ZEPHIR_RUNTIME_BLOCKS_H 1

LLVMValueRef zephir_compile_block(zephir_context *context, zval *statements TSRMLS_DC);

#endif
