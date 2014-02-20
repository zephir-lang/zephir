
namespace Test;

class Resource
{
    public function testLetStatementSTDIN()
    {
        var a;
        let a = STDIN;

        return a;
    }

    public function testLetStatementSTDOUT()
    {
        var a;
        let a = STDOUT;

        return a;
    }

    public function testLetStatementSTDERR()
    {
        var a;
        let a = STDERR;

        return a;
    }

    public function testTypeOffResource()
    {
        var a;
        let a = STDIN;

        return typeof a;
    }
}
