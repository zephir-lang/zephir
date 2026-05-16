namespace Stub;

class Issue2385
{
    public declaredOnly;
    public assignedNull;
    public assignedValue;

    public function __construct()
    {
        let this->assignedNull = null;
        let this->assignedValue = "set";
    }

    public function checkDeclaredOnly() -> bool
    {
        return isset(this->declaredOnly);
    }

    public function checkAssignedNull() -> bool
    {
        return isset(this->assignedNull);
    }

    public function checkAssignedValue() -> bool
    {
        return isset(this->assignedValue);
    }

    public function checkDynamicKeyNull(string key) -> bool
    {
        return isset(this->{key});
    }

    public function checkArrayKeyNull() -> bool
    {
        var arr;
        let arr = ["a": null];
        return isset(arr["a"]);
    }

    public function checkArrayKeyValue() -> bool
    {
        var arr;
        let arr = ["a": "v"];
        return isset(arr["a"]);
    }

    public function checkArrayKeyMissing() -> bool
    {
        var arr;
        let arr = ["b": "v"];
        return isset(arr["a"]);
    }

    public function checkArrayLongKeyNull() -> bool
    {
        var arr;
        let arr = [0: null];
        return isset(arr[0]);
    }

    public function checkArrayLongKeyValue() -> bool
    {
        var arr;
        let arr = [0: "v"];
        return isset(arr[0]);
    }

    public function checkArrayVarKeyNull(var key) -> bool
    {
        var arr;
        let arr = ["a": null];
        return isset(arr[key]);
    }

    public function checkArrayVarKeyValue(var key) -> bool
    {
        var arr;
        let arr = ["a": "v"];
        return isset(arr[key]);
    }
}
