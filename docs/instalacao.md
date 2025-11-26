# Como Rodar o Projeto

## 🛠️ Pré-requisitos

Para compilar e rodar o projeto, você precisará das seguintes ferramentas instaladas:

* **GCC** (Compilador C - versão 7.0 ou superior recomendada)
* **Flex** (Fast Lexical Analyzer - versão 2.6+)
* **Bison** (GNU Parser Generator - versão 3.0+)
* **Python 3** (Para rodar a suíte de testes - versão 3.6+)
* **Make** (GNU Make para automação de build)

**Instalação no Ubuntu/Debian/WSL:**

```bash
sudo apt-get update
sudo apt-get install build-essential flex bison python3
```

**Instalação no macOS:**

```bash
brew install flex bison make python3
```

**Verificação da instalação:**

```bash
gcc --version
flex --version
bison --version
python3 --version
make --version
```

---

## 🚀 Compilação e Execução

O projeto utiliza um Makefile para automatizar todo o processo de build.

### Compilar o Projeto

Gera o executável `interpretador` na pasta `build/`:

```bash
make
```

O processo de compilação executa automaticamente:
1. Geração do lexer (Flex): `lexer/lex.yy.c`
2. Geração do parser (Bison): `parser/parser.tab.c` e `parser/parser.tab.h`
3. Compilação de todos os arquivos `.c` em `src/`
4. Linkagem final para produzir `build/interpretador`

### Limpar Arquivos Gerados

Remove a pasta `build/` e todos os arquivos gerados automaticamente:

```bash
make clean
```

### Executar um Script Manualmente

Para interpretar um arquivo específico da nossa linguagem:

```bash
./build/interpretador caminho/para/arquivo.lang
```

**Exemplo:**
```bash
./build/interpretador tests/valid/fibonacci.lang
```

### Recompilar Tudo do Zero

Para garantir uma compilação limpa:

```bash
make clean && make
```

---

## Estrutura do Projeto

```text
interpretador-grupo16/
├── build/                    # Artefatos de compilação e executável final (gerado automaticamente)
│   └── interpretador         # Executável principal (gerado pelo make)
├── lexer/
│   ├── lex.yy.c              # Código C gerado automaticamente pelo Flex
│   └── lexer.l               # Especificação léxica (definição de tokens e regras)
├── parser/
│   ├── parser.tab.c          # Código C gerado automaticamente pelo Bison
│   ├── parser.tab.h          # Header com definições de tokens e tipos (gerado pelo Bison)
│   └── parser.y              # Especificação gramatical (regras de produção)
├── src/
│   ├── ast.c                 # Implementação da Árvore Sintática Abstrata
│   ├── ast.h                 # Estruturas de dados e interface da AST
│   ├── interpretador.c       # Motor de execução: avaliação de expressões e execução de comandos
│   ├── main.c                # Ponto de entrada: leitura de arquivo, inicialização e coordenação
│   ├── simbolo.c             # Implementação da Tabela de Símbolos (escopos e variáveis)
│   ├── simbolo.h             # Interface da Tabela de Símbolos
|   └── test_runner.py        # Script Python para orquestração e execução automatizada dos testes
├── tests/                    # Suíte completa de testes automatizados
│   ├── valid/                # Casos de teste válidos (devem executar com sucesso)
│   │   ├── *.lang            # Código fonte dos testes
│   │   └── *.out             # Saída esperada (gabarito) para cada teste
│   └── invalid/              # Casos de teste com erros (devem falhar propositalmente)
│       └── *.lang            # Código fonte com erros sintáticos ou semânticos
├── Makefile                  # Automação completa de build, limpeza e testes
└── README.md                 # Documentação do projeto
```

---


## 🔄 Fluxo de Trabalho com Git

### Estrutura de Branches

* **`main`**: Branch estável, apenas código testado e aprovado
* **`dev`**: Branch de integração para desenvolvimento ativo
* **`feature/<nome>`**: Branches para novas funcionalidades individuais
* **`fix/<nome>`**: Branches para correção de bugs

### Processo de Desenvolvimento

1. **Criar feature branch:**
   ```bash
   git checkout dev
   git pull origin dev
   git checkout -b feature/nova-funcionalidade
   ```

2. **Desenvolver e commitar:**
   ```bash
   git add .
   git commit -m "feat: implementar nova funcionalidade"
   ```

3. **Abrir Pull Request:**
   - Requer revisão de pelo menos 1 membro
   - Todos os testes devem passar
   - Código deve seguir padrões de estilo

4. **Merge em `dev`:**
   ```bash
   git checkout dev
   git merge feature/nova-funcionalidade
   ```

### Convenção de Commits

Seguir o padrão [Conventional Commits](https://www.conventionalcommits.org/):

* `feat:` Nova funcionalidade
* `fix:` Correção de bug
* `docs:` Alterações em documentação
* `test:` Adição ou modificação de testes
* `refactor:` Refatoração de código
* `style:` Formatação, espaços em branco
* `chore:` Tarefas de manutenção

**Exemplos:**
```bash
git commit -m "feat: adicionar suporte a arrays"
git commit -m "fix: corrigir vazamento de memória no parser"
git commit -m "test: adicionar casos de teste para loops"
git commit -m "docs: atualizar README com exemplos de uso"
```
