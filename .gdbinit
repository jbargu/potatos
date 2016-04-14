target remote localhost:1234
set disassembly-flavor intel
symbol-file potatos.bin
break kernel_main
c
