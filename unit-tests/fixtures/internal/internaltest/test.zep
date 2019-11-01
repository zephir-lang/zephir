namespace InternalTest;

class Test
{
    internal function test1956() -> <Test>
    {
        return this;
    }

    public function issue1956() -> <Test>
    {
        return this->test1956();
    }
}
