<?php

namespace Stubs;

use Stubs\Events\EventManager;

class Issue_2026
{

    /**
     * Should process return type and generate docblock.
     * Expects:
     *  return array|\StdClass[]
     *
     * @return array|\StdClass[]
     */
    public function getClassCollection(): array
    {
    }

    /**
     * Should process return type and generate docblock.
     * Expects:
     *  return array|bool|\StdClass[]
     *
     * @return array|bool|\StdClass[]
     */
    public function getClassCollectionOrBool()
    {
    }

    /**
     * Should process return type and generate docblock.
     * Expects:
     *  return array|bool|\Stubs\Events\EventManager[]
     *
     * @return array|bool|\Stubs\Events\EventManager[]
     */
    public function getInterfaceCollectionOrBool()
    {
    }

}
