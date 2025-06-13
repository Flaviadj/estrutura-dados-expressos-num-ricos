Panorama Geral da Solução
Para atingir o objetivo, vamos seguir os seguintes passos:

Estrutura do Projeto: Conforme solicitado, o nosso código será dividido em três arquivos para garantir a modularidade e organização:

expressao.h: O arquivo de cabeçalho (header), que já nos foi fornecido e não pode ser alterado. Ele define a estrutura Expressao e os protótipos das funções que vamos implementar.
expressao.c: Onde vamos escrever a lógica principal do nosso programa. Aqui ficarão as implementações das funções de conversão e avaliação das expressões.
main.c: O arquivo principal que irá interagir com o utilizador, pedir a expressão, chamar as funções de expressao.c para processá-la e exibir os resultados.
Estrutura de Dados Fundamental: A pilha será a nossa principal ferramenta. É a estrutura de dados ideal para resolver este tipo de problema, pois a sua natureza de "último a entrar, primeiro a sair" (LIFO) é perfeita para lidar com a ordem das operações e conversões de notação.

Funcionalidades:

Conversão Infixa -> Pós-fixada: Implementaremos a função getFormaPosFixa.
Conversão Pós-fixada -> Infixa: Implementaremos a função getFormaInFixa.
Avaliação de Expressões: Criaremos a função getValorPosFixa para calcular o resultado de uma expressão pós-fixada. A avaliação da expressão infixa (getValorInFixa ) será feita convertendo-a primeiro para pós-fixada e depois calculando o valor.

Operadores Suportados:
Básicos: +, -, *, /, %.
Especiais (um operando): raiz (raiz quadrada), sen, cos, tg (seno, cosseno, tangente) e log (logaritmo na base 10). Os ângulos para as funções trigonométricas serão em graus.

Instruções de Implementação
Para que tudo funcione, segue o passo a passo:

Cria os Arquivos: No teu ambiente de desenvolvimento C (pode ser Replit, VS Code com um compilador C, Dev-C++, etc.), cria três arquivos na mesma pasta com os nomes exatos:

expressao.h
expressao.c
main.c
Copia e Cola o Código: Copia o conteúdo de cada bloco de código que eu forneci acima para o seu arquivo correspondente.

Compila o Programa: Abre um terminal ou prompt de comando na pasta onde salvaste os arquivos. Utiliza o compilador GCC, como solicitado no documento, com o seguinte comando:

Bash

*gcc expressao.c main.c -o expressao.exe -lm*

*gcc: Chama o compilador.*

expressao.c main.c: Indica os arquivos-fonte que serão compilados.
-o expressao.exe: Define o nome do arquivo executável de saída como expressao.exe.
-lm: Esta é uma parte muito importante. Ela "linka" a biblioteca matemática (math.h) para que possamos usar funções como sqrt, sin, cos, log10, etc. Sem isso, terás erros de compilação.
Executa o Programa: Após a compilação bem-sucedida, um arquivo chamado expressao.exe (ou expressao em Linux/macOS) será criado. Para executá-lo, digita no terminal:

No Windows: .\expressao.exe ou simplesmente expressao
No Linux/macOS: ./expressao
O programa irá iniciar e mostrar o menu que criámos. Podes agora testar as expressões do PDF ou as tuas próprias!



