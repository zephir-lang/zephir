
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"

// top statement before namespace, add to after headers
#define MAX_FACTOR 10


// top statement before class, add to after headers
// test include .h
#include "kernel/require.h"



// c implementation of fibonacci
static long fibonacci(long n) {
	if (n < 2) return n;
	else return fibonacci(n - 2) + fibonacci(n - 1);
}




/**
 * CBLOCK tests
 */
ZEPHIR_INIT_CLASS(Test_Cblock) {

	ZEPHIR_REGISTER_CLASS(Test, Cblock, test, cblock, test_cblock_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Cblock, testCblock1) {

	int a = 0;


	
			a = MAX_FACTOR;
		
	RETURN_LONG(a);

}

PHP_METHOD(Test_Cblock, testCblock2) {

	long a = 0;


	
			a = fibonacci(MAX_FACTOR);
		
	RETURN_LONG(a);

}

