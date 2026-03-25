obj-m += calc.o
calc-objs += src/calc.c src/calculator/calculator.o

PWD := $(CURDIR)

all:
	make -C ../linux M=$(PWD) modules

clean:
	make -C ../linux M=$(PWD) clean
