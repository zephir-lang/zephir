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
}
