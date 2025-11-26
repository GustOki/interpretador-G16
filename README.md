# 🚀 Interpretador G16

<div align="center">

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Flex](https://img.shields.io/badge/Flex-Lexer-orange?style=for-the-badge)
![Bison](https://img.shields.io/badge/Bison-Parser-yellow?style=for-the-badge)
![License](https://img.shields.io/github/license/GustOki/interpretador-G16?style=for-the-badge)

**Compiladores 1 | Universidade de Brasília | 2025.2**

[📚 Documentação Completa](https://gustoki.github.io/interpretador-G16/) • [🐛 Reportar Bug](https://github.com/GustOki/interpretador-G16/issues) • [✨ Solicitar Feature](https://github.com/GustOki/interpretador-G16/issues)

</div>

---

## 📖 Sobre o Projeto

Interpretador para uma linguagem de programação simplificada desenvolvido em **C**, utilizando **Flex** para análise léxica e **Bison** para análise sintática. Este projeto é parte da disciplina de Compiladores 1, ministrada pelo Professor Sergio Freitas na Faculdade de Ciência e Tecnologia em Engenharia (FCTE) da UnB.

### ✨ Características

- ✅ **Tipos de dados:** `int`, `float`, `char`, `string`
- ✅ **Estruturas de controle:** `if/else`, `while`, `for`, `do-while`, `switch-case`
- ✅ **Operadores:** Aritméticos, relacionais e de atribuição
- ✅ **Arrays:** Declaração, inicialização e acesso por índice
- ✅ **Sistema de testes:** 40+ casos de teste automatizados
- ✅ **Tratamento de erros:** Mensagens detalhadas de erros léxicos, sintáticos e semânticos

### 🎯 Pipeline de Execução

```
Código Fonte (.lang) → Lexer → Parser → AST → Análise Semântica → Interpretação
```

---

## 🚀 Início Rápido

### Pré-requisitos

```bash
# Ubuntu/Debian/WSL
sudo apt-get update
sudo apt-get install build-essential flex bison python3

# macOS
brew install flex bison make python3
```

### Compilação e Execução

```bash
# Clonar o repositório
git clone https://github.com/GustOki/interpretador-G16.git
cd interpretador-G16

# Compilar
make

# Executar um exemplo
./build/interpretador tests/valid/fibonacci.lang

# Rodar todos os testes
make test
```

---

## 📂 Estrutura do Projeto

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

---

## 👥 Equipe - Grupo 16

<table align="center">
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

## 📚 Documentação Completa

Para informações detalhadas sobre instalação, sintaxe da linguagem, testes e desenvolvimento, acesse nossa documentação completa:

<div align="center">

### [📖 Acessar Documentação](https://gustoki.github.io/interpretador-G16/)

[![Documentação](https://img.shields.io/badge/Documentação-Online-4285F4?style=for-the-badge&logo=googledocs&logoColor=white)](https://gustoki.github.io/interpretador-G16/)

</div>

A documentação inclui:

- 📥 **Como Rodar:** Guia completo de instalação e uso
- 💻 **Linguagem:** Sintaxe, tipos, estruturas e exemplos
- 📅 **Sprints:** Cronograma e contribuições da equipe
- 📋 **Atas:** Registro das reuniões do projeto

---

## 🛠️ Tecnologias Utilizadas

| Componente | Tecnologia |
|-----------|------------|
| **Linguagem** | C |
| **Análise Léxica** | Flex |
| **Análise Sintática** | Bison |
| **Build System** | GNU Make |
| **Testes** | Python 3 |
| **Documentação** | MkDocs Material |

---

## 📝 Licença

Este projeto está sob a licença especificada no arquivo [LICENSE](LICENSE).

---

<div align="center">

[⬆ Voltar ao topo](#-interpretador-g16)

</div>
