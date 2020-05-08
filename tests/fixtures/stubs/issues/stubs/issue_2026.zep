namespace Stubs;

use Stubs\Events\EventManager;

/**
 * Should process return type and generate docblock.
 */
class Issue_2026
{
    /**
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
     * Expects:
     *  return array|bool|\Stubs\Events\EventManager[]
     */
    public function getInterfaceCollectionOrBool() -> <EventManager[]> | bool
    {
        return [
            new EventManager()
        ];
    }

    /**
     * Expects:
     *  return array|\Stubs\Events\EventManager[]
     */
    public function getInterfaceCollectionOrArray() -> <EventManager[]> | array
    {
        return [
            new EventManager()
        ];
    }

    /**
     * Expects:
     *  return array|\Stubs\Events\EventManager[]|\StdClass[]
     */
    public function getMixedCollectionOrArray() -> <EventManager[]> | <\StdClass[]> | array
    {
        return [
            new EventManager()
        ];
    }
}
