/**
 * Declaring a local with an expression default value must compile.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2394
 */

namespace Stub;

class Issue2394
{
    public function helper()
    {
        return 7;
    }

    public function addition(int a)
    {
        var x = a + 1;

        return x;
    }

    public function subtraction(int a)
    {
        var x = a - 1;

        return x;
    }

    public function multiplication(int a)
    {
        var x = a * 2;

        return x;
    }

    public function division(int a)
    {
        var x = a / 2;

        return x;
    }

    public function modulo(int a)
    {
        var x = a % 3;

        return x;
    }

    public function concatenation(string a, string b)
    {
        var s = a . b;

        return s;
    }

    public function ternary(int n)
    {
        var t = (n < 2) ? 10 : 20;

        return t;
    }

    public function comparison(int n)
    {
        var b = n > 2;

        return b;
    }

    public function methodCall()
    {
        var r = this->helper();

        return r;
    }

    /**
     * Reproduces the exact shape from the issue: an expression-default
     * declaration nested inside a loop with a preceding `continue`.
     */
    public function backslashLoop(string format) -> string
    {
        var out, i, sl, sub, kos;

        let out = "";
        let sl = strlen(format);
        let i = 0;
        while (i < sl) {
            let sub = substr(format, i, 1);
            if (sub == "\\") {
                var kos = i + 1;
                let out .= substr(format, kos, 1);
                let i = i + 2;
                continue;
            }
            let out .= sub;
            let i = i + 1;
        }

        return out;
    }
}
