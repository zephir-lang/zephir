<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */
if (!class_exists('MongoRegex', false)) {
    class MongoRegex
    {
    }
}

if (!class_exists('MongoDate', false)) {
    class MongoDate
    {
    }
}

if (!class_exists('MongoId', false)) {
    class MongoId
    {
    }
}

if (!class_exists('MongoDB', false)) {
    class MongoDB
    {
        public const PROFILING_OFF = 0;
        public const PROFILING_SLOW = 1;
        public const PROFILING_ON = 2;

        /**
         * Creates a new database.
         *
         * @see https://www.php.net/manual/en/mongodb.construct.php
         *
         * @param MongoClient $conn
         * @param string      $name
         */
        public function __construct(MongoClient $conn, $name)
        {
        }
    }
}

if (!class_exists('MongoClient', false)) {
    class MongoClient
    {
        public const VERSION = '';
        public const DEFAULT_HOST = 'localhost';
        public const DEFAULT_PORT = 27017;
        public const RP_PRIMARY = 'primary';
        public const RP_PRIMARY_PREFERRED = 'primaryPreferred';
        public const RP_SECONDARY = 'secondary';
        public const RP_SECONDARY_PREFERRED = 'secondaryPreferred';
        public const RP_NEAREST = 'nearest';

        /**
         * Gets a database.
         *
         * @see https://www.php.net/manual/en/mongoclient.selectdb.php
         *
         * @param string $name
         *
         * @return MongoDB
         */
        public function selectDB($name)
        {
        }
    }
}
