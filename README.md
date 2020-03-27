						 __  __ ____   ____ 
						|  \/  |___ \ / ___|
						| |\/| | __) | |  _ 
						| |  | |/ __/| |_| |
						|_|  |_|_____|\____|
						   -File System-


Sistema de arquivos desenvolvido na disciplina de Sistemas Operacionais. Utiliza uma abordagem semelhante ao sistema EXT2, porém com uma estrutura mais simples, dividindo o disco em apenas um setor e omitindo alguns campos no superblock.

# Utilização
A utilização do sistema de arquivos é feita atravéz deste software de gestão de disco, para utilizar o software basta clonar este repositório e compilar o programa. O repositório contém um arquivo *Makefile*, este é um arquivo usado pelo utilitario *make* para automatizar o processo de compilação (para saber mais acesse este [tutorial sobre makefiles](https://www.embarcados.com.br/introducao-ao-makefile/) ). Para compilar usando o comando *make* é preciso ter o *make* já instalado em sua máquina, caso ele já esteja instalado basta executar os comandos abaixo para utilizar o M2G File System.
``` 
git clone https://github.com/matheusnunesismael/M2G-File-System.git
cd M2G-File-System/
make
```
Se tudo der certo e o comando make não retornar nenhum erro o programa vai estar pronto pra ser usado :blush:  
  
Para utilizar o software de controle do sistema de arquivos basta executar o arquivo “m2g”, desse jeito:  
`./m2g`
  
# Diretivas
Para executar as diferentes funcionalidades do software gestor do sistema de arquivos é preciso incluir diretivas no comando de execução, cada diretiva se relaciona a uma ação. Por exemplo:  
`./m2g --diretivaAcao1 `

As ações validas são:

##### HELP
`./m2g [-h || --help]`  
Exibe uma listagem das diretivas validas no sistema de arquivos.
<br>
<br>
##### FORMATAÇÃO DE DISCO
`./m2g [-f || --format] [caminhoDisco]`  
Esta ação formata o disco selecionado com a estrutura padrão M2G, depois de executada o programa irá perguntar se o usuario deseja fazer uma formatação completa do disco ou apenas uma formatação rápida, a formatação completa irá percorrer todo o disco, apagando as informações escritas sobrescrevendo zeros em todos os bytes, já a formatação rápida vai apenas apagar o inicio do disco onde será escrito o bloco de controle (superblock).
<br>
<br>
##### IMPORTAR ARQUIVOS PARA UM DISCO
`./m2g [-i || --import] [caminhoParaArquivoSerCopiado] [caminhoDisco] [CaminhoPastaDestinoDentroDoDisco]`     
Esta ação irá copiar um arquivo qualquer que esteja em outra unidade de armazenamento para dentro do disco, para realizar esta ação é preciso que o disco de destino já tenha sido formatado no padrão M2G.  
**OBS**: Caso o último campo fique em branco o arquivo será copiado para o diretorio raiz do disco de destino.
<br>
<br>
##### EXPORTAR ARQUIVOS DE UM DISCO
`./m2g [-e || --export] [caminhoPastaDestino] [caminhoDisco] [caminhoArquivoDisco]`  
É possível também exportar arquivos de dentro de um disco formatado no padrão M2G para outra unidade de armazenamento.
<br>
<br>
##### CRIAR UM DIRETÓRIO
`./m2g  [-md || --makeDir] [caminhoDisco] [caminhoNovoDiretorio]/[nomeDiretorio]`  
Este comando irá criar um diretório logico dentro do disco formatado, os diretorios criados podem ser visualizados no modo de visualização.
<br>
<br>
##### MODO DE VISUALIZAÇÃO 
`./m2g [-v || --view] [caminhoDisco]`  
O modo de visualização permite navegar pelo disco selecionado, que esteja no padrão M2G, para entrar em um diretorio basta usar o comando cd:   
`m2g> cd minha pasta`
