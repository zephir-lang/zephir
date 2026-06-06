function test() {
    let a = ~umask();
    let b = 0666 & ~umask();
}