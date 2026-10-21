# CÓDIGOS DISPONÍVEIS
Uma breve descrição de cada programa e como rodá-los.<br>(códigos compatíveis apenas com sistemas UNIX)
## sockcreate.c
### DESCRIÇÃO
  Ao executar o programa no terminal, passando como argumento uma porta de rede _(ex: 8080)_, o sistema busca por endereços compatíveis com os critérios especificados em _hints_ e com a porta informada. Em seguida, cria uma socket (_L. 58_) e tenta vincular ela a um dos endereços encontrados (_L. 64_).  

  Caso tenha sido possível vincular a socket a um endereço, imprime o _File Descriptor_ , o protocolo (IPv4 ou IPv6) e o endereço da socket.    
### USO
_Compilar:_  
`gcc sockcreate.c -o sockcreate`  
_Executar:_  
`./sockcreate port`  

## server.c
### DESCRIÇÃO
  Similar ao código anterior, o _server.c_ cria uma socket, vincula um endereço e retorna a porta e o endereço IP da socket. Caso esse processo tenha tido êxito, a socket entra em _listen_ e o sistema passa a escutar por conexões.  

Ao identificar uma conexão, o cliente recebe a mensagem "Olá! Por favor, informe seu nome", ao receber uma resposta do cliente, o programa retorna "Seja bem-vindo(a) {resposta}".  

O programa também permite conexões simultâneas.  

Para gerenciar múltiplas conexões, o sistema usa a chamada _fork()_, onde cada processo filho utiliza as funções _send()_ e _recv()_ para fazer a troca de dados com o cliente.
### USO
_Compilar:_  
`gcc server.c -o server`  
_Executar:_  
`./server`  
_Comunicação:_  
Enquanto o sistema estiver em execução, você pode utilizar o _NetCat_ (ou qualquer software similar), usando como parâmetros o IP e a porta retornados pelo próprio server.c.  
Ex:  
`nc ::1 8008`

### REFERÊNCIAS UTILIZADAS:
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/split/index.html)  
- _The Linux Programming Interface_
