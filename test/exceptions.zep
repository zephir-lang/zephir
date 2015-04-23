
namespace Test;

class Exceptions
{
    public function testException1()
    {
        throw new Exception("hello1");
    }

    public function testExceptionStringEscape()
    {
        throw new Exception("hello \"simple code\" test");
    }

    public function testException2()
    {
        var msg;
        let msg = "hello2";
        throw new Exception(msg);
    }

    public function testException3()
    {
        var ex, msg;
        let msg = "hello3";
        let ex = new Exception(msg);
        throw ex;
    }

    protected function getException()
    {
        return new Exception("hello4");
    }

    public function testException4()
    {
        throw this->getException();
    }

    public function testException5()
    {
        var exception;

        let exception = new Exception("hello5");
        throw exception;
    }

    public function testExceptionLiteral(string type)
    {
        switch(type) {
            case "string":
                throw "Test";
            case "char":
                throw 't';
            case "int":
                throw 123;
            case "double":
                throw 123.123;
        }
    }

    public function testExceptionSprintf(string name)
    {
        throw sprintf("Hello, %s", name);
    }

    public function testExceptionConcat(string framework, string language)
    {
        throw "Framework " . framework . " written using " . language;
    }

    public function testExceptionRethrow()
    {
        var e;
        try {
            this->testException1();
        }
        catch \Exception, e {
            throw e;
        }
    }
}
