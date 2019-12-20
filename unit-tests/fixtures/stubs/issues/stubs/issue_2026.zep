namespace Stubs;

use Stubs\Events\EventManager;

class Issue_2026
{
    /**
     * Should process return type and generate docblock.
     * Expects:
     *  return array|\StdClass[]
     */
    public function getClassCollection() -> <\StdClass[]>
    {
        return [
            new \StdClass()
        ];
    }

    /**
     * Should process return type and generate docblock.
     * Expects:
     *  return array|bool|\StdClass[]
     */
    public function getClassCollectionOrBool() -> <\StdClass[]> | bool
    {
        return [
            new \StdClass()
        ];
    }

    /**
     * Should process return type and generate docblock.
     * Expects:
     *  return array|bool|\Stubs\Events\EventManager[]
     */
    public function getInterfaceCollectionOrBool() -> <EventManager[]> | bool
    {
        return [
            new EventManager()
        ];
    }
}
