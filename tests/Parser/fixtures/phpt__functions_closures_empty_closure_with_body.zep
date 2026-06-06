namespace Example;

class Closure
{
    public function callback()
    {
        var abc = 42;

        return function () use (&abc) {
            var r;

            let r = abc;
            let abc = null;

            return r;
        };
    }
}