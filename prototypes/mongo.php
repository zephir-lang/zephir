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
        const PROFILING_OFF = 0;
        const PROFILING_SLOW = 1;
        const PROFILING_ON = 2;

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
        const VERSION = '';
        const DEFAULT_HOST = 'localhost';
        const DEFAULT_PORT = 27017;
        const RP_PRIMARY = 'primary';
        const RP_PRIMARY_PREFERRED = 'primaryPreferred';
        const RP_SECONDARY = 'secondary';
        const RP_SECONDARY_PREFERRED = 'secondaryPreferred';
        const RP_NEAREST = 'nearest';

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
