all: piCalc extractDigits

create_build_dir:
	mkdir -p ./build

piCalc: create_build_dir
	gcc -o ./build/calcPi calcPi.c -lgmp -lm

extractDigits: create_build_dir
	gcc -o ./build/extractDigits extractDigits.c -lgmp 
