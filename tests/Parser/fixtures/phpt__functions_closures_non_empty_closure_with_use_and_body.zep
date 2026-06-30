namespace Example;

class Closure
{
    public function callback()
    {
        var abc = 42;

        return function (one, two, three) use (const & abc) {
            return [
                one,
                two,
                three,
                abc
            ];
        };
    }
}