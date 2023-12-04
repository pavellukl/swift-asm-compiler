.IFJcode23

  CREATEFRAME
  PUSHFRAME
  DEFVAR GF@str
  DEFVAR GF@substr
  DEFVAR GF@length_
  DEFVAR GF@ord_
  DEFVAR GF@chr_
  DEFVAR GF@int2double
  DEFVAR GF@double2int
  DEFVAR GF@read_int
  DEFVAR GF@read_double
  DEFVAR GF@read_string
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@Hello\032world
  POPS GF@str
  PUSHS string@\010
  PUSHS string@\010
  PUSHS GF@str
  PUSHS int@3
  CALL _write
  PUSHS GF@str
  PUSHS int@0
  PUSHS int@5
  CALL _substring
  POPS GF@substr
  PUSHS string@\010
  PUSHS GF@substr
  PUSHS int@2
  CALL _write
  PUSHS GF@str
  CALL _length
  POPS GF@length_
  PUSHS string@\010
  PUSHS GF@length_
  PUSHS int@2
  CALL _write
  PUSHS GF@str
  CALL _ord
  POPS GF@ord_
  PUSHS string@\010
  PUSHS GF@ord_
  PUSHS int@2
  CALL _write
  PUSHS int@65
  CALL _chr
  POPS GF@chr_
  PUSHS string@\010
  PUSHS GF@chr_
  PUSHS int@2
  CALL _write
  PUSHS string@123\032->\032Int2Double\010
  PUSHS int@1
  CALL _write
  PUSHS int@123
  CALL _Int2Double
  POPS GF@int2double
  PUSHS string@\010
  PUSHS GF@int2double
  PUSHS int@2
  CALL _write
  PUSHS string@123.456\032->\032Double2Int\010
  PUSHS int@1
  CALL _write
  PUSHS float@0x1.edd2f1a9fbe77p+6
  CALL _Double2Int
  POPS GF@double2int
  PUSHS string@\010
  PUSHS GF@double2int
  PUSHS int@2
  CALL _write
  CALL _readInt
  POPS GF@read_int
  PUSHS string@\010
  PUSHS GF@read_int
  PUSHS int@2
  CALL _write
  CALL _readDouble
  POPS GF@read_double
  PUSHS string@\010
  PUSHS GF@read_double
  PUSHS int@2
  CALL _write
  CALL _readString
  POPS GF@read_string
  PUSHS string@\010
  PUSHS GF@read_string
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
