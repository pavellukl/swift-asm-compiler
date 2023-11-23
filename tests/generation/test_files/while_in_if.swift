/*213*/

var a = 3

if (a == 2) {
    write(1)
} else if (false) {
    write(2)
} else if (a == 3) {
    var a = 2
    while (a > 0) {
        write(a)
        a = a - 1
    }
} else {
    write(4)
}

write(a)
