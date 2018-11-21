<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace MongoDB\Driver;

class Manage
{
    public function __construct($uri, $options, $driverOptions)
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
}
