# Documentação Inicial — Interpretador (Grupo 16)

* **Linguagem do projeto:** C/C++
* **Grupo:** Grupo 16
* **Professor:** `sergioaafreitas` / `sergiofreitas@unb.br`
* **Objetivo geral:** projetar e implementar um interpretador para uma linguagem simples definida pelo grupo, usando Flex/Bison para a etapa léxica/sintática e Python para a implementação da AST, análise semântica e interpretador.

---

# 1. Por que usamos C/C++?

* **Protótipo rápido:** A Linguagrem C permite desenvolver e iterar rapidamente estruturas de dados (AST, tabela de símbolos) e componentes do interpretador.
* **Leitura e manutenção:** sintaxe clara e menor boilerplate facilitam o trabalho em equipe.
* **Bibliotecas e ferramentas:** opção de usar bibliotecas de parsing (ex.: PLY, lark) caso se deseje, além de utilitários para testes e logging.
* **Integração com Flex/Bison:** Flex/Bison podem gerar a etapa léxica/sintática tradicional; Python é ideal para implementar a camada de execução (AST/interpreter) sem precisar compilar todo o backend em C/C++.
* **Educação:** facilita a compreensão de conceitos como AST, análise semântica e avaliação.

---

# 2. Estrutura inicial do repositório

```
interpretador-grupo16/
├── README.md
├── LICENSE
├── Makefile
├── requirements.txt
├── .gitignore
├── docs/
│   └── especificacao_linguagem.md
├── examples/
│   ├── hello_world.lang
│   └── exemplos_varios/
├── src/
│   ├── lexer/
│   │   ├── lexer.l
│   │   └── build_lexer.sh
│   ├── parser/
│   │   ├── parser.y
│   │   └── build_parser.sh
│   ├── py/
│   │   ├── ast.py
│   │   ├── interpreter.py
│   │   ├── semantica.py
│   │   ├── runtime.py
│   │   └── utils.py
│   └── tests/
│       ├── test_lexer/
│       ├── test_parser/
│       └── test_interpreter/
└── ci/
```

**Descrição rápida dos principais arquivos**

* `lexer/lexer.l` — especificação lexical (Flex).
* `parser/parser.y` — gramática e ações iniciais (Bison).
* `src/py/ast.py` — definição das classes de nós da AST.
* `src/py/semantica.py` — verificação semântica (tabela de símbolos, checagem simples).
* `src/py/interpreter.py` — lógica para percorrer e executar a AST.
* `examples/` — exemplos de programas da linguagem.

---

# 3. Tokens, tipos e construções iniciais

## Tokens (sugestão inicial)

* `IDENT` — identificadores
* `NUMBER` — inteiros (ex.: `123`)
* `FLOAT` — números com ponto (opcional)
* `STRING` — literais (`"texto"`)
* Palavras-chave: `if`, `else`, `while`, `func`, `return`, `var`, `print`
* Operadores: `+`, `-`, `*`, `/`, `=`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `&&`, `||`
* Delimitadores: `;`, `,`, `(`, `)`, `{`, `}`

## Tipos iniciais (sugeridos)

* `int`, `float` (opcional), `bool`, `string`
* Decisão sobre tipagem: inicialmente tipagem dinâmica simples; avaliar tipagem estática mínima conforme evolução.

## Construções básicas

* Atribuição: `x = 3;`
* Condicional:

  ```lang
  if (x > 0) {
      print(x);
  } else {
      print(0);
  }
  ```
* Loop:

  ```lang
  while (i < 10) {
      i = i + 1;
  }
  ```
* Funções (opcional em fases posteriores):

  ```lang
  func soma(a, b) {
      return a + b;
  }
  ```

## Exemplo (examples/hello\_world.lang)

```lang
var x = 10;
var y = 20;
var z = x + y;
print(z);
```

---

# 4. Instalação de dependências e comandos úteis

## Flex / Bison (Debian/Ubuntu)

```bash
sudo apt-get update
sudo apt-get install flex bison
```

## Python (ambiente virtual)

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

## `requirements.txt` (sugestão)

```
pytest
mypy   # opcional
black  # opcional
```

## Makefile (exemplo)

```makefile
.PHONY: all build-lexer build-parser run test clean

build-lexer:
	flex -o src/lexer/lex.yy.c src/lexer/lexer.l

build-parser:
	bison -d -o src/parser/parser.tab.c src/parser/parser.y

run:
	python src/py/interpreter.py examples/hello_world.lang

test:
	pytest -q

clean:
	rm -f src/lexer/lex.yy.c src/parser/parser.tab.c src/parser/parser.tab.h
```

---

# 5. Fluxo de trabalho com Git

* **Repositório:** GitHub (adicionar todos os membros e o professor).
* **Branches:** `main` (estável), `dev` (integração), `feature/<nome>`.
* **Pull Requests:** revisão por pelo menos 1 membro antes de merge em `dev`.
* **Commits:** mensagens claras (`feat: adicionar lexer`; `fix: parser while`).
* **CI (opcional):** rodar `pytest` em PRs.

---

# 6. Boas práticas

* Documentar decisões em `docs/especificacao_linguagem.md`.
* Criar issues/milestones por sprint.
* Testes automatizados para lexer, parser e interpreter.
* Mensagens de erro com indicação de linha/coluna.
* Commits pequenos e frequentes; PRs revisados.

---

# 7. Responsabilidades (sugestão)

* **Líder do grupo:** (nome) — coordenação geral.
* **Lexer/Parser:** (nome) — Flex/Bison e testes.
* **AST/Semântica:** (nome) — design da AST e análise semântica.
* **Intérprete/Runtime:** (nome) — execução e testes.
* **Todos:** documentação, testes, revisão de código.

---

# 8. Observações finais

* Quartas-feiras sugeridas para daily meetings/integração.
* Mantenham commits estáveis e documentem bem a AST e decisões de design.
* Em caso de dúvidas técnicas sobre Flex/Bison ou integração com Python, documentem e criem issues para rastrear a solução.

---
