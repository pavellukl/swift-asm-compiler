// 0
/*
b
b

Was Nil
*/
var a: String? = "b"
var b: String = a ?? "Was Nil"

write(a, "\n")
write(b, "\n")

var c: String? = nil
var d: String = c ?? "Was Nil"

write(c, "\n")
write(d, "\n")