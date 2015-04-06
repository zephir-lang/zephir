
namespace Test;

class Fasta
{

	public function fastaRepeat(n, string seq)
	{
		var seqi = 0, len, i, j, k, l, block, str, lines;

		let len   = seq->length(),
			str   = join(seq, new array(len * 60 + 1)),
			lines = new array(len * len);

		let j = len * len,
			i = j;

		loop {
			if j <= -1 {
				break;
			}
			let j--, lines[j] = substr(str, 60 * j, 60);
		}

		let j = 0,
			l = floor(n / 60),
			k = floor(l / i),
			block = join("\n", lines);

		while j < k {
			echo block;
			let j++;
		}

		let j = 0,
			k = l % i;
		while j < k {
			echo lines[j];
			let j++;
		}

		if n % 60 > 0 {
			echo substr(lines[k], 0, n % 60);
		}
	 }

	 public function fastRandom()
	 {

	 }

	 public function main(n)
	 {
		var alu, iub, homoSap;

		let alu = "GGCCGGGCGCGGTGGCTCACGCCTGTAATCCCAGCACTTTGG" .
				"GAGGCCGAGGCGGGCGGATCACCTGAGGTCAGGAGTTCGAGA" .
				"CCAGCCTGGCCAACATGGTGAAACCCCGTCTCTACTAAAAAT" .
				"ACAAAAATTAGCCGGGCGTGGTGGCGCGCGCCTGTAATCCCA" .
				"GCTACTCGGGAGGCTGAGGCAGGAGAATCGCTTGAACCCGGG" .
				"AGGCGGAGGTTGCAGTGAGCCGAGATCGCGCCACTGCACTCC" .
				"AGCCTGGGCGACAGAGCGAGACTCCGTCTCAAAAA";

		let iub = [ "a": 0.27, "c": 0.12, "g": 0.12, "t": 0.27, "B": 0.02, "D": 0.02, "H": 0.02, "K": 0.02,
					"M": 0.02, "N": 0.02, "R": 0.02, "S": 0.02, "V": 0.02, "W": 0.02, "Y": 0.02 ];

		let homoSap = [
			"a": 0.3029549426680, "c": 0.1979883004921, "g": 0.1975473066391, "t": 0.3015094502008
		];

		echo ">ONE Homo sapiens alu";
		this->fastaRepeat(2 * n, alu);

		//echo ">TWO IUB ambiguity codes";
		//fastaRandom(3 * n, IUB)

		//echo ">THREE Homo sapiens frequency";
		//fastaRandom(5 * n, HomoSap)
	 }
}

