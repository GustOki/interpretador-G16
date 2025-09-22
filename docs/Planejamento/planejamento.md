# 🗂️ Planejamento do Projeto

Esta seção apresenta o plano de sprints para o desenvolvimento do **Interpretador**, descrevendo os objetivos, entregas, tarefas e observações de cada ciclo. O planejamento busca organizar o fluxo de trabalho, garantir a evolução incremental do sistema e facilitar a integração entre os membros da equipe.

---

## 📌 Sprint 1
**Objetivos**  
- Formar equipes e organizar ambiente (repositório, ferramentas, Flex/Bison).  
- Definir linguagem-fonte e gramática inicial.  

**Entregas**  
- Documento inicial com tokens e exemplos.  
- Protótipo de gramática para Bison.  
- Ambiente configurado (hello world Flex/Bison).  

**Tarefas**  
- Criar repositório e adicionar membros (incluir sergioaafreitas).  
- Criar `.l` e `.y` mínimos.  
- Definir sintaxe básica em `docs/especificacao_linguagem.md`.  

**Observações**  
- Daily rápida nas quartas-feiras; familiarizar-se com Git/GitHub.  

---

## 📌 Sprint 2
**Objetivos**  
- Completar análise léxica (Flex).  
- Implementar regras básicas do parser (Bison).  
- Preparar apresentação P1.  

**Entregas**  
- `.l` completo; primeiras regras em `.y`; formulário P1.  

**Tarefas**  
- Finalizar regexs no `.l` (espaços, comentários).  
- Regras sintáticas básicas em `.y`.  
- Testes léxicos/sintáticos com exemplos.  

**Observações**  
- O Sprint fecha com a apresentação do P1 (máximo de 5 minutos por equipe).  

---

## 📌 Sprint 3
**Objetivos**  
- Construir AST; iniciar análise semântica básica; melhorar mensagens de erro.  

**Entregas**  
- Estruturas/classes da AST; módulo semântico inicial; parser que constrói AST.  

**Tarefas**  
- Implementar ações no `.y` para criar nós da AST.  
- Criar tabela de símbolos; tratar erros básicos.  

**Observações**  
- Quartas-feiras dedicadas ao desenvolvimento prático e integração.  

---

## 📌 Sprint 4
**Objetivos**  
- Implementar interpretação da AST (execução direta).  
- Aprimorar a análise semântica, tratando construções mais complexas.  
- Preparar o Ponto de Controle P2.  

**Entregas**  
- Módulo interpretador capaz de executar instruções.  
- Análise semântica mais robusta.  
- Formulário de P2 preenchido e apresentação realizada.  

**Tarefas**  
- Criar a lógica de interpretação recursiva (`interpretNode()` / métodos `eval()` dos nós).  
- Consolidar estruturas de controle (`if`, `while`, etc.).  
- Testar o interpretador com programas de exemplo.  
- Preparar a apresentação P2.  

**Observações**  
- Use as aulas práticas para integrar e corrigir bugs.  
- O Sprint termina com a apresentação P2.  

---

## 📌 Sprint 5
**Objetivos**  
- Adicionar otimizações opcionais (ex.: simplificação de expressões constantes).  
- Incluir recursos adicionais (funções, arrays, strings).  
- Realizar testes integrados e preparar entrega final.  

**Entregas**  
- Interpretador ampliado, com otimizações e funcionalidades extras.  
- Testes de integração.  
- Versão final pronta para entrega via Teams.  

**Tarefas**  
- Implementar otimizações simples (constant folding, remoção de nós redundantes).  
- Estender a linguagem com novos recursos, se houver tempo.  
- Testar intensivamente e entregar o projeto final.  

**Observações**  
- Planejar a entrega para evitar imprevistos.  

---

## 📌 Sprint 6
**Objetivos**  
- Participar das entrevistas finais com o professor.  
- Corrigir pendências/bugs emergentes.  
- Concluir a documentação e encerrar a disciplina.  

**Entregas**  
- Entrevistas de entrega do projeto final.  
- Documentação completa (README, manual de uso, explicações sobre a AST e a execução).  
- Ajustes finais (caso o professor detecte problemas).  

**Tarefas**  
- Preparar-se para as entrevistas: cada membro domina parser, AST, semântica e interpretador.  
- Corrigir falhas apontadas.  
- Finalizar documentação e organizar exemplos de uso.  

**Observações**  
- Consulte o plano de ensino para verificar as datas das entrevistas.  
- Falta de comparecimento pode zerar a nota da apresentação final.  
