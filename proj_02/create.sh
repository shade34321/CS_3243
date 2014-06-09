#!/bin/bash

nasm -f bin -o Tinney_Alabsa.bin Tinney_Alabsa.asm
dd status=noxfer conv=notrunc if=Tinney_Alabsa.bin of=Tinney_Alabsa.flp
qemu -fda Tinney_Alabsa.flp
