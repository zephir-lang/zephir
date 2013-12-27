<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team and contributors                          |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

/**
 * Class StringsManager
 *
 * Manages the concatenation keys for the extension and the interned strings
 */
class StringsManager
{
	/**
	 * List of headers
	 * @var array
	 */
	protected $_concatKeys = array(
		'vv' => true,
		'vs' => true,
		'sv' => true
	);

	/**
	* Checks if the content of the file on the disk is the same as
	* the content.
	* Returns true if the file has been written
	* @param string $content
	* @param string $path
	*/
	protected function _checkAndWriteIfNeeded($content, $path)
	{
		if (file_exists($path)) {
			$content_md5 = md5($content);
			$existing_md5 = md5_file($path);

			if ($content_md5 != $existing_md5) {
				file_put_contents($path, $content);
				return true;
			}
		}

		return false;
	}

	/**
	* @param string $path
	*/
	public function addConcatKey($key)
	{
		$this->_concatKeys[$key] = true;
	}

	/**
	 * @return array
	 */
	public function genConcatCode()
	{

		$code = '
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ext.h"
#include "ext/standard/php_string.h"
#include "ext.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/concat.h"' . PHP_EOL . PHP_EOL;

		$codeh = '';

		$macros = array();
		ksort($this->_concatKeys, SORT_STRING);
		foreach ($this->_concatKeys as $key => $one){

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

			$macros[] = '#define ZEPHIR_CONCAT_' . strtoupper($key) . '(result, ' . join(', ', $sparams) . ') \\' . PHP_EOL . "\t" . ' zephir_concat_' . $key . '(&result, ' . join(', ', $lparams) . ', 0 TSRMLS_CC);';
			$macros[] = '#define ZEPHIR_SCONCAT_' . strtoupper($key) . '(result, ' . join(', ', $sparams) . ') \\' . PHP_EOL . "\t" . ' zephir_concat_' . $key . '(&result, ' . join(', ', $lparams) . ', 1 TSRMLS_CC);';
			$macros[] = '';

			$proto = 'void zephir_concat_' . $key . '(zval **result, ' . join(', ', $params) . ', int self_var TSRMLS_DC)';

			$codeh.= '' . $proto . ';' . PHP_EOL;

			$code .= $proto . '{' . PHP_EOL . PHP_EOL;

			if (count($zvalCopy)) {
				$code .= "\t" . 'zval result_copy, ' . join(', ', $zvalCopy) . ';' . PHP_EOL;
				$code .= "\t" . 'int use_copy = 0, ' . join(', ', $useCopy) . ';' . PHP_EOL;
			}
			$code .= "\t" . 'uint offset = 0, length;' . PHP_EOL . PHP_EOL;

			foreach ($zvars as $zvar) {
				$code .= "\t" . 'if (Z_TYPE_P(op' . $zvar . ') != IS_STRING) {' . PHP_EOL;
				$code .= "\t" . '	zend_make_printable_zval(op' . $zvar . ', &op' . $zvar . '_copy, &use_copy' . $zvar . ');' . PHP_EOL;
				$code .= "\t" . '	if (use_copy' . $zvar . ') {' . PHP_EOL;
				$code .= "\t" . '		op' . $zvar . ' = &op' . $zvar . '_copy;' . PHP_EOL;
				$code .= "\t" . '	}' . PHP_EOL;
				$code .= "\t" . '}' . PHP_EOL . PHP_EOL;
			}

			$code .= "\t" . 'length = ' . join(' + ', $lengths) . ';' . PHP_EOL;
			$code .= "\t" . 'if (self_var) {' . PHP_EOL;
			$code .= '' .PHP_EOL;
			$code .= "\t\t" . 'if (Z_TYPE_PP(result) != IS_STRING) {' . PHP_EOL;
			$code .= "\t\t\t" . 'zend_make_printable_zval(*result, &result_copy, &use_copy);' . PHP_EOL;
			$code .= "\t\t\t" . 'if (use_copy) {' . PHP_EOL;
			$code .= "\t\t\t\t" .'ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));' . PHP_EOL;
			$code .= "\t\t\t" .'}'. PHP_EOL;
			$code .= "\t\t" . '}'. PHP_EOL . PHP_EOL;
			$code .= "\t\t" . 'offset = Z_STRLEN_PP(result);' . PHP_EOL;
			$code .= "\t\t" . 'length += offset;' . PHP_EOL;
			$code .= "\t\t" . 'Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);' . PHP_EOL;
			$code .= '' . PHP_EOL;
			$code .= "\t" .'} else {' . PHP_EOL;
			$code .= "\t\t" . 'Z_STRVAL_PP(result) = (char *) emalloc(length + 1);' . PHP_EOL;
			$code .= "\t" . '}' . PHP_EOL . PHP_EOL;

			$position = '';
			foreach ($avars as $n => $type) {
				if ($type == 's') {
					$code .= "\t" . 'memcpy(Z_STRVAL_PP(result) + offset' . $position . ', op' . $n . ', op' . $n . '_len);' . PHP_EOL;
					$position .= ' + op' . $n . '_len';
				} else {
					$code .= "\t" . 'memcpy(Z_STRVAL_PP(result) + offset' . $position . ', Z_STRVAL_P(op' . $n . '), Z_STRLEN_P(op' . $n . '));' . PHP_EOL;
					$position .= ' + Z_STRLEN_P(op' . $n . ')';
				}
			}

			$code .= "\t" . 'Z_STRVAL_PP(result)[length] = 0;' . PHP_EOL;
			$code .= "\t" . 'Z_TYPE_PP(result) = IS_STRING;' . PHP_EOL;
			$code .= "\t" . 'Z_STRLEN_PP(result) = length;' . PHP_EOL . PHP_EOL;

			foreach ($zvars as $zvar) {
				$code .= "\t" . 'if (use_copy' . $zvar . ') {' . PHP_EOL;
				$code .= "\t" . '	zval_dtor(op' . $zvar . ');' . PHP_EOL;
				$code .= "\t" . '}' . PHP_EOL . PHP_EOL;
			}

			$code .= "\t" . 'if (use_copy) {' . PHP_EOL;
			$code .= "\t" . '	zval_dtor(&result_copy);' . PHP_EOL;
			$code .= "\t" . '}' . PHP_EOL . PHP_EOL;

			$code .= "}" . PHP_EOL . PHP_EOL;
		}

    	$this->_checkAndWriteIfNeeded(join(PHP_EOL, $macros) . PHP_EOL . PHP_EOL . $codeh, 'ext/kernel/concat.h');
    	$this->_checkAndWriteIfNeeded($code, 'ext/kernel/concat.c');
	}

	public function getConcatKeys()
	{
		return $this->_concatKeys;
	}
}
