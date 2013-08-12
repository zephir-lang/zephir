
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/operators.h"


/**
 * Control Flow
 */
ZEPHIR_INIT_CLASS(Test_Flow) {

	ZEPHIR_REGISTER_CLASS(Test, Flow, flow, test_flow_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Flow, testIf1) {

	long a;

	a = 1;

	if (a) {

		RETURN_LONG(1);

	}

	RETURN_LONG(0);


}

PHP_METHOD(Test_Flow, testIf2) {

	long a;

	a = 0;

	if (a) {

		RETURN_LONG(1);

	}

	RETURN_LONG(0);


}

PHP_METHOD(Test_Flow, testIf3) {

	double a;

	a = (double) (1);

	if (a) {

		RETURN_LONG(1);

	}

	RETURN_LONG(0);


}

PHP_METHOD(Test_Flow, testIf4) {

	double a;

	a = (double) (0);

	if (a) {

		RETURN_LONG(1);

	}

	RETURN_LONG(0);


}

PHP_METHOD(Test_Flow, testIf5) {

	zend_bool a;

	a = (1) ? 1 : 0;

	if (a) {

		RETURN_LONG(1);

	}

	RETURN_LONG(0);


}

PHP_METHOD(Test_Flow, testIf6) {

	zend_bool a;

	a = (0) ? 1 : 0;

	if (a) {

		RETURN_LONG(1);

	}

	RETURN_LONG(0);


}

PHP_METHOD(Test_Flow, testIf7) {

	long a;

	a = 1;

	if (a) {

		RETURN_LONG(1);

	} else {

		RETURN_LONG(0);

	}

}

PHP_METHOD(Test_Flow, testIf8) {

	long a;

	a = 0;

	if (a) {

		RETURN_LONG(1);

	} else {

		RETURN_LONG(0);

	}

}

PHP_METHOD(Test_Flow, testIf9) {

	zend_bool c;
	double b;
	long a;

	a = 1;

	if (a) {

		b = (double) (1);

		if (b) {

			c = (1) ? 1 : 0;

			if (c) {

				RETURN_LONG(1);

			}
		}
	}

	RETURN_LONG(0);


}

PHP_METHOD(Test_Flow, testIf10) {

	zend_bool c;
	double b;
	long a;

	a = 1;

	if (a) {

		b = (double) (1);

		if (b) {

			c = (1) ? 1 : 0;

			if (c) {

				RETURN_LONG(1);

			} else {

				RETURN_LONG(0);

			}
		} else {

			RETURN_LONG(0);

		}
	} else {

		RETURN_LONG(0);

	}

}

PHP_METHOD(Test_Flow, testIf12) {

	zend_bool c;
	double b;
	long a;

	a = 1;

	if (a) {

		b = (double) (1);

		if (b) {

			c = (1) ? 1 : 0;

			if (c) {

				RETURN_LONG(1);

			} else {

				RETURN_LONG(0);

			}
		} else {

			RETURN_LONG(0);

		}
	} else {

		RETURN_LONG(0);

	}

}

PHP_METHOD(Test_Flow, testLoop1) {

	zval a;

	ZEPHIR_SINIT_VAR(a);
	ZVAL_BOOL(&a, 1);

	while (1) {
		if (zend_is_true(&a)) {
			break;
		}
	}

	RETURN_CCTORW(&a);


}

PHP_METHOD(Test_Flow, testLoop2) {

	long a;

	a = 1;

	while (1) {

		a = a + 1;

		if ((a == 5)) {
			break;
		}
	}

	RETURN_LONG(a);


}

PHP_METHOD(Test_Flow, testLoop3) {

	long a;

	a = 1;

	while (1) {

		a = a + 1;

		if ((a == 3)) {

			a = a + 1;

			continue;
		}
		if ((a == 5)) {
			break;
		}
	}

	RETURN_LONG(a);


}

PHP_METHOD(Test_Flow, testLoop4) {

	long a;

	a = 1;

	while (1) {

		a = a + 1;

		if ((a == 3)) {

			a = a + 1;

			continue;
		}
		if ((a == 5)) {
			break;
		}
	}

	RETURN_LONG(a);


}

PHP_METHOD(Test_Flow, testWhile1) {

	long a;

	a = 5;

	while (a) {

		a = a - 1;

	}

	RETURN_LONG(a);


}

PHP_METHOD(Test_Flow, testWhile2) {

	long a;

	a = 5;

	while (a) {

		a = a - 1;

	}

	RETURN_LONG(a);


}

PHP_METHOD(Test_Flow, testWhile3) {

	double c, _0;
	zval b;
	long a;

	a = 5;

	while (a) {

		ZEPHIR_SINIT_NVAR(b);
		ZVAL_LONG(&b, 5);

		while (zend_is_true(&b)) {

			_0 = zephir_get_doubleval(&b) - 1;
			ZEPHIR_SINIT_NVAR(b);
			ZVAL_DOUBLE(&b, _0);

		}

		a = a - 1;

	}

	c = (double) (a + zephir_get_intval(&b));

	RETURN_DOUBLE(c);


}

PHP_METHOD(Test_Flow, testWhile4) {

	double c, _0;
	zval b;
	long a;

	a = 5;

	while (a) {

		ZEPHIR_SINIT_NVAR(b);
		ZVAL_LONG(&b, 5);

		while (zend_is_true(&b)) {

			_0 = zephir_get_doubleval(&b) - 1;
			ZEPHIR_SINIT_NVAR(b);
			ZVAL_DOUBLE(&b, _0);

		}

		a = a - 1;

	}

	c = (double) (a + zephir_get_intval(&b));

	RETURN_DOUBLE(c);


}

PHP_METHOD(Test_Flow, testWhile5) {

	long a;

	a = 5;

	while (a) {

		a--;

	}

	RETURN_LONG(a);


}

PHP_METHOD(Test_Flow, testWhile6) {

	long a;

	a = 5;

	while (a) {

		a--;

	}

	RETURN_LONG(a);


}

PHP_METHOD(Test_Flow, testWhile7) {

	double a;

	a = (double) (5);

	while (a) {

		a--;

	}

	RETURN_DOUBLE(a);


}

PHP_METHOD(Test_Flow, testWhile8) {

	double a;

	a = (double) (5);

	while (a) {

		a--;

	}

	RETURN_DOUBLE(a);


}

PHP_METHOD(Test_Flow, testWhile9) {

	double c;
	zval b;
	long a;

	a = 5;

	while (a) {

		ZEPHIR_SINIT_NVAR(b);
		ZVAL_LONG(&b, 5);

		while (zend_is_true(&b)) {

			ZEPHIR_SINIT_NVAR(b);
			zephir_decrement(&b);

		}

		a--;

	}

	c = (double) (a + zephir_get_intval(&b));

	RETURN_DOUBLE(c);


}

PHP_METHOD(Test_Flow, testFor) {


}

