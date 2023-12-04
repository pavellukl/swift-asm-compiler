.IFJcode23

  CREATEFRAME
  PUSHFRAME
  DEFVAR GF@a
  DEFVAR GF@b
  DEFVAR GF@c
  DEFVAR GF@d
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@b
  POPS GF@a
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@a
  PUSHS string@Was\032Nil
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &3-0 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &3-1
LABEL &3-0
  PUSHS TF@temp1
LABEL &3-1
  POPS GF@b
  PUSHS string@\010
  PUSHS GF@a
  PUSHS int@2
  CALL _write
  PUSHS string@\010
  PUSHS GF@b
  PUSHS int@2
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS nil@nil
  POPS GF@c
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@c
  PUSHS string@Was\032Nil
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &7-0 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &7-1
LABEL &7-0
  PUSHS TF@temp1
LABEL &7-1
  POPS GF@d
  PUSHS string@\010
  PUSHS GF@c
  PUSHS int@2
  CALL _write
  PUSHS string@\010
  PUSHS GF@d
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
