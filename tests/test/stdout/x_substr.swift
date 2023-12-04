.IFJcode23

  CREATEFRAME
  PUSHFRAME
  DEFVAR GF@s1
  DEFVAR GF@s2
  DEFVAR GF@s1String
  DEFVAR GF@s1len
  DEFVAR GF@s1_20ty_znak
  DEFVAR GF@s1_16ty_znak
  DEFVAR GF@s11
  DEFVAR GF@s3
  DEFVAR GF@s1len4
  PUSHS string@\010
  PUSHS string@Ahoj\010"Sve'te\032\092"
  PUSHS int@2
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS string@\032\032Toto\032\010\032\032je\032\010\010\032\032nejaky\032\010\032\032text
  POPS GF@s1
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@s1
  PUSHS string@,\032ktery\032jeste\032trochu\032obohatime
  POPS TF@temp1
  POPS TF@temp0
  CONCAT TF@temp0 TF@temp0 TF@temp1
  PUSHS TF@temp0
  POPS GF@s2
  PUSHS string@\010
  PUSHS GF@s2
  PUSHS string@\032
  PUSHS GF@s1
  PUSHS int@4
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@s1
  POPS GF@s1String
  PUSHS GF@s1String
  CALL _length
  POPS GF@s1len
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@s1len
  POPS GF@s1_20ty_znak
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@4
  POPS GF@s1len
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@s1_20ty_znak
  PUSHS GF@s1len
  SUBS
  POPS GF@s1_16ty_znak
  PUSHS GF@s2
  PUSHS GF@s1_16ty_znak
  PUSHS GF@s1_20ty_znak
  CALL _substring
  POPS GF@s11
  PUSHS string@\010
  PUSHS GF@s11
  PUSHS string@":
  PUSHS GF@s2
  PUSHS string@)\032v\032"
  PUSHS GF@s1_16ty_znak
  PUSHS string@.\032znaku\032(index\032
  PUSHS int@16
  PUSHS string@\032znaky\032od\032
  PUSHS GF@s1len
  PUSHS int@10
  CALL _write
  PUSHS GF@s2
  PUSHS int@15
  PUSHS int@19
  CALL _substring
  POPS GF@s3
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@s3
  PUSHS string@viceradkovy\010"retezec"\010misto\032nil\032vysledku
  POPS TF@temp1
  POPS TF@temp0
  JUMPIFEQ &13-0 TF@temp0 nil@nil
  PUSHS TF@temp0
  JUMP &13-1
LABEL &13-0
  PUSHS TF@temp1
LABEL &13-1
  POPS GF@s1String
  PUSHS GF@s1String
  CALL _length
  POPS GF@s1len4
  PUSHS string@\010
  PUSHS GF@s1
  PUSHS string@":
  PUSHS GF@s2
  PUSHS string@.\032znaku\032v\032"
  PUSHS int@16
  PUSHS string@\032znaky\032od\032
  PUSHS GF@s1len4
  PUSHS int@8
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
