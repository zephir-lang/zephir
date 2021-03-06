<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

/**
 * Class EventsManager.
 */
class EventsManager
{
    /**
     * @var array
     */
    private $listeners = [];

    /**
     * Attaches a listener to a specific event type.
     *
     * @param $event
     * @param $callback
     */
    public function listen($event, $callback)
    {
        if (!isset($this->listeners[$event])) {
            $this->listeners[$event] = [];
        }

        $this->listeners[$event][] = $callback;
    }

    /**
     * Triggers an event for the specified event type.
     *
     * @param $event
     * @param array $param
     */
    public function dispatch($event, array $param = [])
    {
        foreach ($this->listeners[$event] as $listener) {
            \call_user_func_array($listener, $param);
        }
    }
}
