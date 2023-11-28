/*4_3_2_1_24_3_2_1_14_3_2_1_03*/

var a = 3
var b = 3
var c = "_"

while (b != 0) {
    var a = 4
    while (a != 0) {
        var b = 5
        b = 4
        write(a, c)
        a = a - 1
    }
    b = b - 1
    write(b)
}
write(a)