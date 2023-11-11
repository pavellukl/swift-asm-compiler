// OK

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

func test3(_ _ : Int, _ _: String, _ _: Double, _ _: Bool, of x: Int?) -> Int? {
    return x
}

func test2() -> Int {
    return 1
}

func test() -> Int {
    if(true) {
        return 0
    } else {
        return 1
    }
}

func print() {
    let x = "xxx"
}