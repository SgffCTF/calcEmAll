savedcmd_calc.mod := printf '%s\n'   src/calc.c src/calculator/calculator.o | awk '!x[$$0]++ { print("./"$$0) }' > calc.mod
