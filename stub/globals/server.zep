namespace Stub\Globals;

class Server
{
    /**
    * @see https://github.com/phalcon/zephir/issues/1961
    */
    public function f1() -> void
    {
        echo _SERVER["PHP_SELF"];
        this->f2();
        echo _SERVER["PHP_SELF"];
    }

    public function f2() -> void
    {
        echo _SERVER["SCRIPT_NAME"];
    }

    /**
    * @see https://github.com/phalcon/zephir/issues/1970
    */
    public function check() -> var
    {
        return _SERVER["HTTP_USER_AGENT"];
    }
}
