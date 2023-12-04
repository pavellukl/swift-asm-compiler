// 9
// This will actually produce cannot assign to a constant
let str : String? = "test"

if let str {
    var nillable: String?
    str = nillable
} else {

}