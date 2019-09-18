fn main() {
    mut m := map[string]int
    m['one'] = 1
    m['two'] = 2
    println(m['one'])
    println(m['bad_key'])
    println('two' in m)
    m.delete('two')

    numbers := {
        'one': 1,
        'two': 2
    }

    println(numbers['one'])
}