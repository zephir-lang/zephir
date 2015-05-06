<?php

class Redis
{
    /**
     * Options
     */
    const OPT_SERIALIZER        = 1;
    const OPT_PREFIX            = 2;
    const OPT_READ_TIMEOUT = 3;
    const OPT_SCAN = 4;

    /**
     * Serializers
     */
    const SERIALIZER_NONE       = 0;
    const SERIALIZER_PHP        = 1;
    const SERIALIZER_IGBINARY   = 2;

    /**
     * Multi
     */
    const ATOMIC = 0;
    const MULTI = 1;
    const PIPELINE = 2;

    /**
     * Type
     */
    const REDIS_NOT_FOUND       = 0;
    const REDIS_STRING          = 1;
    const REDIS_SET             = 2;
    const REDIS_LIST            = 3;
    const REDIS_ZSET            = 4;
    const REDIS_HASH            = 5;

    /**
     * Scan
     */
    const SCAN_NORETRY = 0;
    const SCAN_RETRY = 1;

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
}
