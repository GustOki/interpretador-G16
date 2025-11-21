# Documentação — Interpretador (Grupo 16)

* **Linguagem do projeto:** C
* **Grupo:** Grupo 16
* **Professor:** `sergioaafreitas` / `sergiofreitas@unb.br`

---

## 📖 Por que usamos C?

* **Protótipo rápido:** A Linguagem C permite desenvolver e iterar rapidamente estruturas de dados (AST, tabela de símbolos) e componentes do interpretador.
* **Leitura e manutenção:** Sintaxe clara e menor boilerplate facilitam o trabalho em equipe.
* **Bibliotecas e ferramentas:** Integração nativa com Flex e Bison, além de ferramentas padrão de sistema (Make, GDB).
* **Integração com Flex/Bison:** Flex/Bison geram código C otimizado para a etapa léxica e sintática.
* **Educação:** Facilita a compreensão de conceitos de baixo nível como gerenciamento de memória, ponteiros e a estrutura interna de um processo de compilação.

---

## 📋 Visão Geral

Este repositório contém a implementação de um **interpretador** para uma linguagem simples definida pelo grupo. A análise léxica e sintática é feita com **Flex** e **Bison**; a AST, análise semântica e o interpretador estão implementados em **C**.

**Pipeline de Execução:**
`Código Fonte (.lang)` → `Lexer (Tokens)` → `Parser (Gramática)` → `AST` → `Verificação Semântica` → `Interpretação/Execução`

---

## 📂 Estrutura do Repositório

```text
interpretador-grupo16/
├── build/                    # Artefatos de compilação e executável final (gerado automaticamente)
├── lexer/
│   ├── lex.yy.c              # Gerado automaticamente pelo Flex
│   └── lexer.l               # Regras léxicas (tokens)
├── parser/
│   ├── parser.tab.c          # Gerado automaticamente pelo Bison
│   ├── parser.tab.h          # Header gerado pelo Bison
│   └── parser.y              # Regras gramaticais
├── src/
│   ├── ast.c                 # Implementação da Árvore Sintática Abstrata
│   ├── ast.h                 # Definições das estruturas da AST
│   ├── interpretador.c       # Lógica de execução, avaliação e runtime
│   ├── main.c                # Ponto de entrada (leitura de arquivo)
|	├── test_runner.py        # Script Python para orquestração dos testes
│   ├── simbolo.c             # Implementação da Tabela de Símbolos
│   └── simbolo.h             # Interface da Tabela de Símbolos
├── tests/                    # Suíte de testes automatizados
│   ├── valid/                # Testes validos (devem rodar com sucesso)
│   └── invalid/              # Testes de erro (devem falhar propositalmente)
├── Makefile                  # Automação de build
└── README.md                 # Documentação
```

🛠️ Pré-requisitos
------------------

Para compilar e rodar o projeto, você precisará das seguintes ferramentas instaladas:

*   **GCC** (Compilador C)
    
*   **Flex**
    
*   **Bison**
    
*   **Python 3** (Para rodar a suíte de testes)
    
*   **Make**
    

**Instalação no Ubuntu/Debian/WSL:**


`   sudo apt-get update  sudo apt-get install build-essential flex bison python3   `

🚀 Compilação e Execução
------------------------

O projeto utiliza um Makefile para automatizar todo o processo.

### Compilar o Projeto

Gera o executável interpretador na pasta build/.

`   make   `

### Limpar Arquivos Gerados

Remove a pasta build e os arquivos C gerados pelo Flex/Bison.

`   make clean   `

### Executar um Script Manualmente

Para rodar um arquivo específico da nossa linguagem:

`   ./build/interpretador caminho/para/arquivo.c   `

🧪 Testes Automatizados
-----------------------

O projeto possui um sistema robusto de **Testes de Regressão** (test\_runner.py). Ele garante que novas funcionalidades não quebrem comportamentos existentes.

### Como Rodar os Testes

Basta executar o comando abaixo. O sistema irá compilar o projeto e rodar todos os casos de teste.

`   make test   `

### Estrutura dos Testes

*   **Pasta tests/valid:** Contém códigos que **devem funcionar**. O teste verifica se o código de saída é 0 e se a saída impressa bate com o gabarito (.out).
    
*   **Pasta tests/invalid:** Contém códigos com erros propositais (sintaxe ou semântica). O teste verifica se o interpretador **identifica o erro** e retorna falha, garantindo segurança.
    

###  Como Adicionar Novos Testes (Geração Automática)

Para adicionar um novo caso de teste válido sem precisar escrever o gabarito manualmente:

1.  Crie o arquivo .lang: Crie um novo arquivo com seu código na pasta tests/valid/ (ex: tests/valid/novo\_teste.lang).
    
2. Gere o gabarito (.out) automaticamente rodando este comando no terminal:

`for f in tests/valid/\*.lang; do ./build/interpretador "$f" > "${f%.lang}.out"; done `

    
3.  **Verifique:** Abra o arquivo .out gerado para garantir que a saída está correta conforme o esperado.
    
4.  **Rode a validação:** Execute `make test` para confirmar que tudo está verde.
    


**O executável final** ficará em `build/interpretador` (conforme `BUILD_DIR` e `TARGET`).


## Tokens, tipos e construções iniciais

**Tokens (exemplos):**
`IDENT`, `NUMBER`, `FLOAT`, `STRING`, palavras-chave (`if`, `else`, `while`, `func`, `return`, `var`, `print`), operadores (`+ - * / = == != < > <= >= && ||`), delimitadores (`; , ( ) { }`).

**Tipos iniciais (sugeridos):** `int`, `float` (opcional), `bool`, `string`.  
**Construções básicas:** atribuições, expressões aritméticas e lógicas, `if/else`, `while`, `print`. Funções podem ser adicionadas nas sprints posteriores.

---

## Testes e boas práticas

* **Unit tests:** escrever testes para AST, semântica e runtime (sugestão: `cmocka`/`CUnit`).
* **Integration tests:** criar scripts que rodem o interpretador sobre exemplos e comparem saída.
* **Mensagens de erro:** reportar `linha:coluna` e fornecer contexto.
* **Estilo:** usar `clang-format`; mantenham regras comuns no time.
* **Sanitizers:** use `-fsanitize=address,undefined` em builds de desenvolvimento para detectar vazamentos/uso inválido de memória.

---

## Erros e reporting

* **Lexer/Parser:** reportem a posição do erro e trecho de código; considerem usar a recuperação de erros do Bison para testes de robustez.
* **Semântica:** verificação de variáveis não declaradas, coerência mínima de tipos, número de parâmetros em funções (se houver).
* **Runtime:** mensagens claras para erros de execução (divisão por zero, índice inválido, etc.).

---

## Sprints / Cronograma (resumo)

As sprints previstas são:

* **Sprint 1** — formar equipes, configurar ambiente, definir linguagem e gramática inicial.
* **Sprint 2** — concluir lexer e primeiras regras do parser; preparar P1.
* **Sprint 3** — criar AST, iniciar análise semântica e melhorar mensagens de erro.
* **Sprint 4** — implementar interpretador que percorre a AST; preparar P2.
* **Sprint 5** — otimizações, recursos extras, testes integrados e entrega final.
* **Sprint 6** — entrevistas finais com o professor, correções e documentação final.

---

# Fluxo de trabalho com Git

* **Repositório:** GitHub (adicionar todos os membros e o professor).
* **Branches:** `main` (estável), `dev` (integração), `feature/<nome>`.
* **Pull Requests:** revisão por pelo menos 1 membro antes de merge em `dev`.
* **Commits:** mensagens claras (`feat: adicionar lexer`; `fix: parser while`).