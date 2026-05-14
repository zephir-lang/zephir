namespace Stub;

class ArrayManipulation
{
    protected arrayFillQueue = [];

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1187
     */
    public function arrayFillMany() -> array
    {
        let this->arrayFillQueue = array_fill(0, 1000000, "message");

        return this->arrayFillQueue;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/774
     */
    public function issue774() -> array
    {
        var obj;

        let
            obj = new \stdClass(),
            obj->prop0 = 0,
            obj->prop1 = 1,
            obj = (array)obj;

        return obj;
    }
}
