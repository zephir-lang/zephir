namespace Stubs;

/**
 * Readonly properties (#2614) must carry the `readonly` modifier into the
 * generated stub, after the visibility keyword and before the type hint. A
 * readonly property is typed, has no default and is initialized once in the
 * constructor.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2614
 */
class Issue_2614
{
    public readonly int id;

    protected readonly string label;

    public readonly array tags;

    public readonly <\ArrayIterator> friend;

    public function __construct()
    {
        let this->id = 1;
        let this->label = "x";
        let this->tags = [];
        let this->friend = new \ArrayIterator();
    }
}
