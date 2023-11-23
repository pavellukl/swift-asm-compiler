/*15*/

func func_with_args0(_ a: Int, _ b: Int) -> Int {
    var c = func_with_args1(2, 3)
    return a + b + c
}

func func_with_args1(_ a: Int, _ b: Int) -> Int {
    return a * b
}

func_with_args0(1, 8)
