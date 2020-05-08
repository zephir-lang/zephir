namespace Stubs\Events;

use Stubs\Events\ManagerInterface;

class EventManager implements ManagerInterface
{
    /**
    * Attach a listener to the events manager
    *
    * @param object|callable handler
    */
    public function attach(string! eventType, handler) -> void
    {
        // mock implementation...
    }
}
