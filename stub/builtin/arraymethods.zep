
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
}
