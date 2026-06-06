function generate() {
    var k, v;

    for k, v in ["a", "b", "c", "d"] {
        yield k, v;
    }
}

function loop_generator() {
    var key, value;

    for key, value in generate() {
        echo key . "-" . val;
    }
}