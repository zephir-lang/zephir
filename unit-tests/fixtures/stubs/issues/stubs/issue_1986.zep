namespace Stubs;

use Stubs\Events\ManagerInterface as EventsManagerInterface;

class Issue_1986 implements EventsManagerInterface
{
    public eventsManager;

    public function getEventsManager() -> <EventsManagerInterface>
    {
        return this->eventsManager;
    }

    /**
    * Attach a listener to the events manager
    *
    * @param object|callable handler
    */
    public function attach(string! eventType, handler) -> void
    {
    }
}
