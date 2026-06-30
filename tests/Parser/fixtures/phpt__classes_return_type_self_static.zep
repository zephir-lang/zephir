class MyClass {
    public function selfReturn() -> <self> {
        return new self();
    }

    public function staticReturn() -> <static> {
        return new static();
    }
}