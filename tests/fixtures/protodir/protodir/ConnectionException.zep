namespace ProtoDir;

use Fake\Http\Client\NetworkExceptionInterface;

class ConnectionException extends \RuntimeException implements NetworkExceptionInterface
{
	public function getRequest()
	{
	}
}
