namespace Stub\Issue2165;

use InvalidArgumentException;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2165
 */
class Issue implements IssueInterface
{
    protected m;
    protected n;

    public static function build(array a = []) -> <Issue>
    {
        return new self(a, true);
    }

    public function __construct(array a, const bool validate = true)
    {
        var i, rowA, valueA;

        int m = count(a);
        int n = count(current(a));

        if validate {
            let a = array_values(a);

            for i, rowA in a {
                if unlikely count(rowA) !== n {
                    throw new InvalidArgumentException("The number of columns"
                        . " must be equal for all rows, " . strval(n)
                        . " needed but " . count(rowA) . " given"
                        . " at row offset " . i . ".");
                }

                for valueA in rowA {
                    if unlikely !is_int(valueA) && !is_float(valueA) {
                        throw new InvalidArgumentException("Matrix element must"
                            . " be an integer or floating point number, "
                            . gettype(valueA) . " given.");
                    }
                }

                let rowA[] = array_values(rowA);
            }
        }

        let this->a = a;
        let this->m = m;
        let this->n = n;
    }

    public static function quick(const array a = []) -> <Issue>
    {
        return new self(a, false);
    }

    public static function ones(const int m, const int n) -> <Issue>
    {
        return self::fill(1, m, n);
    }

    public static function fill(const value, const int m, const int n) -> <Issue>
    {
        if unlikely !is_int(value) && !is_float(value) {
            throw new InvalidArgumentException("Value must be an"
                . " integer or floating point number, "
                . gettype(value) . " given.");
        }

        if unlikely m < 1 {
            throw new InvalidArgumentException("M must be"
                . " greater than 0, " . strval(m) . " given.");
        }

        if unlikely n < 1 {
            throw new InvalidArgumentException("N must be"
                . " greater than 0, " . strval(n) . " given.");
        }

        return self::quick(array_fill(0, m, array_fill(0, n, value)));
    }

    public function reciprocal() -> <Issue>
    {
        return self::ones(this->m, this->n)
            ->divideMatrix(this);
    }

    public function divideMatrix(const <Issue> b) -> <Issue>
    {
        array c = [];

        return self::quick(c);
    }
}
