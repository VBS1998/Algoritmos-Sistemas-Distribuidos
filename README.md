# Segunda Avaliação MC714 - Sistemas Distribuídos

## Resumo

Esse projeto, realizado para a disciplina de Sistemas Distribuídos da UNICAMP, visa implementar alguns algortimos com o uso de máquinas na AWS. 

Os algoritmos a implementados foram:

* Relógio Lógico de Lamport.
* Exclusão Mútua.
* Algoritmo de Bully para Eleição de Líder.

Os algoritmos foram implementados utilizando mensagens MPI com o Open MPI.

Para entender o uso do Open MPI, utilizamos este [turorial](https://www.codingame.com/playgrounds/349/introduction-to-mpi/introduction-to-distributed-computing), junto da [documentação do Open MPI](https://www.open-mpi.org/doc/v4.1/).

## Requisitos

Para executar o projeto é necessária a criação de ao menos três máquinas com Ubuntu (no caso, foram criadas três instâncias no AWS EC2).

Depois, é necessária a instalação do Open MPI em todas as máquinas, e configuras as chaves públicas SSH para acesso delas.

Para a configuração do SSH e do Open MPI nas máquinas AWS, foi utilizado este [vídeo](https://www.youtube.com/watch?v=Nh_MKvur-QA&ab_channel=MatheusGois).

## Compilando e Executando

Uma vez configuradas as máquinas, já é possível a compilação e execução do programa.

Reforçando que, para o makefile funcionar, é necessária a nomeação das duas slaves como `slave1` e `slave2` em `/etc/hosts/`.

Primeiramente é preciso criar uma pasta chamada `Trabalho-02` na home de cada uma das máquinas.

Agora, a pasta do projeto deve ser copiada para a maquina `master` com o comando:
```sh
scp -r <Caminho_Para_o_Projeto>/Trabalho-02 <user-name>@<ip-da-máquina>:~
```

Por fim, deve-se acessar a máquina `master` por SSH, navegar até a pasta do projeto e executar o comando:
```sh
make run
```
que compila o programa com:
```sh
mpiCC src/*.cpp -o exec
```	

depois envia às outras máquinas com:
```sh
scp exec ubuntu@slave1:~/Trabalho-02/
```

e por fim, executa o programa com:
```sh
mpirun -host localhost,slave1,slave2 ./exec
```

Quando executando o programa, é necessário que coloque algum caracter qualquer no entrada padrão entre a execução de cada um dos algoritmos para que o próximo comece.

## Referências
https://www.open-mpi.org/doc/v4.1/ (Documentação Open MPI)

https://www.codingame.com/playgrounds/349/introduction-to-mpi/introduction-to-distributed-computing (Turorial Open MPI)

https://www.youtube.com/watch?v=Nh_MKvur-QA&ab_channel=MatheusGois (Configuração MPI e SSH no AWS)

https://cvw.cac.cornell.edu/mpip2p/waittestfree (Usado e adaptado trecho de código referente ao `MPI_Test()`)

https://pt.wikipedia.org/wiki/Elei%C3%A7%C3%A3o_de_l%C3%ADder (Algoritmo de Bully)

