.IFJcode23

  CREATEFRAME
  PUSHFRAME
  DEFVAR GF@str1
  DEFVAR GF@str2
  DEFVAR GF@i
  DEFVAR GF@newInput
  DEFVAR LF@tmp2
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@Toto\032je\032nejaky\032text\032v\032programu\032jazyka\032IFJ23
  POPS GF@str1
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@str1
  PUSHS string@,\032ktery\032jeste\032trochu\032obohatime
  POPS TF@temp1
  POPS TF@temp0
  CONCAT TF@temp0 TF@temp0 TF@temp1
  PUSHS TF@temp0
  POPS GF@str2
  PUSHS string@\010
  PUSHS GF@str2
  PUSHS string@\010
  PUSHS GF@str1
  PUSHS int@4
  CALL _write
  PUSHS GF@str1
  CALL _length
  POPS GF@i
  PUSHS string@\010
  PUSHS GF@i
  PUSHS string@Pozice\032retezce\032"text"\032v\032str2:\032
  PUSHS int@3
  CALL _write
  PUSHS string@Zadejte\032serazenou\032posloupnost\032vsech\032malych\032pismen\032a-h,\032
  PUSHS int@1
  CALL _write
  CALL _readString
  POPS GF@newInput
  PUSHS GF@newInput
  PUSHS GF@newInput
  PUSHS nil@nil
  JUMPIFEQS ?0f
  DEFVAR LF@newInput1
  POPS LF@newInput1
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS LF@newInput1
  POPS GF@str1
LABEL ?0%0
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@str1
  PUSHS string@abcdefgh
  EQS
  NOTS
  PUSHS bool@false
  JUMPIFEQS ?0%0end
  PUSHS string@Spatne\032zadana\032posloupnost,\032zkuste\032znovu:\010
  PUSHS int@1
  CALL _write
  CALL _readString
  POPS LF@tmp2
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS LF@tmp2
  PUSHS string@
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ ?0%0&9-0 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP ?0%0&9-1
LABEL ?0%0&9-0
  PUSHS TF@temp1
LABEL ?0%0&9-1
  POPS GF@str1
  JUMP ?0%0
LABEL ?0%0end
  JUMP ?0end
LABEL ?0f
LABEL ?0end
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
