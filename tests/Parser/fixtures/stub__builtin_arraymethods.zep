
namespace Stub\BuiltIn;

class ArrayMethods
{
	public function getJoin1()
	{
		return [1, 2, 3]->join("-");
	}

	public function getReversed1()
	{
		return [1, 2, 3]->reversed();
	}

	public function getMap1()
	{
		return [1, 2, 3]->map(x => x * 100);
	}

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/733
     */
	public function issue733BuiltInJoinSpecialChars(string! a, var b = null) -> string
	{
	    if b === null || b === false {
            return a->trimleft();
        }

        if typeof b == "array" {
            let b = b->join("");
        }

        let b = preg_replace("#[-\[\]:\\\\^/]#", "\\\\$0", b);

        return preg_replace("/^[" . b . "]+/u", "", a);
	}

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/733
     * Exercises the bare-minimum case: a `var` holding an array, joined directly.
     */
    public function issue733JoinOnVar(var input) -> string
    {
        return input->join(",");
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/733
     * `reversed()` is also in the whitelist — verifies the fix isn't `join`-only.
     */
    public function issue733ReversedOnVar(var input) -> array
    {
        return input->reversed();
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/733
     * Ensures the standard happy path (typed array literal) still works after
     * the dispatch change.
     */
    public function issue733JoinOnLiteralArray() -> string
    {
        return ["a", "b", "c"]->join("|");
    }
}
