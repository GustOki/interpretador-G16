# Documentação Inicial — Interpretador (Grupo 16)

* **Linguagem do projeto:** C
* **Grupo:** Grupo 16
* **Professor:** `sergioaafreitas` / `sergiofreitas@unb.br`

---

# Por que usamos C?

* **Protótipo rápido:** A Linguagrem C permite desenvolver e iterar rapidamente estruturas de dados (AST, tabela de símbolos) e componentes do interpretador.
* **Leitura e manutenção:** sintaxe clara e menor boilerplate facilitam o trabalho em equipe.
* **Bibliotecas e ferramentas:** opção de usar bibliotecas de parsing (ex.: PLY, lark) caso se deseje, além de utilitários para testes e logging.
* **Integração com Flex/Bison:** Flex/Bison podem gerar a etapa léxica/sintática tradicional; Python é ideal para implementar a camada de execução (AST/interpreter) sem precisar compilar todo o backend em C.
* **Educação:** facilita a compreensão de conceitos como AST, análise semântica e avaliação.


## Visão Geral

Este repositório contém a implementação de um **interpretador** para uma linguagem simples definida pelo grupo. A análise léxica e sintática é feita com **Flex** e **Bison**; a AST, análise semântica e o interpretador estão implementados em **C**.

Pipeline alvo:
`arquivo fonte → lexer (Flex) → parser (Bison) → AST → checagem semântica → interpretação/execução`.

---

## Estrutura do repositório (atual)

```
interpretador-grupo16/
├── build/                    # artefatos de build (opcional)
├── lexer/
│   ├── lex.yy.c              # gerado (Flex)
│   └── lexer.l
├── parser/
│   ├── parser.tab.c          # gerado (Bison)
│   ├── parser.tab.h          # gerado (Bison)
│   └── parser.y
├── src/
│   ├── ast.c
│   ├── ast.h
│   ├── interpretador.c
│   ├── main.c
│   └── simbolo.h
├── Makefile
└── README.md
```

---

<<<<<<< HEAD
# 4. Instalação de dependências e comandos úteis

## Flex / Bison (Debian/Ubuntu)
=======
## Pré-requisitos

No Ubuntu/Debian:
>>>>>>> bf5fc69a41e78f35a0ce7c9c7a6a8dab98079e0f

```bash
sudo apt-get update
sudo apt-get install build-essential flex bison
```

Opcional (testes/unit):

```bash
sudo apt-get install cmocka
```

Recomendações de desenvolvimento:

* `gcc` ou `clang`
* `clang-format` (padronização de estilo)
* usar sanitizers em builds de debug: `-fsanitize=address,undefined`

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

# 7. Observações finais

* Quartas-feiras sugeridas para daily meetings/integração.
* Mantenham commits estáveis e documentem bem a AST e decisões de design.
* Em caso de dúvidas técnicas sobre Flex/Bison ou integração com Python, documentem e criem issues para rastrear a solução.

---
