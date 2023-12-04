.IFJcode23

  CREATEFRAME
  PUSHFRAME
  DEFVAR GF@complexExpression1
  DEFVAR GF@complexExpression2
  DEFVAR GF@complexExpression3
  DEFVAR GF@complexExpression4
  DEFVAR GF@precedenceResult
  DEFVAR GF@compoundExpression
  DEFVAR GF@nestedExpression
  DEFVAR GF@nestedExpression2
  DEFVAR GF@longComplexExpression
  DEFVAR GF@optionalValue1
  DEFVAR GF@optionalValue2
  DEFVAR GF@result1
  DEFVAR GF@result2
  DEFVAR GF@result3
  DEFVAR GF@optionalValueA
  DEFVAR GF@optionalValueB
  DEFVAR GF@optionalValueC
  DEFVAR GF@longComplexExpression2
  DEFVAR GF@optionalValueX
  DEFVAR GF@optionalValueY
  DEFVAR GF@optionalValueZ
  DEFVAR GF@longExpression
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@5
  PUSHS int@3
  ADDS
  PUSHS int@10
  PUSHS int@2
  SUBS
  MULS
  PUSHS int@4
  IDIVS
  POPS GF@complexExpression1
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@12
  PUSHS int@2
  MULS
  PUSHS int@8
  ADDS
  PUSHS int@4
  PUSHS int@2
  ADDS
  IDIVS
  POPS GF@complexExpression2
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@2
  PUSHS int@3
  MULS
  PUSHS int@5
  ADDS
  PUSHS int@8
  PUSHS int@2
  IDIVS
  MULS
  PUSHS int@7
  SUBS
  POPS GF@complexExpression3
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@15
  PUSHS int@3
  SUBS
  PUSHS int@7
  PUSHS int@2
  MULS
  PUSHS int@4
  IDIVS
  ADDS
  POPS GF@complexExpression4
  PUSHS string@\010
  PUSHS GF@complexExpression1
  PUSHS int@2
  CALL _write
  PUSHS string@\010
  PUSHS GF@complexExpression2
  PUSHS int@2
  CALL _write
  PUSHS string@\010
  PUSHS GF@complexExpression3
  PUSHS int@2
  CALL _write
  PUSHS string@\010
  PUSHS GF@complexExpression4
  PUSHS int@2
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@2
  PUSHS int@3
  PUSHS int@4
  MULS
  PUSHS int@2
  IDIVS
  ADDS
  PUSHS int@1
  SUBS
  POPS GF@precedenceResult
  PUSHS string@\010
  PUSHS GF@precedenceResult
  PUSHS int@2
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@3
  PUSHS int@2
  ADDS
  PUSHS int@5
  PUSHS int@1
  SUBS
  MULS
  PUSHS int@2
  PUSHS int@1
  ADDS
  IDIVS
  POPS GF@compoundExpression
  PUSHS string@\010
  PUSHS GF@compoundExpression
  PUSHS int@2
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@4
  PUSHS int@5
  MULS
  PUSHS int@8
  PUSHS int@2
  IDIVS
  SUBS
  PUSHS int@3
  IDIVS
  PUSHS int@7
  ADDS
  POPS GF@nestedExpression
  PUSHS string@\010
  PUSHS GF@nestedExpression
  PUSHS int@2
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@4
  PUSHS int@5
  MULS
  PUSHS int@8
  PUSHS int@2
  IDIVS
  SUBS
  PUSHS int@3
  PUSHS int@7
  ADDS
  IDIVS
  POPS GF@nestedExpression2
  PUSHS string@\010
  PUSHS GF@nestedExpression2
  PUSHS int@2
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@2
  PUSHS int@5
  ADDS
  PUSHS int@3
  PUSHS int@1
  SUBS
  MULS
  PUSHS int@8
  PUSHS int@2
  IDIVS
  PUSHS int@4
  ADDS
  ADDS
  PUSHS int@7
  PUSHS int@3
  SUBS
  MULS
  PUSHS int@6
  PUSHS int@2
  MULS
  PUSHS int@5
  PUSHS int@2
  ADDS
  IDIVS
  ADDS
  PUSHS int@12
  PUSHS int@5
  IDIVS
  PUSHS int@3
  ADDS
  PUSHS int@2
  MULS
  SUBS
  PUSHS int@9
  PUSHS int@3
  ADDS
  PUSHS int@4
  PUSHS int@2
  SUBS
  MULS
  PUSHS int@15
  PUSHS int@3
  IDIVS
  PUSHS int@7
  ADDS
  ADDS
  PUSHS int@10
  PUSHS int@4
  IDIVS
  PUSHS int@1
  ADDS
  PUSHS int@5
  MULS
  SUBS
  IDIVS
  POPS GF@longComplexExpression
  PUSHS string@\010
  PUSHS GF@longComplexExpression
  PUSHS int@2
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@10
  POPS GF@optionalValue1
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS nil@nil
  POPS GF@optionalValue2
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@optionalValue1
  PUSHS int@0
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &23-0 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &23-1
LABEL &23-0
  PUSHS TF@temp1
LABEL &23-1
  PUSHS int@5
  MULS
  POPS GF@result1
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@optionalValue2
  PUSHS int@0
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &25-0 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &25-1
LABEL &25-0
  PUSHS TF@temp1
LABEL &25-1
  PUSHS int@8
  ADDS
  POPS GF@result2
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@optionalValue1
  PUSHS int@0
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &27-0 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &27-1
LABEL &27-0
  PUSHS TF@temp1
LABEL &27-1
  PUSHS GF@optionalValue2
  PUSHS int@0
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &27-2 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &27-3
LABEL &27-2
  PUSHS TF@temp1
LABEL &27-3
  SUBS
  POPS GF@result3
  PUSHS string@\010
  PUSHS GF@result1
  PUSHS int@2
  CALL _write
  PUSHS string@\010
  PUSHS GF@result2
  PUSHS int@2
  CALL _write
  PUSHS string@\010
  PUSHS GF@result3
  PUSHS int@2
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@15
  POPS GF@optionalValueA
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS nil@nil
  POPS GF@optionalValueB
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@7
  POPS GF@optionalValueC
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@optionalValueA
  PUSHS int@0
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &35-0 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &35-1
LABEL &35-0
  PUSHS TF@temp1
LABEL &35-1
  PUSHS int@10
  ADDS
  PUSHS GF@optionalValueB
  PUSHS int@1
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &35-2 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &35-3
LABEL &35-2
  PUSHS TF@temp1
LABEL &35-3
  PUSHS int@3
  SUBS
  MULS
  PUSHS GF@optionalValueC
  PUSHS int@2
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &35-4 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &35-5
LABEL &35-4
  PUSHS TF@temp1
LABEL &35-5
  PUSHS int@1
  ADDS
  IDIVS
  PUSHS GF@optionalValueA
  PUSHS int@0
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &35-6 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &35-7
LABEL &35-6
  PUSHS TF@temp1
LABEL &35-7
  PUSHS int@2
  MULS
  PUSHS GF@optionalValueB
  PUSHS int@1
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &35-8 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &35-9
LABEL &35-8
  PUSHS TF@temp1
LABEL &35-9
  PUSHS int@2
  IDIVS
  PUSHS GF@optionalValueC
  PUSHS int@2
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &35-10 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &35-11
LABEL &35-10
  PUSHS TF@temp1
LABEL &35-11
  PUSHS int@3
  ADDS
  MULS
  SUBS
  ADDS
  POPS GF@longComplexExpression2
  PUSHS string@\010
  PUSHS GF@longComplexExpression2
  PUSHS int@2
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@8
  POPS GF@optionalValueX
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@5
  POPS GF@optionalValueY
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS nil@nil
  POPS GF@optionalValueZ
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@optionalValueX
  PUSHS int@0
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &43-0 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &43-1
LABEL &43-0
  PUSHS TF@temp1
LABEL &43-1
  PUSHS int@2
  ADDS
  PUSHS GF@optionalValueY
  PUSHS int@1
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &43-2 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &43-3
LABEL &43-2
  PUSHS TF@temp1
LABEL &43-3
  PUSHS int@3
  SUBS
  MULS
  PUSHS GF@optionalValueZ
  PUSHS int@2
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &43-4 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &43-5
LABEL &43-4
  PUSHS TF@temp1
LABEL &43-5
  PUSHS int@1
  ADDS
  IDIVS
  PUSHS GF@optionalValueX
  PUSHS int@0
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &43-6 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &43-7
LABEL &43-6
  PUSHS TF@temp1
LABEL &43-7
  PUSHS int@3
  MULS
  PUSHS GF@optionalValueY
  PUSHS int@1
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &43-8 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &43-9
LABEL &43-8
  PUSHS TF@temp1
LABEL &43-9
  PUSHS int@2
  IDIVS
  PUSHS GF@optionalValueZ
  PUSHS int@2
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &43-10 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &43-11
LABEL &43-10
  PUSHS TF@temp1
LABEL &43-11
  PUSHS int@4
  ADDS
  MULS
  SUBS
  ADDS
  POPS GF@longExpression
  PUSHS string@\010
  PUSHS GF@longExpression
  PUSHS int@2
  CALL _write
  exit int@0


LABEL _readString
  CREATEFRAME
  PUSHFRAME
  DEFVAR LF@result
  READ LF@result string
  PUSHS LF@result
  POPFRAME
  RETURN

LABEL _readInt
  CREATEFRAME
  PUSHFRAME
  DEFVAR LF@result
  READ LF@result int
  PUSHS LF@result
  POPFRAME
  RETURN

LABEL _readDouble
  CREATEFRAME
  PUSHFRAME
  DEFVAR LF@result
  READ LF@result float
  PUSHS LF@result
  POPFRAME
  RETURN

LABEL _Int2Double
  CREATEFRAME
  DEFVAR TF@term1
  POPS TF@term1
  PUSHFRAME
  INT2FLOAT LF@term1 LF@term1
  PUSHS LF@term1
  POPFRAME
  RETURN

LABEL _Double2Int
  CREATEFRAME
  DEFVAR TF@term1
  POPS TF@term1
  PUSHFRAME
  FLOAT2INT LF@term1 LF@term1
  PUSHS LF@term1
  POPFRAME
  RETURN

LABEL _length
  CREATEFRAME
  DEFVAR TF@s1
  POPS TF@s1
  PUSHFRAME
  STRLEN LF@s1 LF@s1
  PUSHS LF@s1
  POPFRAME
  RETURN

LABEL _substring
  CREATEFRAME
  DEFVAR TF@j1
  POPS TF@j1
  DEFVAR TF@i1
  POPS TF@i1
  DEFVAR TF@s1
  POPS TF@s1
  PUSHFRAME
  DEFVAR LF@cond
  DEFVAR LF@length
  LT LF@cond LF@i1 int@0
  JUMPIFEQ _substring_return_nil LF@cond bool@true
  LT LF@cond LF@j1 int@0
  JUMPIFEQ _substring_return_nil LF@cond bool@true
  GT LF@cond LF@i1 LF@j1
  JUMPIFEQ _substring_return_nil LF@cond bool@true
  STRLEN LF@length LF@s1
  DEFVAR LF@cond2
  GT LF@cond LF@i1 LF@length
  EQ LF@cond2 LF@i1 LF@length
  OR LF@cond LF@cond LF@cond2
  JUMPIFEQ _string_return_nil LF@cond bool@true
  GT LF@cond LF@j1 LF@length
  JUMPIFEQ _string_return_nil LF@cond bool@true
  DEFVAR LF@substring
  MOVE LF@substring string@
  DEFVAR LF@string_index
  MOVE LF@string_index LF@i1
  DEFVAR LF@char
LABEL _substring%0
  JUMPIFEQ _substring%0end LF@string_index LF@j1
  GETCHAR LF@char LF@s1 LF@string_index
  CONCAT LF@substring LF@substring LF@char
  ADD LF@string_index LF@string_index int@1
  JUMP _substring%0
LABEL _substring%0end
  PUSHS LF@substring
  POPFRAME
  RETURN
LABEL _substring_return_nil
  PUSHS nil@nil
  POPFRAME
  RETURN

LABEL _ord
  CREATEFRAME
  DEFVAR TF@c1
  POPS TF@c1
  PUSHFRAME
  PUSHS LF@c1
  CALL _length
  PUSHS int@0
  JUMPIFEQS _ord?0t
  STRI2INT LF@c1 LF@c1 int@0
  PUSHS LF@c1
  JUMP _ord?0end
LABEL _ord?0t
  PUSHS int@0
LABEL _ord?0end
  POPFRAME
  RETURN

LABEL _chr
  CREATEFRAME
  DEFVAR TF@i1
  POPS TF@i1
  PUSHFRAME
  INT2CHAR LF@i1 LF@i1
  PUSHS LF@i1
  POPFRAME
  RETURN

LABEL _write
  CREATEFRAME
  PUSHFRAME
  DEFVAR LF@param_count
  POPS LF@param_count
  DEFVAR LF@counter
  MOVE LF@counter int@0
  DEFVAR LF@temp_var
LABEL _write%0
  JUMPIFEQ _write%0end LF@counter LF@param_count
  POPS LF@temp_var
  WRITE LF@temp_var
  ADD LF@counter LF@counter int@1
  JUMP _write%0
LABEL _write%0end
  POPFRAME
  RETURN
