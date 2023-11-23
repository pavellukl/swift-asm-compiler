/*33*/

var a = false

if (a) {
    if (a) {
        write(1)
    } else if (false) {
        write(2)
    } else if (!a) {
        write(3)
    } else {
        write(4)
    }
    write(1)
} else if (false) {
    if (a) {
        write(1)
    } else if (false) {
        write(2)
    } else if (!a) {
        write(3)
    } else {
        write(4)
    }
    write(2)
} else if (!a) {
    if (a) {
        write(1)
    } else if (false) {
        write(2)
    } else if (!a) {
        write(3)
    } else {
        write(4)
    }
    write(3)
} else {
    if (a) {
        write(1)
    } else if (false) {
        write(2)
    } else if (!a) {
        write(3)
    } else {
        write(4)
    }
    write(4)
}