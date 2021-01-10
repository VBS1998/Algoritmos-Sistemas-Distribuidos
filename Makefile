all: build send

build: 
	mpiCC src/*.cpp -o exec

send:
	scp exec ubuntu@slave1:/home/ubuntu/Trabalho-02/
	scp exec ubuntu@slave2:/home/ubuntu/Trabalho-02/

run: all
	mpirun -host localhost,slave1,slave2 ./exec

clear:
	rm ./exec