<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir;

/**
 * Class EventsManager
 *
 * @package Zephir
 */
class EventsManager
{
    /**
     * @var array
     */
    private $listeners = array();

    /**
     * Attaches a listener to a specific event type
     *
     * @param $event
     * @param $callback
     */
    public function listen($event, $callback)
    {
        if (!isset($this->listeners[$event])) {
            $this->listeners[$event] = array();
        }

        $this->listeners[$event][] = $callback;
    }

    /**
     * Triggers an event for the specified event type
     *
     * @param $event
     * @param array $param
     */
    public function dispatch($event, array $param = array())
    {
        foreach ($this->listeners[$event] as $listener) {
            call_user_func_array($listener, $param);
        }
    }
}
