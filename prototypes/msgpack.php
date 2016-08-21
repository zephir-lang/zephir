<?php

define('MSGPACK_CLASS_OPT_PHPONLY', -1001);

/**
 * \MessagePackUnpacker
 *
 * @link https://github.com/msgpack/msgpack-php
 */
class MessagePackUnpacker
{
    /**
     * MessagePackUnpacker constructor.
     *
     * @param boolean $opt
     */
    public function __construct($opt = null)
    {
    }

    /**
     * MessagePack destructor.
     */
    public function __destruct()
    {
    }

    /**
     * @param integer $option
     * @param boolean $value
     */
    public function setOption($option, $value)
    {
    }

    /**
     * @param string $str
     */
    public function feed($str)
    {
    }

    /**
     * @param string $str
     * @param integer $offset
     */
    public function execute($str = null, &$offset = null)
    {
    }

    /**
     * @param mixed $object
     */
    public function data($object = null)
    {
    }

    public function reset()
    {
    }
}

/**
 * \MessagePack
 *
 * @link https://github.com/msgpack/msgpack-php
 */
class MessagePack
{
    /**
     * MessagePack constructor.
     *
     * @param boolean $opt
     */
    public function __construct($opt = null)
    {
    }

    /**
     * @param integer $option
     * @param boolean $value
     */
    public function setOption($option, $value)
    {
    }

    /**
     * @param mixed $value
     * @return string
     */
    public function pack($value)
    {
    }

    /**
     * @param string $str
     * @param mixed $object
     * @return mixed
     */
    public function unpack($str, $object = null)
    {
    }

    /**
     * @return MessagePackUnpacker
     */
    public function unpacker()
    {
    }
}

/**
 * @param mixed $value
 * @return string
 */
function msgpack_pack($value) {

}

/**
 * @param string $str
 * @return mixed
 */
function msgpack_unpack($str) {

}


/**
 * @param mixed $value
 * @return string
 */
function msgpack_serialize($value) {

}

/**
 * @param string $str
 * @return mixed
 */
function msgpack_unserialize($str) {

}
