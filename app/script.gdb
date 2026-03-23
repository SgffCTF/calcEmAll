target remote :1234
add-symbol-file ./calc.ko -s .text 0xffffffffc0367000
