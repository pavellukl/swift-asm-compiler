// 0

func decrement(of n: Int, by m: Int) -> Int {
    return n - m
}

func factorial(_ n : Int) -> Int? {
    var result : Int?
    if (n < 2) {
        result = 1
    } else {
        let decremented_n = decrement(of: n, by: 1)
        let temp_result = factorial(decremented_n)
        result = n * temp_result
    }

    return result!
}

func test3(_ _ :Int, _ _: String, _ _: Double, _ _: Bool, of x: Int?) -> Int? {
    return x
}

func test2() -> Int {
    return 1
}

func test4(of x: Int, by y: Int?, _ _: Int?, with z: String, _ e: Bool) {}


let x = test2() + 6
var y = "hello"
var z = 72.999
var _1 = true

var num: Int? = 9
num = test3(x, y, z, _1, of: x)

var factRes = factorial(num!)

num = test3(x, y, z, _1, of: nil)

num = 9

test4(num, of: x, _1, with: y, by: nil)


