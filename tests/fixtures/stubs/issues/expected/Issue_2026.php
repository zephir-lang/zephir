<?php

namespace Stubs;

use Stubs\Events\EventManager;

/**
 * Should process return type and generate docblock.
 */
class Issue_2026
{


    /**
     * Expects:
     *  return array|\StdClass[]
     *
     * @return array|\StdClass[]
     */
    public function getClassCollection(): array
    {
    }

    /**
     * Expects:
     *  return array|bool|\StdClass[]
     *
     * @return array|bool|\StdClass[]
     */
    public function getClassCollectionOrBool()
    {
    }

    /**
     * Expects:
     *  return array|bool|\Stubs\Events\EventManager[]
     *
     * @return array|bool|\Stubs\Events\EventManager[]
     */
    public function getInterfaceCollectionOrBool()
    {
    }

    /**
     * Expects:
     *  return array|\Stubs\Events\EventManager[]
     *
     * @return array|\Stubs\Events\EventManager[]
     */
    public function getInterfaceCollectionOrArray(): array
    {
    }

    /**
     * Expects:
     *  return array|\Stubs\Events\EventManager[]|\StdClass[]
     *
     * @return array|\Stubs\Events\EventManager[]|\StdClass[]
     */
    public function getMixedCollectionOrArray(): array
    {
    }
}
