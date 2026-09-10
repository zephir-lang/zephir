class Handler
{
	public function run(#[Sensitive] string token, #[A] #[B(1)] int retries = 3, plain = null, #[C] ...rest)
	{
	}

	public function make()
	{
		let callback = function(#[D("x")] int value) {
			return value;
		};
	}
}

interface Contract
{
	public function handle(#[Sensitive] string secret);
}

function topLevel(#[E, F] var input)
{
}