/*4_3_2_1_2103*/

var a = 3
var b = 3
var c = "_"

while (b != 0) {
    while (a != 0) {
        var a = 4
        var b = 5
        b = 4
        write(a, c)
        a = a - 1
    }
    b = b - 1
    write(b)
}
write(a)