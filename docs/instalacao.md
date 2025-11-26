# Como Rodar o Projeto

## Pré-requisitos
```bash
# Ubuntu/Debian/WSL
sudo apt-get update
sudo apt-get install build-essential flex bison python3

# macOS
brew install flex bison make python3
```

## Compilar e Executar
```bash
# 1. Clonar o repositório
git clone https://github.com/GustOki/interpretador-G16.git
cd interpretador-G16

# 2. Compilar
make

# 3. Executar um exemplo
./build/interpretador tests/valid/hello_world.lang

# 4. Rodar todos os testes
make test
```

## Comandos Úteis

| Comando | Descrição |
|---------|-----------|
| `make` | Compila o projeto |
| `make clean` | Remove arquivos gerados |
| `make test` | Executa suite de testes |
| `./build/interpretador arquivo.lang` | Executa um programa |

## Estrutura do Projeto
```
interpretador-grupo16/
├── build/              # Executável gerado
├── lexer/              # Análise léxica (Flex)
├── parser/             # Análise sintática (Bison)
├── src/                # AST, interpretador, tabela de símbolos
├── tests/              # Testes automatizados
│   ├── valid/          # Casos válidos
│   └── invalid/        # Casos com erros
└── Makefile            # Automação de build
```