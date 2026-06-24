namespace Stub;

use ReflectionNamedType;
use ReflectionType;

class Issue2565
{
    public function resolveType(<ReflectionType> type) -> string | null
    {
        if type instanceof ReflectionNamedType {
            return type->getName();
        }

        return null;
    }
}
