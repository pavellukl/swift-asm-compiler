.IFJcode23

  CREATEFRAME
  PUSHFRAME
  DEFVAR GF@y
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@10
  POPS GF@y
  PUSHS int@7
  CALL _foo
  PUSHS GF@y
  PUSHS int@1
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

LABEL _foo
  CREATEFRAME
  DEFVAR TF@x1
  POPS TF@x1
  PUSHFRAME
  DEFVAR LF@y1
  DEFVAR LF@y2
  DEFVAR LF@i1
  DEFVAR LF@x3
  DEFVAR LF@inp4
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@hello
  PUSHS string@\010
  POPS TF@temp1
  POPS TF@temp0
  CONCAT TF@temp0 TF@temp0 TF@temp1
  PUSHS TF@temp0
  POPS LF@y1
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS LF@y1
  PUSHS string@hello\010
  EQS
  PUSHS bool@false
  JUMPIFEQS _foo?0f
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@42
  POPS LF@y2
  PUSHS LF@y2
  PUSHS int@1
  CALL _write
  JUMP _foo?0end
LABEL _foo?0f
  PUSHS LF@y1
  PUSHS int@1
  CALL _write
LABEL _foo?0end
  PUSHS LF@y1
  PUSHS int@1
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@0
  POPS LF@i1
LABEL _foo%0
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS LF@i1
  PUSHS int@10
  GTS
  NOTS
  PUSHS bool@false
  JUMPIFEQS _foo%0end
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS LF@i1
  PUSHS int@1
  ADDS
  POPS LF@i1
  CALL _incrGlobal
  PUSHS LF@i1
  PUSHS int@1
  CALL _write
  PUSHS string@Insert\032non-empty\032input\032for\032quiting,\032otherwise\032next\032iteration.\010
  PUSHS int@1
  CALL _write
  CALL _readString
  POPS LF@x3
  PUSHS LF@x3
  PUSHS LF@x3
  PUSHS nil@nil
  JUMPIFEQS _foo%0?1f
  DEFVAR LF@x4
  POPS LF@x4
  PUSHS LF@x4
  CALL _length
  POPS LF@inp4
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS LF@inp4
  PUSHS int@0
  EQS
  NOTS
  PUSHS bool@false
  JUMPIFEQS _foo%0?1?2f
  PUSHS string@\010
  PUSHS LF@x4
  PUSHS int@2
  CALL _write
  POPFRAME
  RETURN
  JUMP _foo%0?1?2end
LABEL _foo%0?1?2f
LABEL _foo%0?1?2end
  JUMP _foo%0?1end
LABEL _foo%0?1f
LABEL _foo%0?1end
  JUMP _foo%0
LABEL _foo%0end
  POPFRAME
  RETURN

LABEL _incrGlobal
  CREATEFRAME
  PUSHFRAME
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@y
  PUSHS int@1
  ADDS
  POPS GF@y
  POPFRAME
  RETURN
