function forms() {
	let a = function () -> int { };
	let b = function () use (x) -> string { };
	let c = function () -> void { return; };
	let d = function () use (x) -> array { return []; };
	let e = function (p) -> bool { };
	let f = function (p) use (x) -> double { };
	let g = function (p) -> int|string { return 1; };
	let h = function (p) use (x) -> <\ArrayObject> { return null; };
}