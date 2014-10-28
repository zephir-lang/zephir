<?php

namespace Phalcon {
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
}

namespace Phalcon\Mvc {
    class Application
    {

    }
}

namespace Phalcon\Mvc\Model {
    class Query
    {

        const TYPE_SELECT = 309;

        const TYPE_INSERT = 306;

        const TYPE_UPDATE = 300;

        const TYPE_DELETE = 303;
    }
}