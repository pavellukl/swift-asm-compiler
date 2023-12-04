.IFJcode23

  CREATEFRAME
  PUSHFRAME
  DEFVAR GF@int_2_double
  DEFVAR GF@double_2_int
  DEFVAR GF@empty_string
  DEFVAR GF@len_empty
  DEFVAR GF@a_string
  DEFVAR GF@len
  DEFVAR GF@hello
  DEFVAR GF@substr
  DEFVAR GF@substr_world
  DEFVAR GF@neg_one
  DEFVAR GF@substr_neg
  DEFVAR GF@substr_neg_2
  DEFVAR GF@substr_neg_3
  DEFVAR GF@substr_neg_4
  DEFVAR GF@substr_neg_5
  DEFVAR GF@substr_one
  DEFVAR GF@substr_empty
  DEFVAR GF@substr_empty_no_nil
  DEFVAR GF@empty_len
  DEFVAR GF@ord_res
  DEFVAR GF@chr_res
  PUSHS string@\010
  PUSHS float@0x1.ecp+6
  PUSHS string@\010
  PUSHS int@123
  PUSHS string@\010
  PUSHS string@\092
  PUSHS string@\032\035
  PUSHS string@ok
  PUSHS int@8
  CALL _write
  PUSHS int@123
  CALL _Int2Double
  POPS GF@int_2_double
  PUSHS string@\010
  PUSHS GF@int_2_double
  PUSHS int@2
  CALL _write
  PUSHS float@0x1.ecp+6
  CALL _Double2Int
  POPS GF@double_2_int
  PUSHS string@\010
  PUSHS GF@double_2_int
  PUSHS int@2
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@
  POPS GF@empty_string
  PUSHS GF@empty_string
  CALL _length
  POPS GF@len_empty
  PUSHS string@\010
  PUSHS GF@len_empty
  PUSHS int@2
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@te\010st
  POPS GF@a_string
  PUSHS GF@a_string
  CALL _length
  POPS GF@len
  PUSHS string@\010
  PUSHS GF@len
  PUSHS int@2
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@Hello\032World
  POPS GF@hello
  PUSHS GF@hello
  PUSHS int@0
  PUSHS int@5
  CALL _substring
  POPS GF@substr
  PUSHS string@\010
  PUSHS GF@substr
  PUSHS int@2
  CALL _write
  PUSHS GF@hello
  PUSHS int@6
  PUSHS int@11
  CALL _substring
  POPS GF@substr_world
  PUSHS string@\010
  PUSHS GF@substr_world
  PUSHS int@2
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@0
  PUSHS int@1
  SUBS
  POPS GF@neg_one
  PUSHS GF@hello
  PUSHS GF@neg_one
  PUSHS int@11
  CALL _substring
  POPS GF@substr_neg
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@substr_neg
  PUSHS string@nil_
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &9-0 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &9-1
LABEL &9-0
  PUSHS TF@temp1
LABEL &9-1
  POPS GF@substr_neg
  PUSHS string@\010
  PUSHS GF@substr_neg
  PUSHS int@2
  CALL _write
  PUSHS GF@hello
  PUSHS int@0
  PUSHS GF@neg_one
  CALL _substring
  POPS GF@substr_neg_2
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@substr_neg_2
  PUSHS string@nil_
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &11-0 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &11-1
LABEL &11-0
  PUSHS TF@temp1
LABEL &11-1
  POPS GF@substr_neg_2
  PUSHS string@\010
  PUSHS GF@substr_neg_2
  PUSHS int@2
  CALL _write
  PUSHS GF@hello
  PUSHS int@5
  PUSHS int@0
  CALL _substring
  POPS GF@substr_neg_3
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@substr_neg_3
  PUSHS string@nil_
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &13-0 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &13-1
LABEL &13-0
  PUSHS TF@temp1
LABEL &13-1
  POPS GF@substr_neg_3
  PUSHS string@\010
  PUSHS GF@substr_neg_3
  PUSHS int@2
  CALL _write
  PUSHS GF@hello
  PUSHS int@11
  PUSHS int@11
  CALL _substring
  POPS GF@substr_neg_4
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@substr_neg_4
  PUSHS string@nil_
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &15-0 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &15-1
LABEL &15-0
  PUSHS TF@temp1
LABEL &15-1
  POPS GF@substr_neg_4
  PUSHS string@\010
  PUSHS GF@substr_neg_4
  PUSHS int@2
  CALL _write
  PUSHS GF@hello
  PUSHS int@0
  PUSHS int@12
  CALL _substring
  POPS GF@substr_neg_5
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@substr_neg_5
  PUSHS string@nil_
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &17-0 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &17-1
LABEL &17-0
  PUSHS TF@temp1
LABEL &17-1
  POPS GF@substr_neg_5
  PUSHS string@\010
  PUSHS GF@substr_neg_5
  PUSHS int@2
  CALL _write
  PUSHS GF@hello
  PUSHS int@0
  PUSHS int@1
  CALL _substring
  POPS GF@substr_one
  PUSHS string@\010
  PUSHS GF@substr_one
  PUSHS int@2
  CALL _write
  PUSHS GF@hello
  PUSHS int@0
  PUSHS int@0
  CALL _substring
  POPS GF@substr_empty
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@substr_empty
  POPS GF@substr_empty_no_nil
  PUSHS GF@substr_empty_no_nil
  CALL _length
  POPS GF@empty_len
  PUSHS string@\010
  PUSHS GF@empty_len
  PUSHS int@2
  CALL _write
  PUSHS string@H
  CALL _ord
  POPS GF@ord_res
  PUSHS string@\010
  PUSHS GF@ord_res
  PUSHS int@2
  CALL _write
  PUSHS int@72
  CALL _chr
  POPS GF@chr_res
  PUSHS string@\010
  PUSHS GF@chr_res
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
