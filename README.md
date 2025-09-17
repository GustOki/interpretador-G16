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

# 4. Plano de Sprints

## Sprint 1

**Objetivos**

* Formar equipes e organizar ambiente (repositório, ferramentas, Flex/Bison).
* Definir linguagem-fonte e gramática inicial.

**Entregas**

* Documento inicial com tokens e exemplos.
* Protótipo de gramática para Bison.
* Ambiente configurado (hello world Flex/Bison).

**Tarefas**

* Criar repositório e adicionar membros (incluir `sergioaafreitas`).
* Criar `.l` e `.y` mínimos.
* Definir sintaxe básica em `docs/especificacao_linguagem.md`.

**Observações**

* Daily rápida nas quartas-feiras; familiarizar-se com Git/GitHub.

---

## Sprint 2

**Objetivos**

* Completar análise léxica (Flex).
* Implementar regras básicas do parser (Bison).
* Preparar apresentação P1.

**Entregas**

* `.l` completo; primeiras regras em `.y`; formulário P1.

**Tarefas**

* Finalizar regexs no `.l` (espaços, comentários).
* Regras sintáticas básicas em `.y`.
* Testes léxicos/sintáticos com exemplos.

**Observações**

* O Sprint fecha com a apresentação do P1 (máximo de 5 minutos por equipe).

---

## Sprint 3

**Objetivos**

* Construir AST; iniciar análise semântica básica; melhorar mensagens de erro.

**Entregas**

* Estruturas/classes da AST; módulo semântico inicial; parser que constrói AST.

**Tarefas**

* Implementar ações no `.y` para criar nós da AST.
* Criar tabela de símbolos; tratar erros básicos.

**Observações**

* Quartas-feiras dedicadas ao desenvolvimento prático e integração.

---

## Sprint 4

**Objetivos**

* Implementar interpretação da AST (execução direta).
* Aprimorar a análise semântica, tratando construções mais complexas.
* Preparar o Ponto de Controle P2.

**Entregas**

* Módulo interpretador capaz de executar instruções.
* Análise semântica mais robusta.
* Formulário de P2 preenchido e apresentação realizada.

**Tarefas**

* Criar a lógica de interpretação recursiva (`interpretNode()` / métodos `eval()` dos nós).
* Consolidar estruturas de controle (if, while, etc.).
* Testar o interpretador com programas de exemplo.
* Preparar a apresentação P2.

**Observações**

* Use as aulas práticas para integrar e corrigir bugs.
* O Sprint termina com a apresentação P2.

---

## Sprint 5

**Objetivos**

* Adicionar otimizações opcionais (ex.: simplificação de expressões constantes).
* Incluir recursos adicionais (funções, arrays, strings).
* Realizar testes integrados e preparar entrega final.

**Entregas**

* Interpretador ampliado, com otimizações e funcionalidades extras.
* Testes de integração.
* Versão final pronta para entrega via Teams.

**Tarefas**

* Implementar otimizações simples (constant folding, remoção de nós redundantes).
* Estender a linguagem com novos recursos, se houver tempo.
* Testar intensivamente e entregar o projeto final.

**Observações**

* Planejar a entrega para evitar imprevistos.

---

## Sprint 6

**Objetivos**

* Participar das entrevistas finais com o professor.
* Corrigir pendências/bugs emergentes.
* Concluir a documentação e encerrar a disciplina.

**Entregas**

* Entrevistas de entrega do projeto final.
* Documentação completa (README, manual de uso, explicações sobre a AST e a execução).
* Ajustes finais (caso o professor detecte problemas).

**Tarefas**

* Preparar-se para as entrevistas: cada membro domina parser, AST, semântica e interpretador.
* Corrigir falhas apontadas.
* Finalizar documentação e organizar exemplos de uso.

**Observações**

* Consulte o plano de ensino para verificar as datas das entrevistas. Falta de comparecimento pode zerar a nota da apresentação final.

---

# 5. Instalação de dependências e comandos úteis

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

# 6. Fluxo de trabalho com Git

* **Repositório:** GitHub (adicionar todos os membros e o professor).
* **Branches:** `main` (estável), `dev` (integração), `feature/<nome>`.
* **Pull Requests:** revisão por pelo menos 1 membro antes de merge em `dev`.
* **Commits:** mensagens claras (`feat: adicionar lexer`; `fix: parser while`).
* **CI (opcional):** rodar `pytest` em PRs.

---

# 7. Boas práticas

* Documentar decisões em `docs/especificacao_linguagem.md`.
* Criar issues/milestones por sprint.
* Testes automatizados para lexer, parser e interpreter.
* Mensagens de erro com indicação de linha/coluna.
* Commits pequenos e frequentes; PRs revisados.

---

# 8. Próximos passos imediatos (Sprint 1 — checklist)

1. Criar repositório e adicionar membros (incluir `sergioaafreitas`).
2. Configurar `.gitignore` e `README.md` inicial.
3. Instalar Flex/Bison e testar localmente.
4. Escrever `.l` mínimo que reconheça IDENT, NUMBER e símbolos; testar com `flex`.
5. Escrever `.y` mínimo para expressões e `print`; testar com `bison`.
6. Definir e documentar a sintaxe básica em `docs/especificacao_linguagem.md`.
7. Agendar reuniões rápidas (quartas-feiras) e criar issues iniciais.

---

# 9. Responsabilidades (sugestão)

* **Líder do grupo:** (nome) — coordenação geral.
* **Lexer/Parser:** (nome) — Flex/Bison e testes.
* **AST/Semântica:** (nome) — design da AST e análise semântica.
* **Intérprete/Runtime:** (nome) — execução e testes.
* **Todos:** documentação, testes, revisão de código.

---

# 10. Observações finais

* Quartas-feiras sugeridas para daily meetings/integração.
* Mantenham commits estáveis e documentem bem a AST e decisões de design.
* Em caso de dúvidas técnicas sobre Flex/Bison ou integração com Python, documentem e criem issues para rastrear a solução.

---

## Anexos possíveis (se desejar que eu gere)

* `README.md` pronto para o repositório.
* Esqueleto `lexer.l` e `parser.y` para começar.
* Esqueleto `ast.py` e `interpreter.py` em Python.

> Se quiser, eu já gero qualquer um dos anexos acima em Markdown/código — diga qual você quer agora.
