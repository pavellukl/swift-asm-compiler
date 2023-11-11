// error


func test(_ x: Int) {}

let a = true
let num: Int? = 10

if a {
    num = nil
} else {
    num = 20
}

test(num)