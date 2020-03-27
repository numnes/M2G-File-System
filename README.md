						 __  __ ____   ____ 
						|  \/  |___ \ / ___|
						| |\/| | __) | |  _ 
						| |  | |/ __/| |_| |
						|_|  |_|_____|\____|
						   -File System-


Sistema de arquivos desenvolvido na disciplina de Sistemas Operacionais. Utiliza uma abordagem semelhante ao sistema EXT2, porém com uma estrutura mais simples, dividindo o disco em apenas um setor e omitindo alguns campos no superblock.


### Compilar
Acessar o diretorio onde está o programa e executar o comando ‘make’.

### Utilização
Para utilizar o software de controle do sistema de arquivos basta executar o arquivo “m2g”. As diretivas que acompanham o comando de execução definem a ação a ser executada. 

### Diretivas
Para executar as diferentes funcionalidades do software gestor do sistema de arquivos é preciso incluir diretivas no comando de execução, cada diretiva se relaciona a uma ação.

Ex:  
`./m2g --diretivaAcao1 `

As ações validas são:

##### FORMATAÇÃO DE DISCO
`./m2g [-f || --format] [caminhoDisco]`


##### IMPORTAR ARQUIVOS PARA UM DISCO
`./m2g [-i || --import] [caminhoParaArquivoSerCopiado] [caminhoDisco] [CaminhoPastaDestinoDentroDoDisco]`  
*OBS*: Caso o último campo fique em branco o arquivo será copiado para o diretorio raiz do disco.


##### EXPORTAR ARQUIVOS DE UM DISCO
`./m2g [-e || --export] [caminhoPastaDestino] [caminhoDisco] [caminhoArquivoDisco]`


##### CRIAR UM DIRETÓRIO
`./m2g  [-md || --makeDir] [caminhoDisco] [caminhoNovoDiretorio]/[nomeDiretorio]`


##### MODO DE VISUALIZAÇÃO 
`./m2g [-v || --view] [caminhoDisco]`
*OBS*: Ao entrar neste modo as entradas do diretório raiz são exibidas. É possível navegar entre elas inserindo seus nomes. Para sair do modo de visualização é necessário digitar o comando “exit”.


##### HELP
`./m2g [-h || --help]`
*OBS*: Exibe a mesma descrição que está acima dos comandos possíveis.

