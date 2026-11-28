namespace Stub;

/**
 * An array property default is declared on the class, as PHP declares it:
 * every object starts from the class's default, shares it until it writes,
 * and no code runs for the property when the object is created. Only an
 * array that reads a constant is still built for each object.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2799
 */
class Issue2799
{
    const K = "k";

    const EMPTY_LIST = [];

    public items = [];

    protected literal = [1, "a": [2, 3], "e": [], "s": "str", "n": null, "f": 1.5, "b": true];

    public withConst = [PHP_INT_SIZE, self::K];

    public array typed = [];

    public scalar = 5;

    public function getItems()
    {
        return this->items;
    }

    public function getLiteral()
    {
        return this->literal;
    }

    public function getWithConst()
    {
        return this->withConst;
    }

    public function append(var v) -> void
    {
        let this->items[] = v;
    }

    public function setItem(var k, var v) -> void
    {
        let this->items[k] = v;
    }

    public function unsetItem(var k) -> void
    {
        unset this->items[k];
    }

    public function setKey(var k, var v) -> void
    {
        let this->literal[k] = v;
    }

    public function appendNested(var v) -> void
    {
        let this->literal["a"][] = v;
    }

    public function appendNestedEmpty(var v) -> void
    {
        let this->literal["e"][] = v;
    }

    public function unsetKey(var k) -> void
    {
        unset this->literal[k];
    }

    public function incrementF() -> void
    {
        let this->literal["f"] += 1;
    }

    public function shift()
    {
        return array_shift(this->literal);
    }

    public function appendConst(var v) -> void
    {
        let this->withConst[] = v;
    }

    public function appendTyped(var v) -> void
    {
        let this->typed[] = v;
    }
}
