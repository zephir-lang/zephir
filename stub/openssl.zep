
namespace Stub;

class Openssl
{
    public function randomPseudoBytes(int length) -> string
    {
        return openssl_random_pseudo_bytes(length);
    }

	public function randomPseudoBytesLoop() -> string
	{
	    var arr, i = 0;
	    let arr = [];
	    for i in range(1, 10) {
	        let arr[] = openssl_random_pseudo_bytes(10);
        }

        return arr;
	}
}
