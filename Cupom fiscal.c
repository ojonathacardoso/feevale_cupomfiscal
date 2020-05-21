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
// Fun��o com rotina para utilizar cor do texto (textcolor) e cor do fundo (textbackground) em um �nico comando
// Para chamar dentro do programa, usar cores(x,y), onde x � numero da cor do texto e y � numero da cor do fundo
//
void cores(int ForgC, int BackC)
{
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	return;
}

//
// Fun��o que define a cor de um texto
//
void CorTexto(int newcolor)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		(csbi.wAttributes & 0xf0) | newcolor);
}

//
// Fun��o que direciona o cursor para um ponto da janela
//
void gotoxy(int x, int y)
{
	COORD c;
	c.X = x - 1;
	c.Y = y - 1;
	SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}

//
// Fun��o que recebe uma string e a retorna toda em mai�scula
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
// Fun��o que exibe mensagem de introdu��o ao programa
//
void Introducao(){
     
    // Define a linguagem como Portugu�s, para mostrar acentos corretamente
	setlocale(LC_ALL, "Portuguese"); 

	// Exibe a mensagem e os cr�ditos
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
// Fun��o que cria a estrutura do cupom fiscal e de cada item
//
typedef struct Item{
	char CodigoEAN[15];
	char Descricao[21];
	float Quantidade;
	float ValorUnitario;
	float ValorDesconto;
	struct Item* Proximo;   // A ess�ncia da lista: Liga um item da lista ao pr�ximo
} CupomFiscal;              // O nome da lista � CupomFiscal, por�m cada um dos itens tem o nome de Item

//
// Fun��o que cria um cupom fiscal
//
CupomFiscal* CriarCupom()
{
	// Cria e prepara o cupom para o uso
	return NULL;
}

//
// Fun��o que testa se um cupom fiscal est� ou n�o vazio
//
int CupomVazio(CupomFiscal* Cupom)
{
	// Retorna 1 se o cupom for nulo (vazio) ou 0 se o cupom n�o for nulo (n�o vazio)
	return (Cupom == NULL);
}

//
// Fun��o que adiciona um novo item no final do cupom fiscal
//
CupomFiscal* InserirItem(CupomFiscal* Cupom)
{
	// Cria dois itens auxiliares, sendo um para o item anterior do cupom...
	CupomFiscal *ItemAnterior = NULL;
	// ...e outro para o item atual do cupom
	CupomFiscal *ItemAtual;
	
	// Cria o novo item que ser� adicionado ao cupom
	CupomFiscal* NovoItem = (CupomFiscal*) malloc(sizeof(Cupom));
	
	// Limpa a tela e define a cor da fonte
	system("cls");	
	CorTexto(11);
	
	// Faz um la�o para que o usu�rio digite o c�digo EAN, enquanto n�o for digitado um valor entre 2 e 14 caracteres
	do {
		fflush(stdin); // Limpa sujeiras da mem�ria
		printf("\n C�digo EAN: ");
		gets(NovoItem->CodigoEAN);	
	} while (strlen(NovoItem->CodigoEAN) < 2 || strlen(NovoItem->CodigoEAN) > 14);

	// Faz um la�o para que o usu�rio digite a descri��o, enquanto n�o for digitado um valor entre 2 e 20 caracteres
	do {
		fflush(stdin); // Limpa sujeiras da mem�ria
		printf("\n Descri��o: ");
		gets(NovoItem->Descricao);
	} while (strlen(NovoItem->Descricao) < 2 || strlen(NovoItem->Descricao) > 20);
	
	// Faz um la�o para que o usu�rio digite a quantidade, enquanto n�o for digitado um valor maior que zero
	do {
		fflush(stdin); // Limpa sujeiras da mem�ria
		printf("\n Quantidade: ");
		scanf("%f", &NovoItem->Quantidade);
	} while(NovoItem->Quantidade <= 0);
	
	// Faz um la�o para que o usu�rio digite o valor unit�rio, enquanto n�o for digitado um valor maior que zero
	do {
		fflush(stdin); // Limpa sujeiras da mem�ria
		printf("\n Valor unit�rio: ");
		scanf("%f", &NovoItem->ValorUnitario);
	} while(NovoItem->ValorUnitario <= 0);
	
	// Faz um la�o para que o usu�rio digite o valor do desconto, enquanto n�o for digitado um valor maior ou igual a zero
	do {
		fflush(stdin); // Limpa sujeiras da mem�ria
		printf("\n Valor do desconto: ");
		scanf("%f", &NovoItem->ValorDesconto);
	} while(NovoItem->ValorDesconto < 0);
	
	// Define o pr�ximo item como nulo. Isto para que, ao ser adicionado mais um item, ap�s deste, se possa ach�-lo para fazer a liga��o
	NovoItem->Proximo = NULL;

	if (CupomVazio(Cupom))
	{
		// Se o cupom estiver vazio, vai apenas colocar no cupom este item, que � o primeiro.
		Cupom = NovoItem;
	}
	else
	{
		// Se o cupom n�o estiver vazio, seguir� a seguinte l�gica:
		
		// Iniciando no primeiro item do cupom (que se obt�m ao pegar a lista Cupom), ele far� um la�o
		ItemAtual = Cupom;
		
		// Esse la�o sempre ir� pro pr�ximo item (o anterior recebe o atual, o atual recebe o pr�ximo do atual) at� que o pr�ximo item seja NULL
		while(ItemAtual != NULL)
		{
			ItemAnterior = ItemAtual;
			ItemAtual = ItemAtual->Proximo;
		}
		
		// Quando o item atual for NULL, o item anterior � exatamente o �ltimo do cupom. A� basta ligar o pr�ximo do �ltimo ao novo item que criamos.
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
// Fun��o que remove um item do cupom fiscal, conforme o EAN digitado
//
CupomFiscal* RemoverItem(CupomFiscal* Cupom, char CodigoEAN[15])
{
	// Cria dois itens auxiliares, sendo um para o item anterior do cupom...
	CupomFiscal* ItemAnterior = NULL;
	// ...e outro para o item atual do cupom, come�ando pelo primeiro item do cupom
	CupomFiscal* ItemAtual = Cupom;
	
	// Percorre todo o cupom, at�, ou encontrar o primeiro item que cont�m o mesmo c�digo EAN pesquisado ou chegar ao final sem ter encontrado item com EAN correspondente
	// Esse la�o sempre ir� pro pr�ximo item (o anterior recebe o atual, o atual recebe o pr�ximo do atual)
	while (ItemAtual != NULL && strcmp(ItemAtual->CodigoEAN, CodigoEAN) != 0)
	{
		ItemAnterior = ItemAtual;
		ItemAtual = ItemAtual->Proximo;
	}
	
	printf("x");
	
	//
	// Antes de liberarmos a mem�ria e excluirmos o item atual, temos que manter a liga��o entre os itens do cupom
	//
	
	// Se h� apenas um item, ele apenas far� o apontamento do cupom pro pr�ximo do �ltimo, que � exatamente NULL
	if (ItemAnterior == NULL)
		Cupom = ItemAtual->Proximo;
	// Se h� mais de um item, temos que informar que o pr�ximo do item anterior ser� n�o mais o item atual, mas o pr�ximo deste item atual
	else
		ItemAnterior->Proximo = ItemAtual->Proximo;
		
	// Agora sim! Liberamos a mem�ria e excluimos o item atual
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
// Fun��o que procura um item para ser exclu�do, conforme o EAN digitado
//
CupomFiscal* ProcurarItem(CupomFiscal* Cupom)
{
	// Se o cupom est� vazio, sai do m�dulo
	if(CupomVazio(Cupom))
	{	
		// Limpa a tela e define a cor da fonte
		system("cls");
		CorTexto(10);
		
		// Exibe a mensagem que o cupom est� vazio
		printf("\n O cupom est� vazio!");
		
		Sleep(2000); // Espera de 2 segundos
		
		// Retorna ao menu
		Menu(Cupom);
	}
	
	// Cria um item auxiliar, para o item atual do cupom
	CupomFiscal* ItemAtual;

	CorTexto(11); // Muda a cor da fonte
	
	// Faz um la�o para que o usu�rio digite o c�digo EAN, enquanto n�o for digitado um valor entre 2 e 14 caracteres
	char CodigoEAN[15];	
	do {
		system("cls"); // Limpa a tela
		fflush(stdin); // Limpa sujeiras da mem�ria
		printf("\n Informe o c�digo EAN do item: ");
		gets(CodigoEAN);
	} while (strlen(CodigoEAN) < 2 || strlen(CodigoEAN) > 14);
	
	int Encontrado = 0;
	
	// Percorre todos os itens do cupom
	for (ItemAtual = Cupom; ItemAtual != NULL; ItemAtual = ItemAtual->Proximo)
	{		
		// Caso o c�digo EAN do item atual seja igual ao c�digo EAN digitado antes, encontramos um item correspondente
		if (strcmp(ItemAtual->CodigoEAN, CodigoEAN) == 0)
			Encontrado = 1;
	}
	
	// Se foi encontrado um item correspondente, chamamos a fun��o acima, que remove um item
	if (Encontrado == 1)
	{
		RemoverItem(Cupom, CodigoEAN);
	}
	// Se n�o foi encontrado, informa ao usu�rio e retorna ao menu
	else
	{
		// Limpa a tela e muda a cor da fonte
		system("cls");	
		CorTexto(10);
		
		// Exibe a mensagem que n�o foi encontrado correspondente no cupom
		printf("\n C�digo EAN n�o encontrado!");
		
		Sleep(2000); // Espera de 2 segundos
		
		// Retorna ao menu, repassando o cupom modificado
		Menu(Cupom);
	}
		
}

//
// Fun��o que cancela um cupom
//
CupomFiscal* CancelarCupom(CupomFiscal* Cupom, int Retornar)
{	
	system("cls"); // Limpa a tela

	// S� vai exibir a pergunta pro usu�rio se esse m�dulo foi acessado pelo menu principal
	// Se o cancelamento do cupom for devido a uma saida do sistema, n�o far� a pergunta
	if (Retornar == 1)
	{	
		// Faz um la�o para que o usu�rio digite se quer cancelar o cupom, enquanto n�o for digitado S ou N
		char Opcao;
		do {
			system("cls"); // Limpa a tela
			fflush(stdin); // Limpa sujeiras da mem�ria
			printf("\n Deseja cancelar o cupom? \n\n [S] para sim ou [N] para n�o: ");
			scanf("%c", &Opcao);
		} while(Opcao != 'S' && Opcao != 'N');
		
		// Caso o usu�rio tenha digitado que n�o quer cancelar, retornamos para o menu principal
		if (Opcao == 'N')
		{
			Menu(Cupom);
		}	
	}

	// Cria dois itens auxiliares, sendo um para o item atual do cupom (iniciando do primeiro item do cupom)...
	CupomFiscal *ItemAtual = Cupom;
	// ...e outro para o item atual do cupom, come�ando pelo primeiro item do cupom
	CupomFiscal *ItemSeguinte;
	
	// Esse la�o sempre far� o seguinte: Pega o pr�ximo item, para liberar o atual. Depois de liberar o atual, a� sim prossegue pro pr�ximo item
    while (ItemAtual != NULL)
	{
        ItemSeguinte = ItemAtual->Proximo;
        free(ItemAtual);
        ItemAtual = ItemSeguinte;
    }
    
    // Para encerrar, atribui o �ltimo item ao cupom - claro, este �ltimo ser� exatamente nulo, pois o pr�ximo do �ltimo item � exatamente NULL
    Cupom = ItemAtual;
	
	// Limpa a tela e muda a cor da fonte
	system("cls");
	CorTexto(10);
	
	// Exibe a mensagem que o item foi removido do cupom
	printf("\n Cupom cancelado com sucesso!");
	
	Sleep(2000); // Espera de 2 segundos
	
	// S� vai exibir a pergunta pro usu�rio se esse m�dulo foi acessado pelo menu principal
	// Se o cancelamento do cupom for devido a uma saida do sistema, n�o far� a pergunta
	if (Retornar == 1)
	{	
		// Retorna ao menu, repassando o cupom modificado
		Menu(Cupom);
	}
	
}

//
// Fun��o que encerra um cupom e o imprime depois
//
CupomFiscal* FecharCupom(CupomFiscal* Cupom)
{
	// Se o cupom est� vazio, sai do m�dulo
	if(CupomVazio(Cupom))
	{	
		// Limpa a tela e define a cor da fonte
		system("cls");
		CorTexto(10);
		
		// Exibe a mensagem que o cupom est� vazio
		printf("\n O cupom est� vazio!");
		
		Sleep(2000); // Espera de 2 segundos
		
		// Retorna ao menu
		Menu(Cupom);
	}
	
	// Faz um la�o para que o usu�rio digite se quer cancelar o cupom, enquanto n�o for digitado S ou N
	char Opcao;	
	do {
		system("cls"); // Limpa a tela		
		fflush(stdin); // Limpa sujeiras da mem�ria
		printf("\n Deseja encerrar o cupom? \n\n Digite [S] para sim ou [N] para n�o:  ");
		scanf("%c", &Opcao);
	} while(Opcao != 'S' && Opcao != 'N');
	
	// Caso o usu�rio tenha digitado que n�o quer fechar, retornamos para o menu principal
	if (Opcao == 'N')
	{
		// Retorna ao menu
		Menu(Cupom);
		
	}
	
	system("cls"); // Limpa a tela
	
	// Exibe o cabe�alho da nota fiscal
	printf("\n +----------------------------------------+");
	printf("\n | CUPOM FISCAL                           |");
	printf("\n | 00.000.000/0000-00                     |");
	printf("\n | ESTABELECIMENTO                        |");
	printf("\n +----------------------------------------+");
	
	// Vari�vel que controla o n�mero do item exibido
	int Item = 1;
	
	// Vari�veis com as somas dos valores dos produtos e dos descontos
	float SomaProdutos = 0.0;
	float SomaDescontos = 0.0;
	
	// Cria um item auxiliar, para o item atual do cupom
	CupomFiscal* ItemAtual;
	
	// Percorre todos os itens do cupom
	for (ItemAtual = Cupom; ItemAtual != NULL; ItemAtual = ItemAtual->Proximo)
	{
		CorTexto(7); // Define a cor da fonte
		
		// Exibe na primeira linha, com os devidos espa�amentos, o n�mero do item, o seu c�digo EAN e a sua descri��o (usando fun��o para deixar em mai�sculo)
		printf("\n | %2d %14s %20s |", Item, ItemAtual->CodigoEAN, strtoupper(ItemAtual->Descricao));
		// Exibe na segunda linha, com os devidos espa�amentos, a quantidade, o valor unit�rio e o valor total (produto dos dois anteriores)
		printf("\n | %10.2f x %.2f %20.2f |", ItemAtual->Quantidade, ItemAtual->ValorUnitario, (ItemAtual->Quantidade * ItemAtual->ValorUnitario) );
		
		// Adiciona � soma dos produtos o valor total do item atual
		SomaProdutos = SomaProdutos + (ItemAtual->ValorUnitario*ItemAtual->Quantidade);
		
		// Se houver desconto, exibe...
		if (ItemAtual->ValorDesconto > 0.00)
		{
			// ...este valor de desconto
			printf("\n |          Desconto do item: %11.2f |", ItemAtual->ValorDesconto);
			
			// Adiciona � soma dos descontos o valor de desconto do item atual
			SomaDescontos = SomaDescontos + ItemAtual->ValorDesconto;
		}
		
		// Incrementa o n�mero do item atual
		Item++;
	}
	
	CorTexto(14); // Define a cor da fonte
	
	printf("\n +----------------------------------------+");
	
	// Exibe os totais da nota fiscal
	printf("\n | Valor total dos itens: %15.2f |", SomaProdutos);
	printf("\n | Valor total dos descontos: %11.2f |", SomaDescontos);
	printf("\n | Valor total do cupom: %16.2f |", (SomaProdutos-SomaDescontos));
	
	// Exibe o rodap� da nota fiscal
	printf("\n +----------------------------------------+");
	printf("\n | OBRIGADO PELA PREFER�NCIA!             |");
	printf("\n +----------------------------------------+");
	
	CorTexto(7); // Define a cor da fonte
	
	// Solicita uma tecla digitada para retornar ao menu principal
	printf("\n\n\n Aperte uma tecla para retornar:  ");
	getch();
	
	// Retorna ao menu
	Menu(Cupom);	
}

//
// Fun��o que sai do programa, cancelando o cupom antes
//
CupomFiscal* Sair(CupomFiscal* Cupom)
{	
	// Faz um la�o para que o usu�rio digite se quer sair do programa, enquanto n�o for digitado S ou N
	char Opcao;
	do {
		system("cls"); // Limpa a tela
		fflush(stdin); // Limpa sujeiras da mem�ria
		printf("\n Deseja sair? \n\n Digite [S] para sim ou [N] para n�o:  ");
		scanf("%c", &Opcao);
	} while(Opcao != 'S' && Opcao != 'N');
	
	// Se digitou S, cancela o cupom e sai do sistema
	if (Opcao == 'S')
	{
		// Chama a fun��o que cancela o cupom
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
// Fun��o que exibe o menu principal
//
Menu(CupomFiscal* Cupom)
{
	// Exibe o menu principal enquanto o usu�rio n�o tiver digitado 1, 2, 3, 4 ou 0 
	int Opcao = 0;	
	do {
		// Limpa a tela e define a cor da fonte
		system("cls");		
		CorTexto(7);
		
		// Exibe as op��es do menu
		printf("\n [1] Inserir item ao cupom fiscal");
		printf("\n [2] Remover item do cupom fiscal");
		printf("\n [3] Cancelar cupom fiscal");
		printf("\n [4] Fechar e imprimir cupom fiscal");
		printf("\n [0] Sair do sistema");
		
		// Exibe esta mensagem se o usu�rio j� digitou antes uma op��o, e a mesma � inv�lida
		if (Opcao < 0 || Opcao > 4)
		{
			CorTexto(12); // Define a cor da fonte
			printf("\n\n Op��o inv�lida! Informe a op��o desejada: ");
		}
		// Exibe esta mensagem se � a primeira vez que o usu�rio vai informar uma op��o
		else
		{
			CorTexto(14); // Define a cor da fonte
			printf("\n\n Informe a op��o desejada: ");
		}
		
		fflush(stdin); // Limpa sujeiras da mem�ria
		scanf("%d", &Opcao);		
	} while (Opcao < 0 || Opcao > 4);
	
	// Testa a op��o digitada, redirecionando para o m�dulo correspondente
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
// Fun��o principal
//
main()
{
	Introducao();
	
	// Cria a lista com o nome de Cupom. Ela ser� usada durante todo o programa
	CupomFiscal* Cupom;
	// Chama a fun��o que cria o cupom
	Cupom = CriarCupom();
	
	Menu(Cupom);
	
}
