namespace Test;

class Fannkuch
{

	public function process(int n)
	{
		var s, p, q, t, q0, qq, sx;
		int i, j, m, maxflips, checksum, sign, flips;

		let s = range(0, n - 1),
			i = 0,
			maxflips = 0,
			checksum = 0,
			sign = 1,
			m = n - 1,
			q = s,
			p = s;

		do {
			// Copy and flip.
			let q0 = p[0];
			if q0 != 0 {
				let q = p, flips = 1;
				do {
					let qq = q[q0];
					if qq == 0 {
						let checksum += sign * flips;
						if flips > maxflips {
							let maxflips = flips;
						}
						break;
					}
					let q[q0] = q0;
					if q0 >= 3 {
						let i = 1, j = q0 - 1;
						do {
							let t = q[i],
								q[i] = q[j],
								q[j] = t,
								i++,
								j--;
						} while i < j;
					}
					let q0 = qq, flips++;
				} while true;
			}

			// Permute.
			if sign == 1 {
				let t = p[1], p[1] = p[0], p[0] = t, sign = -1; // Rotate 0<-1.
			} else {
				let t = p[1], p[1] = p[2], p[2] = t, sign = 1;  // Rotate 1<-2.
				let i = 2;
				while i < n {
					let sx = s[i];
					if sx != 0 {
						let sx--, s[i] = sx; // ?
						break;
					}
					if i == m {
						return printf("%d\nPfannkuchen(%d) = %d\n", checksum, n, maxflips); // Out of permutations.
					}
					let s[i] = i;
					// Rotate 0<-...<-i+1.
					let t = p[0], j = 0;
					while j <= i {
						let p[j] = p[j], j++;
					}
					let i++, p[i] = t;
				}
			}
		} while true;
	}

}