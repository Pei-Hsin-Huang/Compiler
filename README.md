功能: 將程式作業一所切好的token進行分析、檢查文法錯誤，實作含指定statement的Compiler，依各指令放入相對應的Table中並轉換為中間碼，最後將中間碼及所對應的資訊寫成output 檔。
data structure:
line放每行切好的Token，存放每個Token的資訊(table, num)，以vector的方式儲存。

Table1-2為自己寫好的Table

Table1為指定符號:
;()=+-*/^',:

Table2為Reserved word:
AND,BOOLEAN,CALL,DIMENSION,ELSE,ENP,ENS,EQ.GE,GT,GTO,IF,INPUT,INTEGER,LABEL,LE,LT,NE,OR,OUTPUT,PROGRAM,REAL,SUBROUTINE,THEN,VARIABLE,ARRAY,CHARACTER

Table3放Integer，以ascii碼相加mod10為hash的方式放入

Table4放Real Number，以ascii碼相加mod10為hash的方式放入(包含小數點)

Table5放Indentifer，以ascii碼相加mod10為hash的方式放入

Table6放中間碼，以累加的方式放入

Table7放Call參數及陣列資訊，以累加的方式放入
