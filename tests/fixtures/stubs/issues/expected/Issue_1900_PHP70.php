<?php

namespace Stubs;

use Psr\Http\Message\RequestInterface;
use Stubs\Events\ManagerInterface as AliasedManagerInterface;

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
class Issue_1900 implements AliasedManagerInterface
{

    public $eventsManager;

    /**
     * @var array $collect - descr for collect var.
     */
    private $collect;

    static protected $uniqueId = 0;

    /**
     * @return AliasedManagerInterface
     */
    public function getEventsManager(): AliasedManagerInterface
    {
    }

    /**
     * @return \StdClass
     */
    public function test01(): \StdClass
    {
    }

    /**
     * Attach a listener to the events manager
     *
     * @param object|callable $handler
     * @param string $eventType
     * @param int $priority
     * @return void
     */
    public function attach(string $eventType, $handler, int $priority = 1)
    {
    }

    /**
     * Returns if priorities are enabled
     *
     * @return bool
     */
    public function arePrioritiesEnabled(): bool
    {
    }

    /**
     * Tells the event manager if it needs to collect all the responses returned
     * by every registered listener in a single fire
     *
     * @param bool $collect
     * @param string $eventType
     * @param mixed $handler
     * @return void
     */
    public function collectResponses(bool $collect, string $eventType, $handler)
    {
    }

    /**
     * Fires an event in the events manager causing the active listeners to be
     * notified about it
     *
     * ```php
     * $eventsManager->fire("db", $connection);
     * ```
     *
     * @param object $source
     * @param mixed $data
     * @return mixed
     * @param string $eventType
     * @param bool $cancelable
     */
    public function fire(string $eventType, $source, $data = null, bool $cancelable = true)
    {
    }

    /**
     * Internal handler to call a queue of events
     *
     * @return mixed
     * @param SplPriorityQueue $queue
     * @param \Psr\Http\Message\RequestInterface $event
     */
    final public function fireQueue(SplPriorityQueue $queue, \Psr\Http\Message\RequestInterface $event): AliasedManagerInterface
    {
    }

    /**
     * Returns all the attached listeners of a certain type
     *
     * @param string $type
     * @return array
     */
    public function getListeners(string $type): array
    {
    }

    /**
     * @return resource
     */
    public function getResources()
    {
    }

    /**
     * Check whether certain type of event has listeners
     *
     * @param string $type
     * @return AliasedManagerInterface
     */
    public function hasListeners(string $type): AliasedManagerInterface
    {
    }
}
