#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>

//
// Função com rotina para utilizar cor do texto (textcolor) e cor do fundo (textbackground) em um único comando
// Para chamar dentro do programa, usar cores(x,y), onde x é numero da cor do texto e y é numero da cor do fundo
//
void cores(int ForgC, int BackC)
{
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	return;
}

//
// Função que define a cor de um texto
//
void CorTexto(int newcolor)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		(csbi.wAttributes & 0xf0) | newcolor);
}

//
// Função que direciona o cursor para um ponto da janela
//
void gotoxy(int x, int y)
{
	COORD c;
	c.X = x - 1;
	c.Y = y - 1;
	SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}

//
// Função que recebe uma string e a retorna toda em maiúscula
//
char *strtoupper(char *str){
  	char *string = str;
	
	if(str)
	{
		for(; *str; ++str)
			*str = toupper(*str);
	}
		
	return string;
}

//
// Função que exibe mensagem de introdução ao programa
//
void Introducao(){
     
    // Define a linguagem como Português, para mostrar acentos corretamente
	setlocale(LC_ALL, "Portuguese"); 

	// Exibe a mensagem e os créditos
	gotoxy(5,10);
	CorTexto(14);
	printf("Sistema de notas fiscais");
     
	gotoxy(5,12);
	CorTexto(12);
	printf("Autores: Alessandra dos Reis, Jonatha Cardoso e Leda Figueiredo");

	Sleep(2000); // Espera de 2 segundos

	system("cls"); // Limpa a tela
}

//
// Função que cria a estrutura do cupom fiscal e de cada item
//
typedef struct Item{
	char CodigoEAN[15];
	char Descricao[21];
	float Quantidade;
	float ValorUnitario;
	float ValorDesconto;
	struct Item* Proximo;   // A essência da lista: Liga um item da lista ao próximo
} CupomFiscal;              // O nome da lista é CupomFiscal, porém cada um dos itens tem o nome de Item

//
// Função que cria um cupom fiscal
//
CupomFiscal* CriarCupom()
{
	// Cria e prepara o cupom para o uso
	return NULL;
}

//
// Função que testa se um cupom fiscal está ou não vazio
//
int CupomVazio(CupomFiscal* Cupom)
{
	// Retorna 1 se o cupom for nulo (vazio) ou 0 se o cupom não for nulo (não vazio)
	return (Cupom == NULL);
}

//
// Função que adiciona um novo item no final do cupom fiscal
//
CupomFiscal* InserirItem(CupomFiscal* Cupom)
{
	// Cria dois itens auxiliares, sendo um para o item anterior do cupom...
	CupomFiscal *ItemAnterior = NULL;
	// ...e outro para o item atual do cupom
	CupomFiscal *ItemAtual;
	
	// Cria o novo item que será adicionado ao cupom
	CupomFiscal* NovoItem = (CupomFiscal*) malloc(sizeof(Cupom));
	
	// Limpa a tela e define a cor da fonte
	system("cls");	
	CorTexto(11);
	
	// Faz um laço para que o usuário digite o código EAN, enquanto não for digitado um valor entre 2 e 14 caracteres
	do {
		fflush(stdin); // Limpa sujeiras da memória
		printf("\n Código EAN: ");
		gets(NovoItem->CodigoEAN);	
	} while (strlen(NovoItem->CodigoEAN) < 2 || strlen(NovoItem->CodigoEAN) > 14);

	// Faz um laço para que o usuário digite a descrição, enquanto não for digitado um valor entre 2 e 20 caracteres
	do {
		fflush(stdin); // Limpa sujeiras da memória
		printf("\n Descrição: ");
		gets(NovoItem->Descricao);
	} while (strlen(NovoItem->Descricao) < 2 || strlen(NovoItem->Descricao) > 20);
	
	// Faz um laço para que o usuário digite a quantidade, enquanto não for digitado um valor maior que zero
	do {
		fflush(stdin); // Limpa sujeiras da memória
		printf("\n Quantidade: ");
		scanf("%f", &NovoItem->Quantidade);
	} while(NovoItem->Quantidade <= 0);
	
	// Faz um laço para que o usuário digite o valor unitário, enquanto não for digitado um valor maior que zero
	do {
		fflush(stdin); // Limpa sujeiras da memória
		printf("\n Valor unitário: ");
		scanf("%f", &NovoItem->ValorUnitario);
	} while(NovoItem->ValorUnitario <= 0);
	
	// Faz um laço para que o usuário digite o valor do desconto, enquanto não for digitado um valor maior ou igual a zero
	do {
		fflush(stdin); // Limpa sujeiras da memória
		printf("\n Valor do desconto: ");
		scanf("%f", &NovoItem->ValorDesconto);
	} while(NovoItem->ValorDesconto < 0);
	
	// Define o próximo item como nulo. Isto para que, ao ser adicionado mais um item, após deste, se possa achá-lo para fazer a ligação
	NovoItem->Proximo = NULL;

	if (CupomVazio(Cupom))
	{
		// Se o cupom estiver vazio, vai apenas colocar no cupom este item, que é o primeiro.
		Cupom = NovoItem;
	}
	else
	{
		// Se o cupom não estiver vazio, seguirá a seguinte lógica:
		
		// Iniciando no primeiro item do cupom (que se obtém ao pegar a lista Cupom), ele fará um laço
		ItemAtual = Cupom;
		
		// Esse laço sempre irá pro próximo item (o anterior recebe o atual, o atual recebe o próximo do atual) até que o próximo item seja NULL
		while(ItemAtual != NULL)
		{
			ItemAnterior = ItemAtual;
			ItemAtual = ItemAtual->Proximo;
		}
		
		// Quando o item atual for NULL, o item anterior é exatamente o último do cupom. Aí basta ligar o próximo do último ao novo item que criamos.
		ItemAnterior->Proximo = NovoItem;
	}
	
	// Limpa a tela e muda a cor da fonte
	system("cls");	
	CorTexto(10);
	
	// Exibe a mensagem que o item foi adicionado ao cupom
	printf("\n Item adicionado com sucesso!");	
	
	Sleep(2000); // Espera de 2 segundos
	
	// Retorna ao menu, repassando o cupom modificado 
	Menu(Cupom);
	
}

//
// Função que remove um item do cupom fiscal, conforme o EAN digitado
//
CupomFiscal* RemoverItem(CupomFiscal* Cupom, char CodigoEAN[15])
{
	// Cria dois itens auxiliares, sendo um para o item anterior do cupom...
	CupomFiscal* ItemAnterior = NULL;
	// ...e outro para o item atual do cupom, começando pelo primeiro item do cupom
	CupomFiscal* ItemAtual = Cupom;
	
	// Percorre todo o cupom, até, ou encontrar o primeiro item que contém o mesmo código EAN pesquisado ou chegar ao final sem ter encontrado item com EAN correspondente
	// Esse laço sempre irá pro próximo item (o anterior recebe o atual, o atual recebe o próximo do atual)
	while (ItemAtual != NULL && strcmp(ItemAtual->CodigoEAN, CodigoEAN) != 0)
	{
		ItemAnterior = ItemAtual;
		ItemAtual = ItemAtual->Proximo;
	}
	
	printf("x");
	
	//
	// Antes de liberarmos a memória e excluirmos o item atual, temos que manter a ligação entre os itens do cupom
	//
	
	// Se há apenas um item, ele apenas fará o apontamento do cupom pro próximo do último, que é exatamente NULL
	if (ItemAnterior == NULL)
		Cupom = ItemAtual->Proximo;
	// Se há mais de um item, temos que informar que o próximo do item anterior será não mais o item atual, mas o próximo deste item atual
	else
		ItemAnterior->Proximo = ItemAtual->Proximo;
		
	// Agora sim! Liberamos a memória e excluimos o item atual
	free(ItemAtual);

	// Limpa a tela e muda a cor da fonte
	system("cls");	
	CorTexto(10);
	
	// Exibe a mensagem que o item foi removido do cupom
	printf("\n Item removido com sucesso!");
	
	Sleep(2000); // Espera de 2 segundos

	// Retorna ao menu, repassando o cupom modificado 
	Menu(Cupom);

}

//
// Função que procura um item para ser excluído, conforme o EAN digitado
//
CupomFiscal* ProcurarItem(CupomFiscal* Cupom)
{
	// Se o cupom está vazio, sai do módulo
	if(CupomVazio(Cupom))
	{	
		// Limpa a tela e define a cor da fonte
		system("cls");
		CorTexto(10);
		
		// Exibe a mensagem que o cupom está vazio
		printf("\n O cupom está vazio!");
		
		Sleep(2000); // Espera de 2 segundos
		
		// Retorna ao menu
		Menu(Cupom);
	}
	
	// Cria um item auxiliar, para o item atual do cupom
	CupomFiscal* ItemAtual;

	CorTexto(11); // Muda a cor da fonte
	
	// Faz um laço para que o usuário digite o código EAN, enquanto não for digitado um valor entre 2 e 14 caracteres
	char CodigoEAN[15];	
	do {
		system("cls"); // Limpa a tela
		fflush(stdin); // Limpa sujeiras da memória
		printf("\n Informe o código EAN do item: ");
		gets(CodigoEAN);
	} while (strlen(CodigoEAN) < 2 || strlen(CodigoEAN) > 14);
	
	int Encontrado = 0;
	
	// Percorre todos os itens do cupom
	for (ItemAtual = Cupom; ItemAtual != NULL; ItemAtual = ItemAtual->Proximo)
	{		
		// Caso o código EAN do item atual seja igual ao código EAN digitado antes, encontramos um item correspondente
		if (strcmp(ItemAtual->CodigoEAN, CodigoEAN) == 0)
			Encontrado = 1;
	}
	
	// Se foi encontrado um item correspondente, chamamos a função acima, que remove um item
	if (Encontrado == 1)
	{
		RemoverItem(Cupom, CodigoEAN);
	}
	// Se não foi encontrado, informa ao usuário e retorna ao menu
	else
	{
		// Limpa a tela e muda a cor da fonte
		system("cls");	
		CorTexto(10);
		
		// Exibe a mensagem que não foi encontrado correspondente no cupom
		printf("\n Código EAN não encontrado!");
		
		Sleep(2000); // Espera de 2 segundos
		
		// Retorna ao menu, repassando o cupom modificado
		Menu(Cupom);
	}
		
}

//
// Função que cancela um cupom
//
CupomFiscal* CancelarCupom(CupomFiscal* Cupom, int Retornar)
{	
	system("cls"); // Limpa a tela

	// Só vai exibir a pergunta pro usuário se esse módulo foi acessado pelo menu principal
	// Se o cancelamento do cupom for devido a uma saida do sistema, não fará a pergunta
	if (Retornar == 1)
	{	
		// Faz um laço para que o usuário digite se quer cancelar o cupom, enquanto não for digitado S ou N
		char Opcao;
		do {
			system("cls"); // Limpa a tela
			fflush(stdin); // Limpa sujeiras da memória
			printf("\n Deseja cancelar o cupom? \n\n [S] para sim ou [N] para não: ");
			scanf("%c", &Opcao);
		} while(Opcao != 'S' && Opcao != 'N');
		
		// Caso o usuário tenha digitado que não quer cancelar, retornamos para o menu principal
		if (Opcao == 'N')
		{
			Menu(Cupom);
		}	
	}

	// Cria dois itens auxiliares, sendo um para o item atual do cupom (iniciando do primeiro item do cupom)...
	CupomFiscal *ItemAtual = Cupom;
	// ...e outro para o item atual do cupom, começando pelo primeiro item do cupom
	CupomFiscal *ItemSeguinte;
	
	// Esse laço sempre fará o seguinte: Pega o próximo item, para liberar o atual. Depois de liberar o atual, aí sim prossegue pro próximo item
    while (ItemAtual != NULL)
	{
        ItemSeguinte = ItemAtual->Proximo;
        free(ItemAtual);
        ItemAtual = ItemSeguinte;
    }
    
    // Para encerrar, atribui o último item ao cupom - claro, este último será exatamente nulo, pois o próximo do último item é exatamente NULL
    Cupom = ItemAtual;
	
	// Limpa a tela e muda a cor da fonte
	system("cls");
	CorTexto(10);
	
	// Exibe a mensagem que o item foi removido do cupom
	printf("\n Cupom cancelado com sucesso!");
	
	Sleep(2000); // Espera de 2 segundos
	
	// Só vai exibir a pergunta pro usuário se esse módulo foi acessado pelo menu principal
	// Se o cancelamento do cupom for devido a uma saida do sistema, não fará a pergunta
	if (Retornar == 1)
	{	
		// Retorna ao menu, repassando o cupom modificado
		Menu(Cupom);
	}
	
}

//
// Função que encerra um cupom e o imprime depois
//
CupomFiscal* FecharCupom(CupomFiscal* Cupom)
{
	// Se o cupom está vazio, sai do módulo
	if(CupomVazio(Cupom))
	{	
		// Limpa a tela e define a cor da fonte
		system("cls");
		CorTexto(10);
		
		// Exibe a mensagem que o cupom está vazio
		printf("\n O cupom está vazio!");
		
		Sleep(2000); // Espera de 2 segundos
		
		// Retorna ao menu
		Menu(Cupom);
	}
	
	// Faz um laço para que o usuário digite se quer cancelar o cupom, enquanto não for digitado S ou N
	char Opcao;	
	do {
		system("cls"); // Limpa a tela		
		fflush(stdin); // Limpa sujeiras da memória
		printf("\n Deseja encerrar o cupom? \n\n Digite [S] para sim ou [N] para não:  ");
		scanf("%c", &Opcao);
	} while(Opcao != 'S' && Opcao != 'N');
	
	// Caso o usuário tenha digitado que não quer fechar, retornamos para o menu principal
	if (Opcao == 'N')
	{
		// Retorna ao menu
		Menu(Cupom);
		
	}
	
	system("cls"); // Limpa a tela
	
	// Exibe o cabeçalho da nota fiscal
	printf("\n +----------------------------------------+");
	printf("\n | CUPOM FISCAL                           |");
	printf("\n | 00.000.000/0000-00                     |");
	printf("\n | ESTABELECIMENTO                        |");
	printf("\n +----------------------------------------+");
	
	// Variável que controla o número do item exibido
	int Item = 1;
	
	// Variáveis com as somas dos valores dos produtos e dos descontos
	float SomaProdutos = 0.0;
	float SomaDescontos = 0.0;
	
	// Cria um item auxiliar, para o item atual do cupom
	CupomFiscal* ItemAtual;
	
	// Percorre todos os itens do cupom
	for (ItemAtual = Cupom; ItemAtual != NULL; ItemAtual = ItemAtual->Proximo)
	{
		CorTexto(7); // Define a cor da fonte
		
		// Exibe na primeira linha, com os devidos espaçamentos, o número do item, o seu código EAN e a sua descrição (usando função para deixar em maiúsculo)
		printf("\n | %2d %14s %20s |", Item, ItemAtual->CodigoEAN, strtoupper(ItemAtual->Descricao));
		// Exibe na segunda linha, com os devidos espaçamentos, a quantidade, o valor unitário e o valor total (produto dos dois anteriores)
		printf("\n | %10.2f x %.2f %20.2f |", ItemAtual->Quantidade, ItemAtual->ValorUnitario, (ItemAtual->Quantidade * ItemAtual->ValorUnitario) );
		
		// Adiciona à soma dos produtos o valor total do item atual
		SomaProdutos = SomaProdutos + (ItemAtual->ValorUnitario*ItemAtual->Quantidade);
		
		// Se houver desconto, exibe...
		if (ItemAtual->ValorDesconto > 0.00)
		{
			// ...este valor de desconto
			printf("\n |          Desconto do item: %11.2f |", ItemAtual->ValorDesconto);
			
			// Adiciona à soma dos descontos o valor de desconto do item atual
			SomaDescontos = SomaDescontos + ItemAtual->ValorDesconto;
		}
		
		// Incrementa o número do item atual
		Item++;
	}
	
	CorTexto(14); // Define a cor da fonte
	
	printf("\n +----------------------------------------+");
	
	// Exibe os totais da nota fiscal
	printf("\n | Valor total dos itens: %15.2f |", SomaProdutos);
	printf("\n | Valor total dos descontos: %11.2f |", SomaDescontos);
	printf("\n | Valor total do cupom: %16.2f |", (SomaProdutos-SomaDescontos));
	
	// Exibe o rodapé da nota fiscal
	printf("\n +----------------------------------------+");
	printf("\n | OBRIGADO PELA PREFERÊNCIA!             |");
	printf("\n +----------------------------------------+");
	
	CorTexto(7); // Define a cor da fonte
	
	// Solicita uma tecla digitada para retornar ao menu principal
	printf("\n\n\n Aperte uma tecla para retornar:  ");
	getch();
	
	// Retorna ao menu
	Menu(Cupom);	
}

//
// Função que sai do programa, cancelando o cupom antes
//
CupomFiscal* Sair(CupomFiscal* Cupom)
{	
	// Faz um laço para que o usuário digite se quer sair do programa, enquanto não for digitado S ou N
	char Opcao;
	do {
		system("cls"); // Limpa a tela
		fflush(stdin); // Limpa sujeiras da memória
		printf("\n Deseja sair? \n\n Digite [S] para sim ou [N] para não:  ");
		scanf("%c", &Opcao);
	} while(Opcao != 'S' && Opcao != 'N');
	
	// Se digitou S, cancela o cupom e sai do sistema
	if (Opcao == 'S')
	{
		// Chama a função que cancela o cupom
		CancelarCupom(Cupom, 0);
	
		system("cls"); // Limpa a tela
		
		// Sai do programa
		exit(0);
	}
	// Se digitou N, retorna ao menu principal
	else
	{
		Menu(Cupom);
	}
}

//
// Função que exibe o menu principal
//
Menu(CupomFiscal* Cupom)
{
	// Exibe o menu principal enquanto o usuário não tiver digitado 1, 2, 3, 4 ou 0 
	int Opcao = 0;	
	do {
		// Limpa a tela e define a cor da fonte
		system("cls");		
		CorTexto(7);
		
		// Exibe as opções do menu
		printf("\n [1] Inserir item ao cupom fiscal");
		printf("\n [2] Remover item do cupom fiscal");
		printf("\n [3] Cancelar cupom fiscal");
		printf("\n [4] Fechar e imprimir cupom fiscal");
		printf("\n [0] Sair do sistema");
		
		// Exibe esta mensagem se o usuário já digitou antes uma opção, e a mesma é inválida
		if (Opcao < 0 || Opcao > 4)
		{
			CorTexto(12); // Define a cor da fonte
			printf("\n\n Opção inválida! Informe a opção desejada: ");
		}
		// Exibe esta mensagem se é a primeira vez que o usuário vai informar uma opção
		else
		{
			CorTexto(14); // Define a cor da fonte
			printf("\n\n Informe a opção desejada: ");
		}
		
		fflush(stdin); // Limpa sujeiras da memória
		scanf("%d", &Opcao);		
	} while (Opcao < 0 || Opcao > 4);
	
	// Testa a opção digitada, redirecionando para o módulo correspondente
	switch(Opcao)
	{
		case 1:
			InserirItem(Cupom);
			break;
		case 2:
			ProcurarItem(Cupom);
			break;
		case 3:
			CancelarCupom(Cupom, 1);
			break;
		case 4:
			FecharCupom(Cupom);
			break;
		case 0:	
			Sair(Cupom);
			break;
	}
		
}

//
// Função principal
//
main()
{
	Introducao();
	
	// Cria a lista com o nome de Cupom. Ela será usada durante todo o programa
	CupomFiscal* Cupom;
	// Chama a função que cria o cupom
	Cupom = CriarCupom();
	
	Menu(Cupom);
	
}
