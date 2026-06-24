namespace Example;

class Closure
{
    public function callback(resource fp, filter)
    {
        var line, debug = [];
        let line = fgets(fp);

        let line = preg_replace_callback(
            "|<p>\s*\w|",
            function (matches) use ( & debug, const filter ) {
                let debug += [matches];

                if (matches[0] !== filter) {
                    return strtolower(matches[0]);
                }

                return matches[0];
            },
            line
        );

        return line;
    }
}