class AssetCollection {
    public function add(<AssetInterface> asset) -> <static> {
        this->addAsset(asset);
        return this;
    }
}