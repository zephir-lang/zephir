<?php

namespace Phalcon;

class DI
{
    static protected $di;

    public static function setDi($di)
    {
        self::$di = $di;
    }

    public static function getDefault()
    {
        return self::$di;
    }
}