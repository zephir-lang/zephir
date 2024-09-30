namespace Stubs;

use Stubs\Events\EventManager;

/**
 * Should process return type and generate docblock.
 */
class Issue_2026
{
    /**
     * Expects:
     *  return array|\stdClass[]
     */
    public function getClassCollection() -> <\stdClass[]>
    {
        return [
            new \StdClass()
        ];
    }

    /**
     * Expects:
     *  return array|bool|\stdClass[]
     */
    public function getClassCollectionOrBool() -> <\stdClass[]> | bool
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
     *  return array|\Stubs\Events\EventManager[]|\stdClass[]
     */
    public function getMixedCollectionOrArray() -> <EventManager[]> | <\stdClass[]> | array
    {
        return [
            new EventManager()
        ];
    }
}
