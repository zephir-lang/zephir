namespace Example;

class Closure
{
    public function callback()
    {
        var abc = 42;

        return function () use (abc) { };
    }
}