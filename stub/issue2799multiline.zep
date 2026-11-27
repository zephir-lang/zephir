namespace Stub;

/**
 * A string that spans lines in a declaration keeps its line breaks, as a
 * string in a method body does. Array defaults are declared on the class
 * since #2799, which made the template of a class like this reach C.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2799
 */
class Issue2799Multiline
{
    const TEXT = "c1
c2";

    const LINES = ["l1
l2", "k1
k2": "v1
v2"];

    public template = ["main": "
<nav>
    <ol>
%items%
    </ol>
</nav>", "k1
k2": 1];

    public text = "t1
t2";

    public string typedText = "y1
y2";

    public static shared = "s1
s2";

    public function withDefault(string x = "p1
p2") -> string
    {
        return x;
    }
}
