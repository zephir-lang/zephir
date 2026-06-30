namespace Stub;

class Issue2533
{
    const BAR = [1, 2, 3];
    const MAP = ["a": 1, "b": 2];
    const NESTED = [[1, 2], ["k": "v"]];
    const EMPTY_ARR = [];

    public function bar() -> array
    {
        return self::BAR;
    }

    public function map() -> array
    {
        return self::MAP;
    }

    public function nested() -> array
    {
        return self::NESTED;
    }
}
