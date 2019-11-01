namespace Stubs\Events;

/**
 * Test Events Manager, offers an easy way to intercept and manipulate, if
 * needed, the normal flow of operation. With the EventsManager the developer
 * can create hooks or plugins that will offer monitoring of data, manipulation,
 * conditional execution and much more.
 */
interface ManagerInterface
{
    /**
    * Attach a listener to the events manager
    *
    * @param object|callable handler
    */
    public function attach(string! eventType, handler) -> void;
}
