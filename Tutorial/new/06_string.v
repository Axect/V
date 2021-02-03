fn main() {
	name := 'Bob'
	println(name.len)
	println(name[0])
	println(name[1..3])
	windows_newline := '\r\n'
	assert windows_newline.len == 2

	assert 'aloha!'[0] == `a` // Backtit!

	s := '42'
	n := s.int() // That's it
	println(n)

	println("Hello, $name!")

	x := 123.4567

	println('x = ${x:4.2f}')
	println('[${x:10}]')
	println('[${int(x):-10}]')
	println('[${int(x):010}]')

	bobby := name + 'by' // concatenate by +
	println(bobby)
	mut h := 'hello '
	h += 'world'
	println(h)

	age := 11
	println('age = ' + age.str())
	println('age = $age')
}
