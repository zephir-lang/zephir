
namespace Test;

class Quantum
{

	const PI = 3.14159265358979323846;

	const MAX = 751;

	/*
	 */
	public function harmos(double x)
	{
		int	 i, j, n;
		var  psr, psi, p2, v, paramater, fp;
		double dt, dx, k0, item_psr, item_psi;
		var tmp;

		let dx = 0.02,
			k0 = 3.0 * Quantum::PI,
			dt = dx * dx / 4.0;

		let paramater = [dx, k0, dt, x];
		let i 	= 0,
			psr = [],
			psi = [],
			p2  = [],
			v   = [];

		let fp = fopen("harmos.txt", "w");
		if !fp {
			return 1;
		}

		while i <= Quantum::MAX{
			let item_psi = sin(k0 * x) / exp(x * x * 2.0),
				item_psr = cos(k0 * x) / exp(x * x * 2.0);
			let psr[i] = [item_psr],
				psi[i] = [item_psi],
				v[i] = [5.0 * x * x],
				x = x + dx,
				i++;
		}

		let i = 1, j = 1, tmp = [2.0];
		for n in range(0, 20000) {

			for i in range(1, Quantum::MAX - 1) {
				let psr[i][1] = psr[i][0] - paramater[2] * (psi[i + 1][0] + psi[i - 1][0]
								- tmp[0] * psi[i][0]) / (paramater[0] * paramater[0]) + paramater[2] * v[i][0] * psi[i][0],

					p2[i] = psr[i][0]*psr[i][1] + psi[i][0]*psi[i][0];
			}

			for j in range(1, Quantum::MAX - 1) {
				let psr[0][1] = 0, psr[Quantum::MAX][1] = 0;
				let psi[j][1] = psi[j][0] + paramater[2] * (psr[j + 1][1] + psr[j - 1][1]
							  - tmp[0] * psr[j][1]) / (paramater[0] * paramater[0]) - paramater[2] * v[j][0] * psr[j][1];

			}

			//output split
			if n ==0 || n % 2000 == 0 {
				let i = 1;
				while i < Quantum::MAX - 1 {
					fprintf(fp, "%16.8lf %16.8lf %16.8lf \n", i * dx, n * dt, p2[i]);
					let i = i + 10;
				}
				fprintf(fp, "\n");
			}

			// change new->old
			let j = 1;
			while j <  Quantum::MAX - 1 {
				let psi[j][0] = psi[j][1],
					psr[j][0] = psr[j][1];
				let j++;
			}
		}

		return true;
	}
}
