.IFJcode23

  CREATEFRAME
  PUSHFRAME
  DEFVAR GF@end
  DEFVAR LF@inp1
  DEFVAR LF@vysl4
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@0
  POPS GF@end
LABEL %0
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS GF@end
  PUSHS int@0
  EQS
  PUSHS bool@false
  JUMPIFEQS %0end
  PUSHS string@Zadejte\032cislo\032pro\032vypocet\032faktorialu:\032\010
  PUSHS int@1
  CALL _write
  CALL _readInt
  POPS LF@inp1
  PUSHS LF@inp1
  PUSHS LF@inp1
  PUSHS nil@nil
  JUMPIFEQS %0?0f
  DEFVAR LF@inp2
  POPS LF@inp2
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS LF@inp2
  PUSHS int@0
  LTS
  PUSHS bool@false
  JUMPIFEQS %0?0?1f
  PUSHS string@Faktorial\032nelze\032spocitat!\010
  PUSHS int@1
  CALL _write
  JUMP %0?0?1end
LABEL %0?0?1f
  PUSHS LF@inp2
  CALL _factorial
  POPS LF@vysl4
  PUSHS string@\010
  PUSHS LF@vysl4
  PUSHS string@Vysledek\032je:\032
  PUSHS int@3
  CALL _write
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@1
  POPS GF@end
LABEL %0?0?1end
  JUMP %0?0end
LABEL %0?0f
  PUSHS string@\010
  PUSHS string@Chyba\032pri\032nacitani\032celeho\032cisla!
  PUSHS int@2
  CALL _write
LABEL %0?0end
  JUMP %0
LABEL %0end
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

LABEL _decrement
  CREATEFRAME
  DEFVAR TF@m1
  POPS TF@m1
  DEFVAR TF@n1
  POPS TF@n1
  PUSHFRAME
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS LF@n1
  PUSHS LF@m1
  SUBS
  POPFRAME
  RETURN
  POPFRAME
  RETURN

LABEL _factorial
  CREATEFRAME
  DEFVAR TF@n1
  POPS TF@n1
  PUSHFRAME
  DEFVAR LF@result1
  DEFVAR LF@decremented_n3
  DEFVAR LF@temp_result3
  PUSHS nil@nil
  POPS LF@result1
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS LF@n1
  PUSHS int@2
  LTS
  PUSHS bool@false
  JUMPIFEQS _factorial?0f
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS int@1
  POPS LF@result1
  JUMP _factorial?0end
LABEL _factorial?0f
  PUSHS LF@n1
  PUSHS int@1
  CALL _decrement
  POPS LF@decremented_n3
  PUSHS LF@decremented_n3
  CALL _factorial
  POPS LF@temp_result3
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS LF@n1
  PUSHS LF@temp_result3
  MULS
  POPS LF@result1
LABEL _factorial?0end
  CREATEFRAME
  DEFVAR TF@temp0
  DEFVAR TF@temp1
  PUSHS LF@result1
  POPFRAME
  RETURN
  POPFRAME
  RETURN
