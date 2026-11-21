namespace Stub;

function issue2824_pair(var a, var b) -> string
{
    return a . "," . b;
}

/**
 * Call sites passing named arguments.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
class Issue2824
{
    public function typed(<Issue2824Pair> p) -> string
    {
        return p->pair(b: "2", a: "1");
    }

    public function untyped(var p) -> string
    {
        return p->pair(b: "2", a: "1");
    }

    public function positionalThenNamed(var p) -> string
    {
        return p->pair("1", b: "2");
    }

    public function typedMixed(<Issue2824Pair> p) -> string
    {
        return p->pair("1", b: "2");
    }

    public function skipped(<Issue2824Pair> p) -> string
    {
        return p->withDefault(b: "2");
    }

    public function skippedUntyped(var p) -> string
    {
        return p->withDefault(b: "2");
    }

    public function skippedCallable(<Issue2824Pair> p) -> string
    {
        return p->withCallable(b: "2");
    }

    public function skippedObject(<Issue2824Pair> p) -> string
    {
        return p->withObject(b: "2");
    }

    public function skippedInt(<Issue2824Pair> p) -> string
    {
        return p->withInt(b: 7);
    }

    public function variadicExtras(<Issue2824Pair> p) -> array
    {
        return p->variadic(1, 2, x: 3);
    }

    public function unknownName(var p) -> string
    {
        return p->pair(a: "1", zz: "2");
    }

    public function overwrite(var p) -> string
    {
        return p->pair("1", a: "2");
    }

    public function statement(<Issue2824Pair> p) -> void
    {
        p->pair(b: "2", a: "1");
    }

    public function classStatic() -> string
    {
        return Issue2824Pair::spair(b: "2", a: "1");
    }

    public function dynamicClass(string className) -> string
    {
        return {className}::spair(b: "2", a: "1");
    }

    public function dynamicMethod(var p, string method) -> string
    {
        return p->{method}(b: "2", a: "1");
    }

    public function userFunction() -> string
    {
        return issue2824_pair(b: "2", a: "1");
    }

    public function numberFormat() -> string
    {
        return number_format(thousands_separator: " ", num: 1234.567, decimals: 2);
    }

    public function strposNamed() -> var
    {
        return strpos(needle: "b", haystack: "abc");
    }

    public function inArrayNamed() -> bool
    {
        return in_array(haystack: ["1", "b"], strict: true, needle: 1);
    }

    public function pregMatchNamed() -> array
    {
        var matches;

        preg_match(subject: "abc", matches: matches, pattern: "/(b)/");

        return matches;
    }

    public function positionalByReference() -> array
    {
        var result;

        parse_str("a=1", result);

        return result;
    }

    public function namedArgumentReleased(<Issue2824Pair> p) -> array
    {
        var probe, weak, released;

        let released = [];

        let probe = new \stdClass();
        let weak = \WeakReference::create(probe);
        gettype(value: probe);
        let probe = null;
        let released[] = weak->get() === null;

        let probe = new \stdClass();
        let weak = \WeakReference::create(probe);
        p->withObject(o: probe);
        let probe = null;
        let released[] = weak->get() === null;

        return released;
    }

    public function byReferenceLeftUnassigned() -> string
    {
        var word;

        sscanf("", "%s", word);
        sscanf("abc", "%s", word);

        return word;
    }


    public function construct() -> array
    {
        var instance;

        let instance = new Issue2824Ctor(b: "2", a: "1");

        return instance->getArrayCopy();
    }

    public function closure() -> string
    {
        var f;

        let f = function (a, b) {
            return a . "," . b;
        };

        return {f}(b: "2", a: "1");
    }

    public function callUserFunc(var p) -> string
    {
        return call_user_func([p, "pair"], b: "2", a: "1");
    }
}
