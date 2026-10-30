namespace Stub\Issue2761;

/**
 * Property shortcuts take their accessor types from the declared property
 * type, falling back to a `@var` docblock that may name a class.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2761
 */
class Accessors
{
    protected string name = "" { get, set };

    protected <Collaborator> thing { get, set };

    protected ?<Collaborator> maybe = null { get, set };

    protected ?int count = null { get, set };

    protected int pending { get };

    /** @var \Stub\Issue2761\Collaborator */
    protected docQualified { get, set };

    /** @var Collaborator */
    protected docRelative { get };

    /** @var Collaborator|null */
    protected docNullable { get, set };

    /** @var Collaborator */
    protected int declaredWins = 0 { get, set };
}
