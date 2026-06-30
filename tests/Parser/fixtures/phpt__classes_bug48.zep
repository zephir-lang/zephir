namespace Acme;

use Psr\Http\Client\ClientExceptionInterface;

final class ServerException extends \RuntimeException implements ClientExceptionInterface
{
}