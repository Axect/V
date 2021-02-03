fn main() {
	println(sum())
	println(sum(1))
	println(sum(2,3))
	a := [2,3,4]
	println(a)
	println(sum(...a))
	b := [5,6,7]
	println(b)
	println(sum(...b))
}

fn sum(a ...int) int {
	mut total := 0
	for x in a {
		total += x
	}
	return total
}
