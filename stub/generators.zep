/**
 * Generator methods (`yield`) — issue #1849.
 * Exercised by tests/Extension/GeneratorTest.php.
 */

namespace Stub;

class Generators
{
    public sideEffects = 0;
    public items = [];

    public function countTo(int n)
    {
        int i = 1;
        let this->sideEffects = this->sideEffects + 1;
        while i <= n {
            yield i;
            let i = i + 1;
        }
    }

    public function keysMix()
    {
        yield "a", 1;
        yield 10, 2;
        yield 3;
        yield "z", 4;
        yield 5;
    }

    public function bare()
    {
        yield;
        yield;
    }

    public function withReturn()
    {
        yield 1;
        return "done";
    }

    public function three()
    {
        yield 1;
        yield 2;
        yield 3;
    }

    public function boom()
    {
        yield 1;
        throw new \RuntimeException("boom");
    }

    public static function seq(int n, double step, bool tail, string label)
    {
        int i = 1;
        double acc = 0.0;
        while i <= n {
            let acc = acc + step;
            yield acc;
            let i = i + 1;
        }
        if tail {
            yield label;
        }
    }

    public function arr(array items)
    {
        var k, v;
        for k, v in items {
            yield k, v;
        }
    }

    public function arrReverse(array items)
    {
        var v;
        for v in reverse items {
            yield v;
        }
    }

    public function nested(array matrix)
    {
        var row, x;
        for row in matrix {
            for x in row {
                yield x;
            }
        }
    }

    public function overItems()
    {
        var v;
        for v in this->items {
            yield v;
        }
    }

    public function range4(int n)
    {
        int i;
        for i in range(1, n) {
            yield i;
        }
    }

    public function viaSwitch(int mode)
    {
        switch mode {
            case 1:
                yield "one";
                break;
            default:
                yield "other";
        }
    }

    public function relay(<\Iterator> it)
    {
        var v;
        for v in it {
            yield v;
        }
    }

    public function consume(<\Iterator> it) -> array
    {
        var v;
        array result = [];
        for v in it {
            let result[] = v;
        }
        return result;
    }
}
