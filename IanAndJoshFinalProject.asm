0; top-left 
0; top-mid 
0; top-right 
0; mid-left 
0; mid-mid
0; mid-right
0; bottom-left
0; bottom-mid
0; bottom-right
0; row 1 weight
0; row 2 weight
0; row 3 weight
0; colum 1 weight
0; colum 2 weight
0; colum 3 weight
0; diagonal 1
0; diagonal 2
0; will change to nine when the game has been won


get
cmp ax 0
je [686]
jmp [415]


;PRINT BOARD[30]
mov ax 32
putc
mov ax [0]
putc
mov ax 32
putc
mov ax 124
putc
mov ax 32
putc
mov ax [1]
putc
mov ax 32
putc
mov ax 124
putc
mov ax 32
putc
mov ax [2]
putc
mov ax 32
putc
mov ax 9
putc
mov ax 48
putc
mov ax 32
putc
mov ax 49
putc
mov ax 32
putc
mov ax 50
putc
mov ax 32
putc
mov ax 10
putc
mov ax 45
putc
putc
putc
putc
putc
putc
putc
putc
putc
putc
putc
mov ax 10
putc
mov ax 32
putc
mov ax [3]
putc
mov ax 32
putc
mov ax 124
putc
mov ax 32
putc
mov ax [4]
putc
mov ax 32
putc
mov ax 124
putc
mov ax 32
putc
mov ax [5]
putc
mov ax 32
putc
mov ax 32
putc
mov ax 9
putc
mov ax 51
putc
mov ax 32
putc
mov ax 52
putc
mov ax 32
putc
mov ax 53
putc
mov ax 10
putc
mov ax 45
putc
putc
putc
putc
putc
putc
putc
putc
putc
putc
putc
mov ax 10
putc
mov ax 32
putc
mov ax [6]
putc
mov ax 32
putc
mov ax 124
putc
mov ax 32
putc
mov ax [7]
putc
mov ax 32
putc
mov ax 124
putc
mov ax 32
putc
mov ax [8]
putc
mov ax 32
putc
mov ax 32
putc
mov ax 9
putc
mov ax 54
putc
mov ax 32
putc
mov ax 55
putc
mov ax 32
putc
mov ax 56
putc
mov ax 10
putc
ret [235]




; get player move[240] 
get
cmp ax 0
jb [240]
cmp ax 8
ja [240]
mov bx ax
mov ax [bx]
cmp ax 0
jne [240]
mov cx [239]
mov [bx] cx
ret [260]




; calculate row weights [265] 
mov ax 0; checking row 1
mov bx 0; top-left
add ax [bx]
add ax [bx+1]
add ax [bx+2]
mov [10] ax; weights-array-1

mov ax 0; checking row 2
mov bx 3; middle-left
add ax [bx]
add ax [bx+1]
add ax [bx+2]
mov [11] ax; weights-array-2

mov ax 0; checking row 3
mov bx 6; bottom-left
add ax [bx]
add ax [bx+1]
add ax [bx+2]
mov [12] ax; weights-array-3

mov ax 0; checking column 1
mov bx 0; top-left
add ax [bx]
add ax [bx+3]
add ax [bx+6]
mov [13] ax; weights-array-4

mov ax 0; checking column 2
mov bx 1; top-middle
add ax [bx]
add ax [bx+3]
add ax [bx+6]
mov [14] ax; weights-array-5

mov ax 0; checking column 3
mov bx 2; top-right
add ax [bx]
add ax [bx+3]
add ax [bx+6]
mov [15] ax; weights-array-6

mov ax 0; checking diagnal 1
mov bx 0; top-left
add ax [bx]
add ax [bx+4]
add ax [bx+8]
mov [16] ax; weights-array-7

mov ax 0; checking diagnal 2
mov bx 2; top-right
add ax [bx]
add ax [bx+2]
add ax [bx+4]
mov [17] ax; weights-array-8x
ret [357]


;check if game over[365]
mov cx [364]
cmp cx [10]
je [401]
cmp cx [11]
je [401]
cmp cx [12]
je [401]
cmp cx [13]
je [401]
cmp cx [14]
je [401]
cmp cx [15]
je [401]
cmp cx [16]
je [401]
cmp cx [17]
je [401]
jmp [405]
mov ax 9
mov [18] ax
ret [407]








fun [30] 0
fun [240] 1
88
fun [265] 0
fun [365] 1
264
mov ax [18]
cmp ax 9
je [474]
add bx 1
cmp bx 9
je [474]

fun [30] 0
fun [240] 1
79
fun [265] 0
fun [365] 1
237
mov ax [18]
cmp ax 9
je [474]
add bx 1
cmp bx 9
je [474]
jmp [415]
fun [30] 0
halt




;[483]
mov bx 10; weights-array-1 --=find winning move=--
mov dx [bx]
cmp dx 158
je [540]; jmp to find open space in row
add bx 1
cmp bx 17
jbe [485]; jump to -find winning move-

mov bx 10; weights-array-1 --=sabatoge start=--
mov dx [bx]
cmp dx 176
je [540]; jmp to find open space in row
add bx 1
cmp bx 17
jbe [499]; jump to -sabatoge start-

mov bx 0; top-left --=find empty corner/middle=--
mov dx [bx]
cmp dx 0
je [671]; jmp to place X
add bx 2; moving to corner/middle
cmp bx 17; bottom-right
jbe [513]; jump to -find empty corner/middle-
1111
mov bx 0; top-left --=find empty space=--
mov dx [bx]
cmp dx 0
je [671]; jump to place X
add bx 1; parse through all board
cmp bx 17; bottom-right
jbe [527]; jump to find empty space +1

;----find open space in row------
cmp bx 10
jne [557]; cmp bx 11
mov bx 0
mov ax [bx]; HERE
cmp ax 0
je [671]; place x
add bx 1
jmp [546]; HERE


cmp bx 11
jne [574]; cmp bx 12
mov bx 3
mov ax [bx]; HERE
cmp ax 0
je [671]; place x
add bx 1
jmp [563]; HERE


cmp bx 12
jne [591]; cmp bx 13
mov bx 6
mov ax [bx]; HERE
cmp ax 0
je [671]; place x
add bx 1
jmp [580]; HERE


cmp bx 13
jne [608]; cmp bx 14
mov bx 0
mov ax [bx]; HERE
cmp ax 0
je [671]; place x
add bx 3
jmp [597]; HERE


cmp bx 14
jne [625]; cmp bx 15
mov bx 1
mov ax [bx]; HERE
cmp ax 0
je [671]; place x
add bx 3
jmp [614]; HERE


cmp bx 15
jne [642]; cmp bx 16
mov bx 2
mov ax [bx]; HERE
cmp ax 0
je [671]; place x
add bx 3
jmp [631]; HERE


cmp bx 16
jne [659]; cmp bx 17
mov bx 0
mov ax [bx]; HERE
cmp ax 0
je [671]; place x
add bx 4
jmp [648]; HERE


mov bx 2
mov ax [bx]; here
cmp ax 0
je [671]; place x 
add bx 2
jmp [661]; here

mov ax 79; moves ascii O  --=place O=--
mov [bx] ax; place 0 in 
ret [676]










fun [30] 0
fun [240] 1
88
fun [265] 0
fun [365] 1
264
mov ax [18]
cmp ax 9
je [474]
add bx 1
cmp bx 9
je [474]


fun [483] 0
fun [265] 0
fun [365] 1
237
mov ax [18]
cmp ax 9
je [474]
add bx 1
cmp bx 9
je [474]

jmp [686]




