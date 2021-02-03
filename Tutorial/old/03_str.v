fn main() {
    name := 'Bob'
    println('Hello, $name!')
    println(name.len)

    bobby := name + 'by'
    println(bobby)

    println(bobby.substr(1, 3))

    mut s := 'hello '
    s += 'world'
    println(s)
}