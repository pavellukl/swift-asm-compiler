// 0
func test() {
    if 5 == 5 {

    }
    else if 5 == 5 {}
    else {

    }
}

func test0() -> Int {
    if(1 > 0) {
        return 1

    } 
    else if (2 > 0) {
        return 2
    }

    return 3
}

func test1() -> Int {
    if(1 > 0) {
    } 
    else if (2 > 0) {
    }

    return 1
}

func test2() -> Int {
    if(1 > 0) {
        return 1
    } 
    else if (2 > 0) {
        return 2
    }
    else {
        return 0
    }
}

func test3() -> Int {
    if(1 > 0) {
    } 
    else if (2 > 0) {
    }
    else {
        return 0
    }

    return 1
}

func test4() -> Int {
    if(1 > 0) {
        if (true) {
            return 1
        } else {
            return 2
        }
    } 
    else if (2 > 0) {
        return 3
    }
    else if false {
        if true {
            return 4
        } else if false {
        } else {
            return 5
        }

        return 6
    }
    else {
        if (true) {
        } else {
        }

        return 7
    }
}

func test5() -> Int {
    if(1 > 0) {
      if true {
            return 1
        } else if false {
        } else {
            return 2

        }
    } 
    else if (2 > 0) {
          if true {
        } else if false {
            return 3
        } else {

        }
    }
    else {
          if true {
        } else if false {
        } else {
            return 5
        }
    }

    return 0
}
