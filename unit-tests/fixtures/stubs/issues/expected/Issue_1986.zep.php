<?php

namespace Stubs;

use Stubs\Events\ManagerInterface as EventsManagerInterface;

class Issue_1986 implements EventsManagerInterface
{

    public $eventsManager;


    /**
     * @return EventsManagerInterface
     */
    public function getEventsManager(): EventsManagerInterface
    {
    }

    /**
     * Attach a listener to the events manager
     *
     * @param object|callable $handler
     * @param string $eventType
     */
    public function attach(string $eventType, $handler)
    {
    }

}
