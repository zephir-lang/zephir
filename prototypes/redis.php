<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */
class redis
{
    /**
     * Options.
     */
    public const OPT_SERIALIZER = 1;
    public const OPT_PREFIX = 2;
    public const OPT_READ_TIMEOUT = 3;
    public const OPT_SCAN = 4;

    /**
     * Serializers.
     */
    public const SERIALIZER_NONE = 0;
    public const SERIALIZER_PHP = 1;
    public const SERIALIZER_IGBINARY = 2;

    /**
     * Multi.
     */
    public const ATOMIC = 0;
    public const MULTI = 1;
    public const PIPELINE = 2;

    /**
     * Type.
     */
    public const REDIS_NOT_FOUND = 0;
    public const REDIS_STRING = 1;
    public const REDIS_SET = 2;
    public const REDIS_LIST = 3;
    public const REDIS_ZSET = 4;
    public const REDIS_HASH = 5;

    /**
     * Scan.
     */
    public const SCAN_NORETRY = 0;
    public const SCAN_RETRY = 1;

    public function connect($host, $port = 6379, $timeout = 0.0)
    {
    }

    public function pconnect($host, $port = 6379, $timeout = 0.0)
    {
    }

    public function auth($auth)
    {
    }

    public function select($db)
    {
    }

    public function get($key)
    {
    }

    public function flushDB($async = false)
    {
    }

    public function set($key, $value, $timeout = 0)
    {
    }

    public function del($key, ...$other_keys)
    {
    }

    /**
     * Legacy alias of del(); deprecated in phpredis 5 and removed in phpredis 6.
     */
    public function delete($key1, $key2 = null, $key3 = null)
    {
    }

    /**
     * @see https://github.com/phpredis/phpredis#setoption
     *
     * @param mixed $name
     * @param mixed $value
     *
     * @return bool
     */
    public function setOption($name, $value)
    {
    }

    /**
     * @see https://github.com/phpredis/phpredis#getoption
     *
     * @param mixed $name
     *
     * @return mixed
     */
    public function getOption($name)
    {
    }
}

/**
 * RedisCluster ships with the same `redis` extension, so its prototype must
 * live in this file: the prototype loader maps the file basename to an
 * extension name and skips the file when that extension is loaded.
 *
 * @see https://github.com/phpredis/phpredis/blob/develop/cluster.md
 */
class RedisCluster
{
    /**
     * Options.
     */
    public const OPT_SERIALIZER = 1;
    public const OPT_PREFIX = 2;
    public const OPT_READ_TIMEOUT = 3;
    public const OPT_SCAN = 4;

    /**
     * Slave failover.
     */
    public const OPT_SLAVE_FAILOVER = 5;
    public const FAILOVER_NONE = 0;
    public const FAILOVER_ERROR = 1;
    public const FAILOVER_DISTRIBUTE = 2;
    public const FAILOVER_DISTRIBUTE_SLAVES = 3;

    /**
     * Serializers.
     */
    public const SERIALIZER_NONE = 0;
    public const SERIALIZER_PHP = 1;
    public const SERIALIZER_IGBINARY = 2;

    /**
     * Multi. RedisCluster does not support pipelining, so there is no PIPELINE.
     */
    public const ATOMIC = 0;
    public const MULTI = 1;

    /**
     * Type.
     */
    public const REDIS_NOT_FOUND = 0;
    public const REDIS_STRING = 1;
    public const REDIS_SET = 2;
    public const REDIS_LIST = 3;
    public const REDIS_ZSET = 4;
    public const REDIS_HASH = 5;

    /**
     * Scan.
     */
    public const SCAN_NORETRY = 0;
    public const SCAN_RETRY = 1;

    public function __construct($name, $seeds = null, $timeout = 0, $read_timeout = 0, $persistent = false, $auth = null, $context = null)
    {
    }

    public function get($key)
    {
    }

    public function set($key, $value, $options = null)
    {
    }

    public function del($key, ...$other_keys)
    {
    }

    public function exists($key, ...$other_keys)
    {
    }

    public function mget($keys)
    {
    }

    public function mset($key_values)
    {
    }

    /**
     * @see https://github.com/phpredis/phpredis#setoption
     *
     * @param mixed $option
     * @param mixed $value
     *
     * @return bool
     */
    public function setOption($option, $value)
    {
    }

    /**
     * @see https://github.com/phpredis/phpredis#getoption
     *
     * @param mixed $option
     *
     * @return mixed
     */
    public function getOption($option)
    {
    }

    public function close()
    {
    }

    /**
     * Directed node command: $key_or_address selects the node to flush.
     */
    public function flushDB($key_or_address, $async = false)
    {
    }

    public function _masters()
    {
    }
}
