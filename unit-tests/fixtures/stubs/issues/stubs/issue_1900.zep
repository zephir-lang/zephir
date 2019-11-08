/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Stubs;

use Psr\Http\Message\RequestInterface;
use Stubs\Events\ManagerInterface as AliasedManagerInterface;

class Issue_1900 implements AliasedManagerInterface
{
    public eventsManager;

    /** @var array $collect - descr for collect var. */
    private collect;

    protected static uniqueId = 0;

    /**
     * @return AliasedManagerInterface
     */
    public function getEventsManager() -> <AliasedManagerInterface>
    {
    }

    public function test01() -> <\StdClass>
    {
        return new \StdClass();
    }

    /**
     * Attach a listener to the events manager
     *
     * @param object|callable handler
     */
    public function attach(string! eventType, var handler, int! priority = 1) -> void
    {
        var priorityQueue;

        // Insert the handler in the queue
        priorityQueue->insert(handler, priority);
    }

    /**
    * Returns if priorities are enabled
    */
    public function arePrioritiesEnabled() -> bool
    {
        return true;
    }

    /**
    * Tells the event manager if it needs to collect all the responses returned
    * by every registered listener in a single fire
    */
    public function collectResponses(bool collect, string! eventType, var handler) -> void
    {
        let this->collect = collect;
    }

    /**
    * Fires an event in the events manager causing the active listeners to be
    * notified about it
    *
    *```php
    * $eventsManager->fire("db", $connection);
    *```
    *
    * @param object source
    * @param mixed  data
    * @return mixed
    */
    public function fire(string! eventType, object source, var data = null, bool cancelable = true)
    {
        return 1;
    }

    /**
    * Internal handler to call a queue of events
    *
    * @return mixed
    */
    final public function fireQueue(<SplPriorityQueue> queue, <RequestInterface> event) -> <AliasedManagerInterface>
    {
        return event;
    }

    /**
    * Returns all the attached listeners of a certain type
    */
    public function getListeners(string! type) -> array
    {
        return [];
    }

    /**
    * Returns all the responses returned by every handler executed by the last
    * 'fire' executed
    */
    public function getResponses() -> array | null
    {
        return [];
    }

    public function getResources() -> Resource
    {
        return [];
    }

    /**
    * Check whether certain type of event has listeners
    */
    public function hasListeners(string! type) -> <AliasedManagerInterface>
    {
        return [];
    }
}
