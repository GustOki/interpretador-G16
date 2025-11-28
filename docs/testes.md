## 🧪 Sistema de Testes Automatizados

O projeto possui um sistema robusto de **testes de regressão** que garante a qualidade do código e previne bugs em novas funcionalidades.

### Como Rodar os Testes

Execute o comando abaixo para compilar o projeto e rodar todos os testes automaticamente:

```bash
make test
```

O sistema de testes:

- ✅ Compila o interpretador automaticamente
- ✅ Executa todos os casos de teste válidos e inválidos
- ✅ Compara saídas com gabaritos esperados
- ✅ Reporta falhas com detalhes claros
- ✅ Fornece estatísticas de aprovação/falha

### Estrutura e Tipos de Testes

#### 1. **Testes Válidos (`tests/valid/`)**

Contêm programas que **devem executar com sucesso**. O sistema verifica:

- ✅ **Código de saída 0** (execução sem erros)
- ✅ **Saída padrão** corresponde exatamente ao gabarito (`.out`)

**Exemplo de teste válido:**
```
tests/valid/
├── hello_world.lang        # Código fonte do teste
├── hello_world.out         # Saída esperada (gabarito)
├── arithmetic.lang
├── arithmetic.out
├── loops.lang
└── loops.out
```

**Critérios de aprovação:**

- Interpretador retorna exit code 0
- Saída impressa em `stdout` é idêntica ao arquivo `.out` correspondente
- Nenhum erro de segmentação ou vazamento de memória

#### 2. **Testes Inválidos (`tests/invalid/`)**

Contêm programas com **erros propositais** (sintáticos ou semânticos). O sistema verifica:

- ✅ **Código de saída ≠ 0** (interpretador detectou o erro)
- ✅ **Mensagens de erro** são emitidas apropriadamente

**Exemplo de teste inválido:**
```
tests/invalid/
├── syntax_error.lang       # Erro de sintaxe
├── undefined_var.lang      # Variável não declarada
├── type_mismatch.lang      # Erro de tipo
└── division_by_zero.lang   # Erro de runtime
```

**Critérios de aprovação:**

- Interpretador retorna exit code diferente de 0
- Mensagem de erro clara é emitida
- Programa não trava ou causa comportamento indefinido

### Como Adicionar Novos Testes

#### Método 1: Geração Automática de Gabarito (Recomendado)

Para testes válidos, você pode gerar o gabarito automaticamente:

1. **Crie o arquivo `.lang`:**
   ```bash
   nano tests/valid/novo_teste.lang
   ```

2. **Gere o gabarito automaticamente:**
   ```bash
   for f in tests/valid/*.lang; do 
       ./build/interpretador "$f" > "${f%.lang}.out"
   done
   ```

3. **Verifique o gabarito gerado:**
   ```bash
   cat tests/valid/novo_teste.out
   ```

4. **Valide o teste:**
   ```bash
   make test
   ```

#### Método 2: Criação Manual do Gabarito

1. **Crie o arquivo de teste:**
   ```bash
   nano tests/valid/novo_teste.lang
   ```

2. **Execute manualmente e verifique:**
   ```bash
   ./build/interpretador tests/valid/novo_teste.lang
   ```

3. **Crie o gabarito com a saída esperada:**
   ```bash
   echo "saída esperada aqui" > tests/valid/novo_teste.out
   ```

#### Adicionando Testes de Erro

Para testes inválidos (não precisam de gabarito `.out`):

```bash
nano tests/invalid/novo_erro.lang
# Escreva código com erro proposital
make test  # Verifique que o interpretador detecta o erro
```

### Exemplos de Casos de Teste

#### Teste Válido - Contador Simples
```c
// tests/valid/contador.lang
int i;
for (i = 0; i < 5; i = i + 1) {
    printf("Contador: %d\n", i);
}
```

#### Teste Válido - Cálculo de Média
```c
// tests/valid/media.lang
float nota1 = 8.5;
float nota2 = 7.0;
float nota3 = 9.5;
float media = (nota1 + nota2 + nota3) / 3.0;
printf("Média: %f\n", media);
```

#### Teste Válido - Switch-Case
```c
// tests/valid/switch_test.lang
int dia = 3;
switch (dia) {
    case 1:
        printf("Segunda-feira\n");
        break;
    case 2:
        printf("Terça-feira\n");
        break;
    case 3:
        printf("Quarta-feira\n");
        break;
    default:
        printf("Outro dia\n");
        break;
}
```

#### Teste Inválido - Variável Não Declarada
```c
// tests/invalid/undefined_var.lang
printf("%d\n", x);  // Erro: x não foi declarada
```

#### Teste Inválido - Erro de Sintaxe
```c
// tests/invalid/syntax_error.lang
int x = 10  // Erro: faltando ponto e vírgula
printf("%d\n", x);
```

### Saída do Sistema de Testes

```
=================================
Executando Testes Automatizados
=================================

[VALID] hello_world.lang .................. ✓ PASS
[VALID] arithmetic.lang ................... ✓ PASS
[VALID] loops.lang ........................ ✓ PASS
[INVALID] syntax_error.lang ............... ✓ PASS (erro detectado)
[INVALID] undefined_var.lang .............. ✓ PASS (erro detectado)

=================================
Resumo: 5/5 testes aprovados (100%)
=================================
```