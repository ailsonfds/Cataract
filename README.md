# Trabalho de ITP-PTP sobre detecção de catarata em imagens 2017.2

## Como compilar

1. Antes de começar certifique-se de que você possui os seguintes programas instalados
	- [GNU Make](https://www.gnu.org/software/make/) : para verificar se está instalado basta digitar no terminal:
		
		  make --version


	  caso esteja instalado deverá apresentar algo como:
		
		  GNU Make X.X
		  Built for x86_64-pc-linux-gnu


	- [GCC](https://gcc.gnu.org/) : para verificar se está instalado basta digitar no terminal:

		  gcc --version


	  caso esteja instalado deverá apresentar algo como:

		  gcc (OS info) x.x.x releasedate



2. Agora vamos compilar o projeto:
	- Abra o terminal no diretório inicial do projeto.
	- Digite o comando:
		
		  make

	- seu executável será criado com o nome _catarata_ no diretório

		  bin/

## Funcionalidades do programa

_catarata_ é capaz de processar um conjunto de parâmetros, como indicado a  seguir:
- _-i_ _input-image_ a  imagem de entrada a  ser processada.
	<aside class="notice">
	Não me responsabilizo pelo diagnóstico em imagens que não sejam ppm.
	</aside>
- _-o_ _diagnose-file_ (opcional) indica o nome do arquivo texto contendo o diagnóstico. Caso opte por não adicionar essa opção, o diagnostico será impresso na saída padrão.

Exemplo de execução: 

	./catarata -i imagem.ppm -o diagnostico.txt

O arquivo de diagnóstico conterá ao final da execução:
1. Diagnóstico Geral: Com/Sem catarata.
2. Porcentagem de Comprometimento: _porcentagem_%.

## Descrição do projeto

Este projeto de diagnóstico de catarata foi desenvolvido em linguagem C, a ser executado, em sua versão mais simples, através de linha de comando (entrada e saída em um console/terminal). 