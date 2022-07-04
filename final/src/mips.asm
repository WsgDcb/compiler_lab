.mips
addi $8, $0, 3
mul $8, $8, 2
sw $8, 8216
sw $8, 8196
addi $8, $8, 2
sw $8, 8220
sw $8, 8200
lw $9, 8196
add $8, $8, $9
sw $8, 8224
div $8, $8, 2
sw $8, 8228
sw $9, 8196
lw $10, 8200
sub $9, $9, $10
bgez $9,false0
addi $9, $0, 1
j true0
false0:
addi $9, $0, 0
true0:
sw $8, 8192
subi $8, $8, 5
blez $8,false1
addi $8, $0, 1
j true1
false1:
addi $8, $0, 0
true1:
sb $9, 8232
and $9, $9, $8
beq $9,$0,FJ0
lw $11, 8192
not $11, $11
addi $11, $11, 1
sw $11, 8244
sw $11, 8204
not $11, $11
addi $11, $11, 1
sw $11, 8248
subi $11, $11, 0
blez $11,false2
addi $11, $0, 1
j true2
false2:
addi $11, $0, 0
true2:
beq $11,$0,FJ1
addi $12, $0, 10
FJ1:
j ie0
FJ0:
lw $13, 8192
lw $14, 8196
sw $13, 8192
sub $13, $13, $14
sw $13, 8256
ie0:
sb $8, 8236
sb $9, 8240
sw $10, 8200
sb $11, 8252
sw $12, 8212
sw $13, 8208
sw $14, 8196
