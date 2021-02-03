fn main() {
	mut a := 0
	mut b := 1
	println('$a, $b')
	a, b = b, a
	println('$a, $b')
}
