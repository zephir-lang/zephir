
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

	public function opensslEncrypt(string data)
	{
	    return openssl_encrypt(data, "AES-256-CBC", "passphrase", OPENSSL_RAW_DATA, "1234567890123456");
	}
}
