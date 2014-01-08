<?php

if (function_exists('apc_cache_info')) {
	return false;
}

/**
 * @link http://www.php.net/manual/en/function.apc-cache-info.php
 * @param string $type
 * @param bool $limited
 */
function apc_cache_info($type = '', $limited = false){}

/**
 * @link http://www.php.net/manual/en/function.apc-clear-cache.php
 * @param string $cache_type
 */
function apc_clear_cache($cache_type = ''){}

/**
 * @link http://www.php.net/manual/en/function.apc-sma-info.php
 * @param bool $limited
 */
function apc_sma_info($limited = false){}

/**
 * @link http://www.php.net/manual/en/function.apc-store.php
 * @param $key
 * @param $var
 * @param int $ttl
 */
function apc_store($key, $var, $ttl = 0){}

/**
 * @link http://www.php.net/manual/en/function.apc-fetch.php
 * @param $key
 * @param null $success
 */
function apc_fetch($key, &$success = null){}

/**
 * @link http://www.php.net/manual/en/function.apc-delete.php
 * @param $key
 */
function apc_delete($key){}

/**
 * @link http://www.php.net/manual/en/function.apc-define-constants.php
 * @param $key
 * @param array $constants
 * @param bool $case_sensitive
 */
function apc_define_constants($key, array $constants, $case_sensitive = true){}

/**
 * @link http://www.php.net/manual/en/function.apc-add.php
 * @param $key
 * @param $var
 * @param int $ttl
 */
function apc_add($key, $var, $ttl = 0){}

/**
 * @link http://www.php.net/manual/en/function.apc-compile-file.php
 * @param $filename
 * @param bool $atomic
 */
function apc_compile_file($filename, $atomic = true){}

/**
 * @link http://www.php.net/manual/en/function.apc-load-constants.php
 * @param $key
 * @param bool $case_sensitive
 */
function apc_load_constants($key, $case_sensitive = true){}

/**
 * @link http://www.php.net/manual/en/function.apc-exists.php
 * @param $keys
 */
function apc_exists($keys){}

/**
 * @link http://www.php.net/manual/en/function.apc-delete-file.php
 * @param $keys
 */
function apc_delete_file($keys){}

/**
 * @link http://www.php.net/manual/en/function.apc-inc.php
 * @param $key
 * @param int $step
 * @param null $success
 */
function apc_inc($key, $step = 1, &$success = null){}

/**
 * @link http://www.php.net/manual/en/function.apc-dec.php
 * @param $key
 * @param int $step
 * @param null $success
 */
function apc_dec($key, $step = 1, &$success = null){}

/**
 * @link http://php.net/manual/en/function.apc-cas.php
 * @param $key
 * @param $old
 * @param $new
 */
function apc_cas($key, $old, $new){}

/**
 * @link http://php.net/manual/en/function.apc-bin-dump.php
 * @param null $files
 * @param null $user_vars
 */
function apc_bin_dump($files = null, $user_vars = null){}

/**
 * @link http://php.net/manual/en/function.apc-bin-dumpfile.php
 * @param $files
 * @param $user_vars
 * @param $filename
 * @param int $flags
 * @param null $context
 */
function apc_bin_dumpfile($files, $user_vars, $filename, $flags = 0, $context = null){}

/**
 * @link http://php.net/manual/en/function.apc-bin-load.php
 * @param $data
 * @param int $flags
 */
function apc_bin_load($data, $flags = 0){}

/**
 * @link http://php.net/manual/en/function.apc-bin-loadfile.php
 * @param $filename
 * @param null $context
 * @param int $flags
 */
function apc_bin_loadfile($filename, $context = null, $flags = 0){}

/**
 * Class APCIterator
 * @link http://www.php.net/manual/en/class.apciterator.php
 */
class APCIterator implements Iterator
{
	public function __construct($cache, $search = null, $format = APC_ITER_ALL, $chunk_size = 100, $list = APC_LIST_ACTIVE){}

	public function rewind(){}

	public function valid(){}

	public function current(){}

	public function key(){}

	public function next(){}

	public function getTotalHits(){}

	public function getTotalSize(){}

	public function getTotalCount(){}
}