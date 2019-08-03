<?php

// @codingStandardsIgnoreStart

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace MongoDB\Driver {
    if (!class_exists('MongoDB\Driver\Manager', false)) {
        class mongodb
        {
            public function __construct($uri = 'mongodb://127.0.0.1/', array $options = [], array $driverOptions = [])
            {
            }

            public function executeQuery($namespace, $query, $readPreference)
            {
            }

            public function executeBulkWrite($namespace, $bulk, $writeConcern)
            {
            }

            public function executeCommand($db, $command, $readPreference)
            {
            }

            public function getServers()
            {
            }

            public function selectServer($readPreference)
            {
            }

            public function executeReadCommand($db, $command, array $options = [])
            {
            }

            public function executeReadWriteCommand($db, $command, array $options = [])
            {
            }

            public function executeWriteCommand($db, $command, array $options = [])
            {
            }

            public function getReadConcern()
            {
            }

            public function getReadPreference()
            {
            }

            public function getWriteConcern()
            {
            }

            public function startSession(array $options = [])
            {
            }
        }
    }
}
