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
     *  return array|\stdClass[]
     *
     * @return array|\stdClass[]
     */
    public function getClassCollection(): array
    {
    }

    /**
     * Expects:
     *  return array|bool|\stdClass[]
     *
     * @return array|bool|\stdClass[]
     */
    public function getClassCollectionOrBool(): bool|array
    {
    }

    /**
     * Expects:
     *  return array|bool|\Stubs\Events\EventManager[]
     *
     * @return array|bool|\Stubs\Events\EventManager[]
     */
    public function getInterfaceCollectionOrBool(): bool|array
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
     *  return array|\Stubs\Events\EventManager[]|\stdClass[]
     *
     * @return array|\Stubs\Events\EventManager[]|\stdClass[]
     */
    public function getMixedCollectionOrArray(): array
    {
    }
}
