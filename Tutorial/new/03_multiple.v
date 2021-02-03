fn main() {
	a, b := foo()
	println(a)
	println(b)
	c, _ := foo()
	println(c)
}

fn foo() (int, int) {
	return 2, 3
}
