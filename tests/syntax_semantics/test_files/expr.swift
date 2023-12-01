// 0
let complexExpression1 = (5 + 3) * (10 - 2) / 4
let complexExpression2 = (12 * 2 + 8) / (4 + 2)
let complexExpression3 = (2 * 3 + 5) * (8 / 2) - 7
let complexExpression4 = (15 - 3) + (7 * 2) / 4
write(complexExpression1, "\n")
write(complexExpression2, "\n")
write(complexExpression3, "\n")
write(complexExpression4, "\n")

let precedenceResult = 2 + 3 * 4 / 2 - 1
write(precedenceResult, "\n")

let compoundExpression = (3 + 2) * (5 - 1) / (2 + 1)
write(compoundExpression, "\n")

let nestedExpression = ((4 * 5) - (8 / 2)) / 3 + 7
write(nestedExpression, "\n")

let nestedExpression2 = ((4 * 5) - (8 / 2)) / (3 + 7)
write(nestedExpression2, "\n")

let longComplexExpression = ((((2 + 5) * (3 - 1)) + ((8 / 2) + 4)) * (7 - 3) + ((6 * 2) / (5 + 2)) - ((12 / 5) + 3) * 2) / (((9 + 3) * (4 - 2)) + ((15 / 3) + 7) - ((10 / 4) + 1) * 5)
write(longComplexExpression, "\n")

// Nullable Variables
let optionalValue1: Int? = 10
let optionalValue2: Int? = nil

// Arithmetic Expression with Nil-Coalescing Operator
let result1 = (optionalValue1 ?? 0) * 5
let result2 = (optionalValue2 ?? 0) + 8
let result3 = (optionalValue1 ?? 0) - (optionalValue2 ?? 0)

// Displaying Results
write(result1, "\n")
write(result2, "\n")
write(result3, "\n")

// Nullable Variables
let optionalValueA: Int? = 15
let optionalValueB: Int? = nil
let optionalValueC: Int? = 7

// Complex Arithmetic Expression with Nil-Coalescing Operator
let longComplexExpression2 = (((optionalValueA ?? 0) + 10) * ((optionalValueB ?? 1) - 3) / ((optionalValueC ?? 2) + 1)) + (((optionalValueA ?? 0) * 2) - ((optionalValueB ?? 1) / 2) * ((optionalValueC ?? 2) + 3))

// Displaying Result
write(longComplexExpression2, "\n")

// Nullable Variables
let optionalValueX: Int? = 8
let optionalValueY: Int? = 5
let optionalValueZ: Int? = nil

// Long Arithmetic Expression with Forced Unwrapping
let longExpression = (((optionalValueX ?? 0) + 2) * ((optionalValueY ?? 1) - 3) / ((optionalValueZ ?? 2)! + 1)) + (((optionalValueX ?? 0) * 3) - ((optionalValueY ?? 1) / 2) * ((optionalValueZ ?? 2)! + 4))

// Displaying Result
write(longExpression, "\n")


