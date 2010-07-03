
nasm -f bin stage1.asm -o stage1.bin
partcopy stage1.bin 0 200 -f1

nasm -f bin stage2.asm -o stage2.sys
copy stage2.sys b:\


pause
