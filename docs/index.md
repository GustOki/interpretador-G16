# 🚀 Interpretador G16

## Compiladores 1 | UnB 2025.2

**Universidade de Brasília (UnB)**  
Faculdade de Ciência e Tecnologia em Engenharia (FCTE)  
**Professor:** Sergio Freitas

---

## 👥 Equipe - Grupo 16

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/daramariabs">
        <img src="https://github.com/daramariabs.png" width="120px;" alt="Dara Maria"/><br>
        <b>Dara Maria</b>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/davirnunes">
        <img src="https://github.com/davirnunes.png" width="120px;" alt="Davi Nunes"/><br>
        <b>Davi Nunes</b>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/GustOki">
        <img src="https://github.com/GustOki.png" width="120px;" alt="Gustavo Oki"/><br>
        <b>Gustavo Oki</b>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/marcosfilhopq">
        <img src="https://github.com/marcosfilhopq.png" width="120px;" alt="Marcos Filho"/><br>
        <b>Marcos Filho</b>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/SamaraAlvess">
        <img src="https://github.com/SamaraAlvess.png" width="120px;" alt="Samara Alves"/><br>
        <b>Samara Alves</b>
      </a>
    </td>
  </tr>
</table>

---

## 💡 Sobre o Projeto

Interpretador para uma linguagem de programação simplificada desenvolvido em **C**, aplicando conceitos de Compiladores 1 através da metodologia **Problem Based Learning (PBL)**.

**Pipeline de Execução:**

```mermaid
graph LR
    A[📄 Código .lang] --> B[🔍 Lexer]
    B --> C[📋 Parser]
    C --> D[🌳 AST]
    D --> E[✅ Semântica]
    E --> F[⚡ Execução]
    
    style A fill:#e3f2fd,stroke:#1976d2,stroke-width:2px
    style B fill:#fff3e0,stroke:#f57c00,stroke-width:2px
    style C fill:#fff3e0,stroke:#f57c00,stroke-width:2px
    style D fill:#e8f5e9,stroke:#388e3c,stroke-width:2px
    style E fill:#f3e5f5,stroke:#7b1fa2,stroke-width:2px
    style F fill:#fce4ec,stroke:#c2185b,stroke-width:2px
```

---

## 🚀 Início Rápido

```bash
# Clonar o repositório
git clone https://github.com/GustOki/interpretador-G16.git
cd interpretador-G16

# Compilar
make

# Executar exemplo
./build/interpretador tests/valid/hello_world.lang
```

---

## 📚 Documentação

| 📖 Página | 📝 Conteúdo |
|:----------|:-----------|
| **[📥 Como Rodar](instalacao.md)** | Instalação, compilação e comandos úteis |
| **[💻 Linguagem](linguagem.md)** | Sintaxe, exemplos e recursos suportados |
| **[📅 Sprints](sprints.md)** | Cronograma e progresso do projeto |
| **[📋 Atas](atas.md)** | Registro das reuniões da equipe |

---

## 🛠️ Stack Tecnológico

| Componente | Tecnologia |
|:-----------|:-----------|
| **Análise Léxica** | Flex (Fast Lexical Analyzer) |
| **Análise Sintática** | Bison (GNU Parser Generator) |
| **Runtime** | C (AST, Tabela de Símbolos, Interpretador) |
| **Build System** | GNU Make |
| **Testes** | Python 3 |

---

## ✨ Recursos Implementados

| Categoria | Recursos |
|:----------|:---------|
| **Tipos de Dados** | `int` · `float` · `char` · `string` |
| **Estruturas de Controle** | `if/else` · `while` · `for` · `do-while` · `switch-case` |
| **Operadores** | Aritméticos (`+` `-` `*` `/`) · Relacionais (`==` `!=` `<` `>` `<=` `>=`) |
| **Recursos Avançados** | Arrays · Indexação · Sistema de Testes · Tratamento de Erros |


---

## 🔗 Links Úteis

[![GitHub](https://img.shields.io/badge/GitHub-Repositório-181717?style=for-the-badge&logo=github)](https://github.com/GustOki/interpretador-G16)
[![Docs](https://img.shields.io/badge/Documentação-Completa-4285F4?style=for-the-badge&logo=readthedocs&logoColor=white)](instalacao.md)

---

!!! info "Projeto Educacional"
    📄 Projeto desenvolvido para fins educacionais - UnB 2025.2