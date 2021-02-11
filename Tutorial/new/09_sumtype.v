fn main() {
	a := AD { 1.0, 2.0 }
	b := AD { 2.0, 3.0 }
	println(a + b)
}

struct AD {
	x f64
	y f64
}

fn (a AD) + (b AD) AD {
	return AD { a.x + b.x, a.y + b.y }
}
