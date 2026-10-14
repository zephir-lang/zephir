namespace Stub\Issue2391;

/**
 * A getter that returns `this->prop` verifies the value against the return
 * type it advertises, as PHP does for a hand-written method.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2391
 */
class Accessors
{
    /** @var bool */
    protected flag { get };

    /** @var \Stub\Issue2391\Collaborator */
    protected thing { get };

    /** @var object */
    protected obj { get };

    /** @var Collaborator|null */
    protected maybe { get };

    /** @var int|string */
    protected key { get };

    /** @var array|false */
    protected listOrFalse { get };

    /** @var double */
    protected ratio { get };

    /** @var string */
    protected name { get };

    protected label { toString };

    /**
     * Stores any value, so a test can put a wrong-typed one behind a getter.
     */
    public function poke(string property, var value) -> <Accessors>
    {
        let this->{property} = value;

        return this;
    }

    public function flagByHand() -> bool
    {
        return this->flag;
    }

    public function thingByHand() -> <Collaborator>
    {
        return this->thing;
    }

    public function maybeByHand() -> <Collaborator> | null
    {
        return this->maybe;
    }

    public function objByHand() -> object
    {
        return this->obj;
    }
}
