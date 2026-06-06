
namespace Stub\Oo;

class ExtendPdoClass extends \PDO
{
	public function __construct(string dsn, string username = null, string password = null, attrs = [])
	{
		let attrs[\PDO::ATTR_STATEMENT_CLASS] = ["Stub\\PdoStatement"];
		parent::__construct(dsn, username, password, attrs);
	}
}
