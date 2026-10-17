namespace Stub;

/**
 * Array keys in declarations: class constants and property defaults. PHP
 * converts a float, bool or null key and a canonical numeric string key once,
 * when the declaration is evaluated, so these are folded at compile time.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2706
 */
class Issue2706Keys
{
    const KEYS = [1.5: "a", true: "b", false: "c", null: "d", 2.0: "e", "3": "f", "03": "g", -1.5: "h"];

    const NESTED = ["x": [1.5: "a", true: "b", "4": "c"]];

    public keys = [1.5: "a", true: "b", false: "c", null: "d", 2.0: "e", "3": "f", "03": "g", -1.5: "h"];

    public static staticKeys = [1.5: "a", true: "b", false: "c", null: "d", 2.0: "e", "3": "f", "03": "g", -1.5: "h"];

    protected array typedKeys = [1.5: "a", true: "b", false: "c", null: "d", 2.0: "e", "3": "f", "03": "g", -1.5: "h"];

    public function getTypedKeys() -> array
    {
        return this->typedKeys;
    }

    public static function getStaticKeys() -> array
    {
        return self::staticKeys;
    }
}
