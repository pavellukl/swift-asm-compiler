//0
/*
ok #\
123
0x1.ecp+6
0x1.ecp+6
123
0
5
Hello
World
nil_
nil_
nil_
nil_
nil_
H
0
72
H
*/

write("ok", " #", "\\", "\n", 123, "\n", 123.0, "\n")

let int_2_double : Double = Int2Double(123)
write(int_2_double, "\n")

let double_2_int : Int = Double2Int(123.0)
write(double_2_int, "\n")

let empty_string : String = ""
let len_empty = length(empty_string)
write(len_empty, "\n")

let a_string : String = "te\nst"
let len = length(a_string)
write(len, "\n")

let hello = "Hello World"

let substr : String? = substring(of :hello, startingAt: 0, endingBefore: 5)
write(substr, "\n")

let substr_world : String? = substring(of :hello, startingAt: 6, endingBefore: 11)
write(substr_world, "\n")

// i < 0
let neg_one : Int = 0-1

var substr_neg : String? = substring(of :hello, startingAt: neg_one, endingBefore: 11)
substr_neg = substr_neg ?? "nil_"
write(substr_neg, "\n")

// j < 0
var substr_neg_2 : String? = substring(of :hello, startingAt: 0, endingBefore: neg_one)
substr_neg_2 = substr_neg_2 ?? "nil_"
write(substr_neg_2, "\n")

// i > j
var substr_neg_3 : String? = substring(of :hello, startingAt: 5, endingBefore: 0)
substr_neg_3 = substr_neg_3 ?? "nil_"
write(substr_neg_3, "\n")

// i >= len
var substr_neg_4 : String? = substring(of :hello, startingAt: 11, endingBefore: 11)
substr_neg_4 = substr_neg_4 ?? "nil_"
write(substr_neg_4, "\n")

// j > len
var substr_neg_5 : String? = substring(of :hello, startingAt: 0, endingBefore: 12)
substr_neg_5 = substr_neg_5 ?? "nil_"
write(substr_neg_5, "\n")

// one char
var substr_one : String? = substring(of :hello, startingAt: 0, endingBefore: 1)
write(substr_one, "\n")

// empty string
var substr_empty : String? = substring(of :hello, startingAt: 0, endingBefore: 0)
var substr_empty_no_nil : String = substr_empty!
var empty_len = length(substr_empty_no_nil)
write(empty_len, "\n")

// ord
let ord_res : Int = ord("H")
write(ord_res, "\n")

// chr
let chr_res : String = chr(72)
write(chr_res, "\n")
