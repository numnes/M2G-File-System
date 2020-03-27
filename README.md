						 __  __ ____   ____ 
						|  \/  |___ \ / ___|
						| |\/| | __) | |  _ 
						| |  | |/ __/| |_| |
						|_|  |_|_____|\____|
						   -File System-


Sistema de arquivos desenvolvido na disciplina de Sistemas Operacionais. Utiliza uma abordagem semelhante ao sistema EXT2, porém com uma estrutura mais simples, dividindo o disco em apenas um setor e omitindo alguns campos no superblock.


### Compilar
Para compilar o software gestor do sistema de arquivos basta clonar este repositorio, acessar o diretorio do programa e executar o comando ‘make’. Caso tudo dê certo o programa estará pronto para ser usado.
  
### Utilização
Para utilizar o software de controle do sistema de arquivos basta executar o arquivo “m2g”. As diretivas que acompanham o comando de execução definem a ação a ser executada. 
  
### Diretivas
Para executar as diferentes funcionalidades do software gestor do sistema de arquivos é preciso incluir diretivas no comando de execução, cada diretiva se relaciona a uma ação. Por exemplo:  
`./m2g --diretivaAcao1 `

As ações validas são:

##### HELP
`./m2g [-h || --help]`  
Exibe uma listagem das diretivas validas no sistema de arquivos.

##### FORMATAÇÃO DE DISCO
`./m2g [-f || --format] [caminhoDisco]`  
Esta ação formata o disco selecionado com a estrutura padrão M2G, depois de executada o programa irá perguntar se o usuario deseja fazer uma formatação completa do disco ou apenas uma formatação rápida, a formatação completa irá percorrer todo o disco, apagando as informações escritas sobrescrevendo zeros em todos os bytes, já a formatação rápida vai apenas apagar o inicio do disco onde será escrito o bloco de controle (superblock).


##### IMPORTAR ARQUIVOS PARA UM DISCO
`./m2g [-i || --import] [caminhoParaArquivoSerCopiado] [caminhoDisco] [CaminhoPastaDestinoDentroDoDisco]`     
Esta ação irá copiar um arquivo qualquer que esteja em outra unidade de armazenamento para dentro do disco, para realizar esta ação é preciso que o disco de destino já tenha sido formatado no padrão M2G.  
**OBS**: Caso o último campo fique em branco o arquivo será copiado para o diretorio raiz do disco de destino.
  

##### EXPORTAR ARQUIVOS DE UM DISCO
`./m2g [-e || --export] [caminhoPastaDestino] [caminhoDisco] [caminhoArquivoDisco]`  
É possível também exportar arquivos de dentro de um disco formatado no padrão M2G para outra unidade de armazenamento.
  

##### CRIAR UM DIRETÓRIO
`./m2g  [-md || --makeDir] [caminhoDisco] [caminhoNovoDiretorio]/[nomeDiretorio]`  
Este comando irá criar um diretório logico dentro do disco formatado, os diretorios criados podem ser visualizados no modo de visualização.


##### MODO DE VISUALIZAÇÃO 
`./m2g [-v || --view] [caminhoDisco]`  
O modo de visualização permite navegar pelo disco selecionado, que esteja no padrão M2G, para entrar em um diretorio basta usar o comando cd:   
`m2g> cd minha pasta`
