<?php

if (!class_exists('APCIterator', false)) {

// See: https://github.com/php/pecl-caching-apc/blob/master/apc_bin.h
defined('APC_BIN_VERIFY_CRC32') || define('APC_BIN_VERIFY_CRC32', 1 << 1);
defined('APC_BIN_VERIFY_MD5') || define('APC_BIN_VERIFY_MD5', 1 << 0);

// See: https://github.com/php/pecl-caching-apc/blob/master/apc_iterator.h
defined('APC_ITER_ALL') || define('APC_ITER_ALL', 0xffffffff);
defined('APC_ITER_ATIME') || define('APC_ITER_ATIME', 1 << 11);
defined('APC_ITER_CTIME') || define('APC_ITER_CTIME', 1 << 9);
defined('APC_ITER_DEVICE') || define('APC_ITER_DEVICE', 1 << 3);
defined('APC_ITER_DTIME') || define('APC_ITER_DTIME', 1 << 10);
defined('APC_ITER_FILENAME') || define('APC_ITER_FILENAME', 1 << 2);
defined('APC_ITER_INODE') || define('APC_ITER_INODE', 1 << 4);
defined('APC_ITER_KEY') || define('APC_ITER_KEY', 1 << 1);
defined('APC_ITER_MD5') || define('APC_ITER_MD5', 1 << 6);
defined('APC_ITER_MEM_SIZE') || define('APC_ITER_MEM_SIZE', 1 << 13);
defined('APC_ITER_MTIME') || define('APC_ITER_MTIME', 1 << 8);
defined('APC_ITER_NONE') || define('APC_ITER_NONE', 0x00000000);
defined('APC_ITER_NUM_HITS') || define('APC_ITER_NUM_HITS', 1 << 7);
defined('APC_ITER_REFCOUNT') || define('APC_ITER_REFCOUNT', 1 << 12);
defined('APC_ITER_TTL') || define('APC_ITER_TTL', 1 << 14);
defined('APC_ITER_TYPE') || define('APC_ITER_TYPE', 1 << 0);
defined('APC_ITER_VALUE') || define('APC_ITER_VALUE', 1 << 5);
defined('APC_LIST_ACTIVE') || define('APC_LIST_ACTIVE', 0x1);
defined('APC_LIST_DELETED') || define('APC_LIST_DELETED', 0x2);

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
}

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
 * @return mixed
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
