.IFJcode23

  CREATEFRAME
  PUSHFRAME
  DEFVAR GF@a
  DEFVAR GF@b
  DEFVAR GF@c
  DEFVAR GF@d
  DEFVAR GF@e
  DEFVAR GF@f
  DEFVAR GF@read_str
  DEFVAR GF@read_int
  DEFVAR GF@read_double
  DEFVAR GF@a_double
  DEFVAR GF@an_int
  DEFVAR GF@len
  DEFVAR GF@substr
  DEFVAR GF@ord_res
  DEFVAR GF@chr_res
  DEFVAR GF@infer1
  DEFVAR GF@str_nillable
  DEFVAR GF@a_string
  DEFVAR LF@a_string4
  DEFVAR GF@nil_var
  DEFVAR GF@nil_var_2
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@hello
  POPS GF@a
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@hello
  POPS GF@c
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@hello
  POPS GF@d
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@hello
  POPS GF@f
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@1
  PUSHS int@1
  EQS
  PUSHS bool@false
  JUMPIFEQS ?0f
  PUSHS GF@a
  PUSHS int@1
  CALL _write
  JUMP ?0end
LABEL ?0f
  PUSHS GF@a
  PUSHS int@1
  CALL _write
LABEL ?0end
LABEL %0
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@1
  PUSHS int@1
  EQS
  PUSHS bool@false
  JUMPIFEQS %0end
  PUSHS GF@a
  PUSHS int@1
  CALL _write
  JUMP %0
LABEL %0end
  CALL _fn
  PUSHS GF@a
  CALL _fn2
  PUSHS string@lit
  CALL _fn2
  PUSHS GF@a
  PUSHS GF@d
  CALL _fn3
  PUSHS string@lit
  PUSHS string@lit
  CALL _fn3
  PUSHS GF@a
  PUSHS GF@d
  CALL _fn4
  PUSHS string@lit
  PUSHS string@lit
  CALL _fn4
  PUSHS float@0x1.ec7df3b645a1dp+6
  PUSHS string@abc
  PUSHS int@123
  PUSHS int@3
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@hello
  POPS GF@a
  CALL _readString
  POPS GF@read_str
  CALL _readInt
  POPS GF@read_int
  CALL _readDouble
  POPS GF@read_double
  PUSHS int@123
  CALL _Int2Double
  POPS GF@a_double
  PUSHS float@0x1.ec7df3b645a1dp+6
  CALL _Double2Int
  POPS GF@an_int
  PUSHS GF@a
  CALL _length
  POPS GF@len
  PUSHS GF@a
  PUSHS int@0
  PUSHS int@1
  CALL _substring
  POPS GF@substr
  PUSHS GF@a
  CALL _ord
  POPS GF@ord_res
  PUSHS int@123
  CALL _chr
  POPS GF@chr_res
  PUSHS GF@a
  CALL _fn2
  POPS GF@infer1
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@test
  POPS GF@str_nillable
  PUSHS GF@str_nillable
  PUSHS GF@str_nillable
  PUSHS nil@nil
  JUMPIFEQS ?1f
  DEFVAR LF@str_nillable3
  POPS LF@str_nillable3
  PUSHS LF@str_nillable3
  CALL _arg_str
  JUMP ?1end
LABEL ?1f
LABEL ?1end
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@test
  POPS GF@a_string
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@1
  PUSHS int@1
  EQS
  PUSHS bool@false
  JUMPIFEQS ?2f
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@123
  POPS LF@a_string4
  PUSHS LF@a_string4
  CALL _accept_int
  PUSHS LF@a_string4
  PUSHS int@1
  CALL _write
  JUMP ?2end
LABEL ?2f
LABEL ?2end
  PUSHS nil@nil
  CALL _nil_arg
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS nil@nil
  POPS GF@nil_var
  PUSHS nil@nil
  POPS GF@nil_var_2
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS nil@nil
  POPS GF@nil_var_2
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

LABEL _fn
  CREATEFRAME
  PUSHFRAME
  POPFRAME
  RETURN

LABEL _fn2
  CREATEFRAME
  DEFVAR TF@a1
  POPS TF@a1
  PUSHFRAME
  PUSHS string@a
  PUSHS string@b
  CALL _fn3
  PUSHS LF@a1
  CALL _fn2
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS LF@a1
  POPFRAME
  RETURN
  POPFRAME
  RETURN

LABEL _fn3
  CREATEFRAME
  DEFVAR TF@b1
  POPS TF@b1
  DEFVAR TF@a1
  POPS TF@a1
  PUSHFRAME
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS LF@a1
  POPFRAME
  RETURN
  POPFRAME
  RETURN

LABEL _fn4
  CREATEFRAME
  DEFVAR TF@b1
  POPS TF@b1
  DEFVAR TF@a1
  POPS TF@a1
  PUSHFRAME
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS LF@a1
  POPFRAME
  RETURN
  POPFRAME
  RETURN

LABEL _arg_str
  CREATEFRAME
  DEFVAR TF@a1
  POPS TF@a1
  PUSHFRAME
  POPFRAME
  RETURN

LABEL _accept_int
  CREATEFRAME
  DEFVAR TF@a1
  POPS TF@a1
  PUSHFRAME
  POPFRAME
  RETURN

LABEL _redef_var
  CREATEFRAME
  PUSHFRAME
  DEFVAR LF@a_string1
  DEFVAR LF@a_string2
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@test
  POPS LF@a_string1
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@1
  PUSHS int@1
  EQS
  PUSHS bool@false
  JUMPIFEQS _redef_var?0f
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@123
  POPS LF@a_string2
  PUSHS LF@a_string2
  CALL _accept_int
  PUSHS LF@a_string2
  PUSHS int@1
  CALL _write
  JUMP _redef_var?0end
LABEL _redef_var?0f
LABEL _redef_var?0end
  PUSHS LF@a_string1
  PUSHS int@1
  CALL _write
  POPFRAME
  RETURN

LABEL _nil_arg
  CREATEFRAME
  DEFVAR TF@a1
  POPS TF@a1
  PUSHFRAME
  POPFRAME
  RETURN
