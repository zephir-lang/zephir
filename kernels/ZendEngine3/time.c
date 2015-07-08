
/*
 +------------------------------------------------------------------------+
 | Zephir Language                                                        |
 +------------------------------------------------------------------------+
 | Copyright (c) 2011-2015 Zephir Team  (http://www.zephir-lang.com)      |
 +------------------------------------------------------------------------+
 | This source file is subject to the New BSD License that is bundled     |
 | with this package in the file docs/LICENSE.txt.                        |
 |                                                                        |
 | If you did not receive a copy of the license and are unable to         |
 | obtain it through the world-wide-web, please send an email             |
 | to license@zephir-lang.com so we can send you a copy immediately.      |
 +------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef PHP_WIN32
#include "win32/time.h"
#elif defined(NETWARE)
#include <sys/timeval.h>
#include <sys/time.h>
#else
#include <sys/time.h>
#endif

#include <ctype.h>

#include "php.h"
#include "php_ext.h"

#include "kernel/main.h"
#include "kernel/time.h"
#include "kernel/operators.h"

