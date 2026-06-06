namespace Stub;

class Issue2564
{
    protected cipher;
    protected useSigning;
    protected padFactory;

    public function __construct(
        string cipher = "aes-256-cfb",
        bool useSigning = true,
        <Issue2564PadFactory> padFactory = null
    ) {
        let this->cipher = cipher;
        let this->useSigning = useSigning;
        let this->padFactory = padFactory;
    }

    public function getCipher() -> string
    {
        return this->cipher;
    }
}
