/*7_3_1*/

var a = 1

if (a == 0) {
    var a = 0
    write("1_")
} else if (false) {
    var a = 0
    write("2_")
} else if (!(a == 0)) {
    var a = 2
    if ((a == 0)) {
        write("5_")
    } else if (false) {
        write("6_")
    } else if (a == 2) {
        write("7_")
    } else {
        write("8_")
    }
    write("3_")
} else {
    var a = 0
    if (a == 1) {
        write("9_")
    } else if (false) {
        write("10_")
    } else if (a == 1) {
        write("11_")
    } else {
        write("12_")
    }
    write("4_")
}

write(a)