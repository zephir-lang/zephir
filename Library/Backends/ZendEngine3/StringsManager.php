<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Backends\ZendEngine3;

use Zephir\Utils;
use Zephir\StringsManager as BaseStringsManager;

/**
 * Class StringsManager
 *
 * Manages the concatenation keys for the extension and the interned strings
 */
class StringsManager extends BaseStringsManager
{
    /**
     * List of headers
     * @var array
     */
    protected $concatKeys = array(
        'vv' => true,
        'vs' => true,
        'sv' => true
    );

    /**
     * Adds a concatenation combination to the manager
     *
     * @param string $key
     */
    public function addConcatKey($key)
    {
        $this->concatKeys[$key] = true;
    }

    /**
     * Generates the concatenation code
     *
     * @return array
     */
    public function genConcatCode()
    {
         $code = '
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <php.h>
#include "php_ext.h"
#include <ext/standard/php_string.h>
#include "ext.h"
#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/concat.h"' . PHP_EOL . PHP_EOL;
        $pcodeh = '
#ifndef ZEPHIR_KERNEL_CONCAT_H
#define ZEPHIR_KERNEL_CONCAT_H
#include <php.h>
#include <Zend/zend.h>
#include "kernel/main.h"
';
        $codeh = '';

        $macros = array();
        ksort($this->concatKeys, SORT_STRING);
        foreach ($this->concatKeys as $key => $one) {
            $len = strlen($key);
            $params = array();
            $zvalCopy = array();
            $useCopy = array();
            $avars = array();
            $zvars = array();
            $svars = array();
            $lengths = array();
            $sparams = array();
            $lparams = array();
            for ($i = 0; $i < $len; $i++) {
                $n = $i + 1;
                $t = substr($key, $i, 1);
                $sparams[] = 'op' . $n;
                if ($t == 's') {
                    $params[] = 'const char *op' . $n . ', zend_uint op' . $n . '_len';
                    $lparams[] = 'op' . $n . ', sizeof(op' . $n . ')-1';
                    $lengths[] = 'op' . $n . '_len';
                    $svars[] = $n;
                    $avars[$n] = 's';
                } else {
                    $params[] = 'zval *op' . $n;
                    $lparams[] = 'op' . $n;
                    $zvalCopy[] = 'op' . $n . '_copy';
                    $useCopy[] = 'use_copy' . $n . ' = 0';
                    $lengths[] = 'Z_STRLEN_P(op' . $n . ')';
                    $zvars[] = $n;
                    $avars[$n] = 'v';
                }
            }

            $macros[] = '#define ZEPHIR_CONCAT_' . strtoupper($key) . '(result, ' . join(', ', $sparams) . ') \\' . PHP_EOL . "\t" . ' zephir_concat_' . $key . '(result, ' . join(', ', $lparams) . ', 0);';
            $macros[] = '#define ZEPHIR_SCONCAT_' . strtoupper($key) . '(result, ' . join(', ', $sparams) . ') \\' . PHP_EOL . "\t" . ' zephir_concat_' . $key . '(result, ' . join(', ', $lparams) . ', 1);';
            $macros[] = '';

            $proto = 'void zephir_concat_' . $key . '(zval *result, ' . join(', ', $params) . ', int self_var)';
            $proto = 'void zephir_concat_' . $key . '(zval *result, ' . join(', ', $params) . ', int self_var)';

            $codeh .= '' . $proto . ';' . PHP_EOL;

            $code .= $proto . '{' . PHP_EOL . PHP_EOL;

            if (count($zvalCopy)) {
                $code .= "\t" . 'zval result_copy, ' . join(', ', $zvalCopy) . ';' . PHP_EOL;
                $code .= "\t" . 'int use_copy = 0, ' . join(', ', $useCopy) . ';' . PHP_EOL;
            } else {
                $code .= "\t" . 'zval result_copy;' . PHP_EOL;
                $code .= "\t" . 'int use_copy = 0;' . PHP_EOL;
            }
            $code .= "\t" . 'uint offset = 0, length;' . PHP_EOL . PHP_EOL;

            foreach ($zvars as $zvar) {
                $code .= "\t" . 'if (Z_TYPE_P(op' . $zvar . ') != IS_STRING) {' . PHP_EOL;
                $code .= "\t" . '   use_copy' . $zvar . ' = zend_make_printable_zval(op' . $zvar . ', &op' . $zvar . '_copy);' . PHP_EOL;
                $code .= "\t" . '   if (use_copy' . $zvar . ') {' . PHP_EOL;
                $code .= "\t" . '       op' . $zvar . ' = &op' . $zvar . '_copy;' . PHP_EOL;
                $code .= "\t" . '   }' . PHP_EOL;
                $code .= "\t" . '}' . PHP_EOL . PHP_EOL;
            }

            $code .= "\t" . 'length = ' . join(' + ', $lengths) . ';' . PHP_EOL;
            $code .= "\t" . 'if (self_var) {' . PHP_EOL;
            $code .= '' .PHP_EOL;
            $code .= "\t\t" . 'if (Z_TYPE_P(result) != IS_STRING) {' . PHP_EOL;
            $code .= "\t\t\t" . 'use_copy = zend_make_printable_zval(result, &result_copy);' . PHP_EOL;
            $code .= "\t\t\t" . 'if (use_copy) {' . PHP_EOL;
            $code .= "\t\t\t\t" .'ZEPHIR_CPY_WRT_CTOR(result, (&result_copy));' . PHP_EOL;
            $code .= "\t\t\t" .'}'. PHP_EOL;
            $code .= "\t\t" . '}'. PHP_EOL . PHP_EOL;
            $code .= "\t\t" . 'offset = Z_STRLEN_P(result);' . PHP_EOL;
            $code .= "\t\t" . 'length += offset;' . PHP_EOL;
            $code .= "\t\t" . 'Z_STR_P(result) = zend_string_realloc(Z_STR_P(result), length, 0);' . PHP_EOL;
            $code .= '' . PHP_EOL;
            $code .= "\t" .'} else {' . PHP_EOL;
            $code .= "\t\t" . 'ZVAL_STR(result, zend_string_alloc(length, 0));' . PHP_EOL;
            $code .= "\t" . '}' . PHP_EOL . PHP_EOL;

            $position = '';
            foreach ($avars as $n => $type) {
                if ($type == 's') {
                    $code .= "\t" . 'memcpy(Z_STRVAL_P(result) + offset' . $position . ', op' . $n . ', op' . $n . '_len);' . PHP_EOL;
                    $position .= ' + op' . $n . '_len';
                } else {
                    $code .= "\t" . 'memcpy(Z_STRVAL_P(result) + offset' . $position . ', Z_STRVAL_P(op' . $n . '), Z_STRLEN_P(op' . $n . '));' . PHP_EOL;
                    $position .= ' + Z_STRLEN_P(op' . $n . ')';
                }
            }

            $code .= "\t" . 'Z_STRVAL_P(result)[length] = 0;' . PHP_EOL;
            $code .= "\t" . 'zend_string_forget_hash_val(Z_STR_P(result));' . PHP_EOL;

            foreach ($zvars as $zvar) {
                $code .= "\t" . 'if (use_copy' . $zvar . ') {' . PHP_EOL;
                $code .= "\t" . '   zval_dtor(op' . $zvar . ');' . PHP_EOL;
                $code .= "\t" . '}' . PHP_EOL . PHP_EOL;
            }

            $code .= "\t" . 'if (use_copy) {' . PHP_EOL;
            $code .= "\t" . '   zval_dtor(&result_copy);' . PHP_EOL;
            $code .= "\t" . '}' . PHP_EOL . PHP_EOL;

            $code .= "}" . PHP_EOL . PHP_EOL;
        }

        $code .= <<<EOF
void zephir_concat_function(zval *result, zval *op1, zval *op2)
{
	zval tmp;
	SEPARATE_ZVAL_IF_NOT_REF(result);

	/*
		res == op1 == op2: won't leak
		res == op1 != op2: won't leak
		res == op2 != op1: will leak
	 */
	if (result == op2 && result != op1) {
		ZVAL_COPY_VALUE(&tmp, result);
		ZVAL_NULL(result);
		op2 = &tmp;
	}
	else {
		ZVAL_UNDEF(&tmp);
	}

	concat_function(result, op1, op2);
	assert(!Z_REFCOUNTED(tmp) || 1 == Z_REFCOUNT(tmp));
	zval_dtor(&tmp);
}
EOF;


        $codeh .= "void zephir_concat_function(zval *result, zval *op1, zval *op2);
#endif /* ZEPHIR_KERNEL_CONCAT_H */
";
        Utils::checkAndWriteIfNeeded($pcodeh . join(PHP_EOL, $macros) . PHP_EOL . PHP_EOL . $codeh, 'ext/kernel/concat.h');
        Utils::checkAndWriteIfNeeded($code, 'ext/kernel/concat.c');
    }

    /**
     * Obtains the existing concatenation keys
     *
     * @return array
     */
    public function getConcatKeys()
    {
        return $this->concatKeys;
    }
}
