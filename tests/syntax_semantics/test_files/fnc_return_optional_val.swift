// 7

var x = true

func test(_ x: Bool) -> Int? {
    if x {
        return 10
    } else {
        return nil
    }
}

x = false
var num: Int = 0
num = test(x)