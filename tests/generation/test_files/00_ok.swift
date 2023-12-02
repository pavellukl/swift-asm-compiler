/* hello */
/* hello 12 3.1415 asdas 555 551.6295 qergr 81 15.0 qwd */

var a : String = "hello"
var b : String
var c = "hello"

let d : String = "hello"
let e : String
let f = "hello"

if 1 == 1 {
    write(a)
} else {
    write(a)
}

while 1 == 1 {
    write(a)
}

// Function definition
func fn() {}

func fn2(_ a : String) -> String {
    fn3("a", "b")
    fn2(a)

    return a
}

func fn3(_ a : String, _ b : String) -> String {
    return a
}

func fn4(aa a : String, bb b : String) -> String {
    return a
}

// Function call
fn()
fn2(a)
fn2("lit")
fn3(a, d)
fn3("lit", "lit")
fn4(aa: a, bb: d)
fn4(aa: "lit", bb: "lit")
write(123, "abc", 123.123)

// Assignment
a = "hello"
let read_str : String? = readString()
let read_int : Int? = readInt()
let read_double : Double? = readDouble()

let a_double : Double = Int2Double(123)
let an_int : Int = Double2Int(123.123)

let len : Int = length(a)
let substr : String? = substring(of :a, startingAt: 0, endingBefore: 1)
let ord_res : Int = ord(a)
let chr_res : String = chr(123)

// Infer type from function call
let infer1 = fn2(a)

// If let String? -> String
func arg_str(_ a : String) {}

let str_nillable : String? = "test"
if let str_nillable {
    arg_str(str_nillable)
} else {}


// Redefine variable in scope
func accept_int(_ a : Int) {}

var a_string: String = "test"

if 1 == 1 {
    var a_string : Int = 123
    accept_int(a_string)
    write(a_string)
} else {
}

// Redefine a variable in function
func redef_var() {
    let a_string : String = "test"

    if 1 == 1 {
        var a_string : Int = 123
        accept_int(a_string)
        write(a_string)
    } else {
    }

    write(a_string)
}

// Pass nil as an argument
func nil_arg(_ a : String?) {}

nil_arg(nil)

// Assign nil to a variable
var nil_var : String? = nil
var nil_var_2 : Int?
nil_var_2 = nil
