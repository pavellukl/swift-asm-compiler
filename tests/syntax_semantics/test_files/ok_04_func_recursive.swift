// 0
/*
3
2
1
0
*/

var a : Int = 3

func recursive() {
    write(a, "\n")

    if (a == 0) {
        return
    } else {
        a = a - 1
        recursive()
    }
}

recursive()