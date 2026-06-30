namespace Example;

class Test
{
    public function block()
    {
        %{

            // Some comment

            {
                while(1) {
                    RETURN_MM_NULL();
                }
            }
        }%
    }
}