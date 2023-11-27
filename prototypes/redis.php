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

    public function flushDB()
    {
    }

    public function set($key, $value, $timeout = 0)
    {
    }

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
