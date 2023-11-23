/*true*/

var a = ((true && !(false && true)) || false) || false
var b = ((true && !(false && true)) || false) || false
var c = a && b

write(c)
