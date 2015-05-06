<?php

/**
 * Class Memcached
 * @link http://ru2.php.net/manual/en/book.memcached.php
 */
class Memcached
{
    const OPT_COMPRESSION = -1001;
    const OPT_COMPRESSION_TYPE = -1004;
    const OPT_PREFIX_KEY = -1002;
    const OPT_SERIALIZER = -1003;
    const OPT_HASH = 2;
    const OPT_DISTRIBUTION = 9;
    const OPT_LIBKETAMA_COMPATIBLE = 16;
    const OPT_LIBKETAMA_HASH = 17;
    const OPT_TCP_KEEPALIVE = 32;
    const OPT_BUFFER_WRITES = 10;
    const OPT_BINARY_PROTOCOL = 18;
    const OPT_NO_BLOCK = 0;
    const OPT_TCP_NODELAY = 1;
    const OPT_SOCKET_SEND_SIZE = 4;
    const OPT_SOCKET_RECV_SIZE = 5;
    const OPT_CONNECT_TIMEOUT = 14;
    const OPT_RETRY_TIMEOUT = 15;
    const OPT_SEND_TIMEOUT = 19;
    const OPT_RECV_TIMEOUT = 20;
    const OPT_POLL_TIMEOUT = 8;
    const OPT_CACHE_LOOKUPS = 6;

    const HAVE_IGBINARY = 0;
    const HAVE_JSON = 0;
    const HAVE_SESSION = 1;
    const HAVE_SASL = 0;

    const HASH_DEFAULT = 0;
    const HASH_MD5 = 1;
    const HASH_CRC = 2;
    const HASH_FNV1_64 = 3;
    const HASH_FNV1A_64 = 4;
    const HASH_FNV1_32 = 5;
    const HASH_FNV1A_32 = 6;
    const HASH_HSIEH = 7;
    const HASH_MURMUR = 8;


    const DISTRIBUTION_MODULA = 0;
    const DISTRIBUTION_CONSISTENT = 1;

    const OPT_SERVER_FAILURE_LIMIT = 21;
    const OPT_AUTO_EJECT_HOSTS = 28;
    const OPT_HASH_WITH_PREFIX_KEY = 25;
    const OPT_NOREPLY = 26;
    const OPT_SORT_HOSTS = 12;
    const OPT_VERIFY_KEY = 13;
    const OPT_USE_UDP = 27;
    const OPT_NUMBER_OF_REPLICAS = 29;
    const OPT_RANDOMIZE_REPLICA_READ = 30;
    const OPT_REMOVE_FAILED_SERVERS = 35;

    const RES_SUCCESS = 0;
    const RES_FAILURE = 1;
    const RES_HOST_LOOKUP_FAILURE = 2;
    const RES_UNKNOWN_READ_FAILURE = 7;
    const RES_PROTOCOL_ERROR = 8;

    const RES_CLIENT_ERROR = 9;
    const RES_SERVER_ERROR = 10;
    const RES_WRITE_FAILURE = 5;

    const RES_DATA_EXISTS = 12;
    const RES_NOTSTORED = 14;
    const RES_NOTFOUND = 16;
    const RES_PARTIAL_READ = 18;
    const RES_SOME_ERRORS = 19;
    const RES_NO_SERVERS = 20;
    const RES_END = 21;
    const RES_ERRNO = 26;
    const RES_BUFFERED = 32;
    const RES_TIMEOUT = 31;
    const RES_BAD_KEY_PROVIDED = 33;
    const RES_STORED = 15;
    const RES_DELETED = 22;
    const RES_STAT = 24;
    const RES_ITEM = 25;
    const RES_NOT_SUPPORTED = 28;
    const RES_FETCH_NOTFINISHED = 30;
    const RES_SERVER_MARKED_DEAD = 35;
    const RES_UNKNOWN_STAT_KEY = 36;
    const RES_INVALID_HOST_PROTOCOL = 34;
    const RES_MEMORY_ALLOCATION_FAILURE = 17;
    const RES_CONNECTION_SOCKET_CREATE_FAILURE = 11;
    const RES_PAYLOAD_FAILURE = -1001;

    const SERIALIZER_PHP = 1;
    const SERIALIZER_IGBINARY = 2;
    const SERIALIZER_JSON = 3;
    const SERIALIZER_JSON_ARRAY = 4;

    const COMPRESSION_FASTLZ = 2;
    const COMPRESSION_ZLIB = 1;

    const GET_PRESERVE_ORDER = 1;
    const GET_ERROR_RETURN_VALUE = false;

    /**
     * http://ru2.php.net/manual/en/memcached.construct.php
     * @param string $persistent_id
     */
    public function __construct($persistent_id)
    {

    }

    /**
     * @link http://php.net/manual/en/memcached.add.php
     * @param $key
     * @param $value
     * @param int $expiration
     */
    public function add($key, $value, $expiration = 0)
    {

    }

    /**
     * @link http://php.net/manual/en//memcached.delete.php
     * @param $key
     * @param int $time
     */
    public function delete($key, $time = 0)
    {

    }

    /**
     * Retrieve an item
     * @link http://php.net/manual/en/memcached.get.php
     * @param string $key
     * @param callable $cache_cb
     * @param float $cas_token
     */
    public function get ($key, callable $cache_cb = null, &$cas_token = null)
    {

    }

    /**
     * @link http://php.net/manual/en/memcached.flush.php
     * @param int $delay
     * @return bool
     */
    public function flush ($delay = 0)
    {

    }

    /**
     * @link http://ru2.php.net/manual/en/memcached.set.php
     * @param $key
     * @param $value
     * @param int $expiration
     */
    public function set($key, $value, $expiration = 0)
    {

    }

    /**
     * @link http://ru2.php.net/manual/en/memcached.addbykey.php
     * @param $server_key
     * @param $key
     * @param $value
     * @param int $expiration
     */
    public function addByKey($server_key, $key, $value, $expiration = 0)
    {

    }

    /**
     * @link http://ru2.php.net/manual/en/memcached.addserver.php
     * @param $host
     * @param $port
     * @param int $weight
     */
    public function addServer($host, $port, $weight = 0)
    {

    }

    /**
     * @link http://ru2.php.net/manual/en/memcached.addservers.php
     * @param array $servers
     */
    public function addServers($servers)
    {

    }

    /**
     * @link http://ru2.php.net/manual/en/memcached.setoption.php
     * @param int $option
     * @param mixed $value
     */
    public function setOption($option, $value)
    {

    }

    /**
     * @link http://ru2.php.net/manual/en/memcached.setoption.php
     * @param array $options
     */
    public function setOptions($options)
    {

    }

    /**
     * @link http://ru2.php.net/manual/en/memcached.setsaslauthdata.php
     * @param string $username
     * @param string $password
     */
    public function setSaslAuthData($username, $password)
    {

    }

    /**
     * @link http://ru2.php.net/manual/en/memcached.touch.php
     * @param string $key
     * @param int $expiration
     */
    public function touch($key, $expiration)
    {

    }

    /**
     * @link http://ru2.php.net/manual/en/memcached.touchbykey.php
     * @param $server_key
     * @param $key
     * @param $expiration
     */
    public function touchByKey($server_key, $key, $expiration)
    {

    }
}

class MemcachedException extends RuntimeException {
}
