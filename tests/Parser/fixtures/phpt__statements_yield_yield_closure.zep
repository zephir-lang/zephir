function loop_from_closure() {
    var closure, generator, key, value;

    let closure = function()  {
        var k, v;

        for k, v in ["a", "b", "c", "d"] {
            yield k, v;
        }
    };

    let generator = closure();
    for key, value in generator {
        echo key . "-" . val;
    }
}