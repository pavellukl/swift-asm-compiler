// 0
// NO_INT
/*
Hello world

Hello
11
72
A
123 -> Int2Double
123.0
123.456 -> Double2Int
123
*/
let str: String = "Hello world"
write(str, "\n", "\n")
let substr: String? = substring(of: str, startingAt: 0, endingBefore: 5)
write(substr, "\n")
let length_: Int = length(str)
write(length_, "\n")
let ord_: Int = ord(str)
write(ord_, "\n")
let chr_: String = chr(65)
write(chr_, "\n")

write("123 -> Int2Double\n")
let int2double: Double = Int2Double(123)
write(int2double, "\n")

write("123.456 -> Double2Int\n")
let double2int: Int = Double2Int(123.456)
write(double2int, "\n")

let read_int: Int? = readInt()
write(read_int, "\n")
let read_double: Double? = readDouble()
write(read_double, "\n")
let read_string: String? = readString()
write(read_string, "\n")
