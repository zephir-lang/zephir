
namespace Test;

/**
 * RegexDNA
 *
 * @see http://www.haskell.org/haskellwiki/Shootout/Regex_DNA
 */
class RegexDNA
{

	public function process(var path)
	{
		var variants, vIUB, vIUBnew, stuffToRemove, contents, initialLength,
			regex, codeLength, discard = null;

		let variants = [
			"agggtaaa|tttaccct",
			"[cgt]gggtaaa|tttaccc[acg]",
			"a[act]ggtaaa|tttacc[agt]t",
			"ag[act]gtaaa|tttac[agt]ct",
			"agg[act]taaa|ttta[agt]cct",
			"aggg[acg]aaa|ttt[cgt]ccct",
			"agggt[cgt]aa|tt[acg]accct",
			"agggta[cgt]a|t[acg]taccct",
			"agggtaa[cgt]|[acg]ttaccct"
		];

		let vIUB = [], 			vIUBnew = [],
			vIUB[] = "/B/S",    vIUBnew[] = "(c|g|t)",
			vIUB[] = "/D/S",    vIUBnew[] = "(a|g|t)",
			vIUB[] = "/H/S",    vIUBnew[] = "(a|c|t)",
			vIUB[] = "/K/S",    vIUBnew[] = "(g|t)",
			vIUB[] = "/M/S",    vIUBnew[] = "(a|c)",
			vIUB[] = "/N/S",    vIUBnew[] = "(a|c|g|t)",
			vIUB[] = "/R/S",    vIUBnew[] = "(a|g)",
			vIUB[] = "/S/S",    vIUBnew[] = "(c|g)",
			vIUB[] = "/V/S",    vIUBnew[] = "(a|c|g)",
			vIUB[] = "/W/S",    vIUBnew[] = "(a|t)",
			vIUB[] = "/Y/S",    vIUBnew[] = "(c|t)";

		let stuffToRemove = "^>.*$|\n",
			discard = null;

		//read in file
		let contents = file_get_contents(path),
			initialLength = strlen(contents);

		//remove things
		let contents = preg_replace("/" . stuffToRemove . "/mS", "", contents),
			codeLength = strlen(contents);

		//do regexp counts
		for regex in variants {
			echo regex , " " , preg_match_all("/" . regex . "/iS", contents, discard), '\n';
		}

		// do replacements
		let contents = preg_replace(vIUB, vIUBnew, contents);

		echo '\n', initialLength, '\n', codeLength, '\n', strlen(contents), '\n';
	}

}
