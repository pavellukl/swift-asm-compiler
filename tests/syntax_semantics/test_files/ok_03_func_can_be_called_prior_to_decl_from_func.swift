// 0
/*
func1
1
func2
func1
0
*/
var func1_count = 1

func func1() {
    write("func1", "\n")
    write(func1_count, "\n")

    if (func1_count == 0) {
        return
    } else {
    }

    func1_count = func1_count - 1

    func2()
}

func func2() {
    write("func2", "\n")

    func1()
}

func1()