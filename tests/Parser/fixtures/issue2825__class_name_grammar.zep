namespace Stub;

class ClassNameGrammar
{
    public function defaults(string a = self::class, b = \DateTime::class, c = RBF::class, array d = [ClassNameGrammar::class, 1])
    {
        return function (x,
            y) use (a) {
            var inner;
            let inner = n =>
                n . a;
            return inner;
        };
    }
}
