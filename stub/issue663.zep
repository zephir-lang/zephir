namespace Stub;

class Issue663
{
    public static function is_array_assoc(arr) -> bool
    {
        if (!is_array(arr) || empty(arr)) {
            return false;
        }

        return static::is_array_assoc_internal(arr);
    }

    private static function is_array_assoc_internal(array arr) -> bool
    {
        int count, i;
        let count = count(arr);
        for i in range(0, count - 1) {
            if (!isset(arr[i])) {
                return true;
            }
        }

        return false;
    }

    public static function is_array_indexed(arr) -> bool
    {
        if (!is_array(arr) || empty(arr)) {
            return false;
        }

        return !static::is_array_assoc_internal(arr);
    }
}
