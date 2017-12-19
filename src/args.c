#include "args.h"

void decode(int argc, char* argv[]){
	int i;
	for (i = 0; i < argc - 1; i++){
		if(strcmp("-i",argv[i]) == 0 && (argv[i+1] != NULL || strcmp(argv[i+1],"") != 0)){
			strcpy(img_f_name, argv[i+1]);
		} else if(strcmp("-o",argv[i]) == 0 && (argv[i+1] != NULL || strcmp(argv[i+1],"") == 0)){
			strcpy(diag_f_name, argv[i+1]);
		} else if((strcmp("-h",argv[i]) == 0 || (strcmp("--help",argv[i]) == 0)) && (argv[i+1] != NULL || strcmp(argv[i+1],"") == 0)){
			print_help();
			exit(0);
		} else if(argv[i+1] == NULL || strcmp(argv[i+1],"") == 0){
			fprintf(stderr, "Invalid number of arguments\n\n");
			print_help();
			exit(1);
		}
	}
	if(diag_f_name == NULL || strcmp(diag_f_name,"") == 0 || img_f_name == NULL || strcmp(img_f_name,"") == 0){
		fprintf(stderr, "Unknown args\n\n");
		print_help();
		exit(1);
	}
}

void print_help(){
	fprintf(stderr, "catarata é capaz de processar um conjunto de parâmetros, como indicado a seguir:\n");
	fprintf(stderr, "\t-i\t<input-image>\t a imagem de entrada a ser processada.\n");
	fprintf(stderr, "\t-o\t<diagnose-file>\t (opcional) indica o nome do arquivo texto contendo o diagnóstico. Caso opte por não adicionar essa opção, o diagnostico será impresso na saída padrão.\n");
	fprintf(stderr, "\t-h\t--help\tImprime o menu de ajuda.\n");
	fprintf(stderr, "Exemplo de execução:\n");
	fprintf(stderr, "\t./catarata -i imagem.bmp -f bmp -o diagnostico.txt\n");
	fprintf(stderr, "O arquivo de diagnóstico conterá ao final da execução:\n");
	fprintf(stderr, "\tDiagnóstico Geral: Com/Sem catarata.\n");
	fprintf(stderr, "\tPorcentagem de Comprometimento: porcentagem%%\n");
}