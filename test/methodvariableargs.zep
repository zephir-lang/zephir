namespace Test;


class MethodVariableArgs
{
    // Without type
    public function testWithoutType1(int a, int b, ...args) -> int
    {

    }

    // Without type
    public function testWithoutType2(int a, int b, ...args) -> int
    {

    }

    // With type
    public function testWithType(int a, int ...args) -> int
    {

    }

    // With strict-type
    public function testWithTypeStrict(int! ...args) -> int
    {

    }

    // With type-hint
    public function testWithTypeHint(<\StdClass> ...args) -> int
    {

    }
}
