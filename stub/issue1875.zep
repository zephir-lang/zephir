namespace Stub;

class Issue1875
{
    /**
     * Case A: var declared but only conditionally assigned, then read.
     * Mirrors the cphalcon Phalcon\Validation\Validator\StringLength snippet
     * from issue #1875.
     */
    public function conditionalThenRead(array! options) -> var
    {
        var included;

        if isset options["included"] {
            let included = options["included"];
        } elseif isset options["includedMinimum"] {
            let included = options["includedMinimum"];
        }

        return included;
    }

    /**
     * Case B: declared, assigned once unconditionally, then reassigned in
     * both branches of an if/else. Mirrors Min::validate from #1875.
     */
    public function reassignInBranches(array! options) -> var
    {
        var included;

        let included = options["seed"];

        if typeof included == "array" {
            let included = (bool) included["x"];
        } else {
            let included = (bool) included;
        }

        return included;
    }

    /**
     * Case C: declared but never assigned, returned directly. Should yield
     * null at runtime under PHP semantics.
     */
    public function neverAssigned() -> var
    {
        var x;
        return x;
    }

    /**
     * Case D: control mirroring the original report exactly, returning the
     * variable inside a constructor of another class.
     */
    public function forwardedToNewInstance(array! options) -> var
    {
        var included, validator;

        if isset options["included"] {
            let included = options["included"];
        }

        let validator = [
            "min": options["min"],
            "included": included
        ];

        return validator;
    }
}
