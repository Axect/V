fn main() {
	mut nums := [1,2,3]
	println(nums)
	println(nums[1])
	nums[1] = 5
	println(nums)
	println(nums.len)
	nums << 4
	println(nums)
	nums << [5,6,7]
	println(nums)

	// Initializing
	arr := []int{len: 5, init: -1}
	println(arr)

	mut numbers := []int{cap: 1000}
	println(numbers.len)
	for i in 0 .. 1000 {
		numbers << i
	}
	println(numbers.len)
}
