namespace Stubs;

use Stubs\Events\ManagerInterface;

/**
 * Should process return type and generate docblock.
 */
class Issue_2092
{
    /**
     * Returns the internal event manager
     * Expects:
     *  return \Stubs\Events\ManagerInterface|null
     */
    public function getInternalEventsManager() -> <ManagerInterface> | null
    {
        return null;
    }
}
