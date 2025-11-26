# A Linguagem

## Tipos de Dados
```c
int x = 42;              // Inteiro
float pi = 3.14;         // Ponto flutuante
char letra = 'A';        // Caractere
string nome = "Maria";   // String
```

## Estruturas de Controle

### If/Else
```c
if (x > 0) {
    printf("Positivo");
} else {
    printf("Negativo");
}
```

### Loops
```c
// While
while (i < 10) {
    printf("%d\n", i);
    i = i + 1;
}

// For
for (i = 0; i < 10; i = i + 1) {
    printf("%d\n", i);
}

// Do-While
do {
    printf("%d\n", i);
    i = i + 1;
} while (i < 10);
```

### Switch-Case
```c
switch (opcao) {
    case 1:
        printf("Opção 1");
        break;
    case 2:
        printf("Opção 2");
        break;
    default:
        printf("Inválida");
}
```

## Operadores

| Tipo | Operadores |
|------|------------|
| Aritméticos | `+`, `-`, `*`, `/` |
| Relacionais | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| Atribuição | `=` |

## Exemplos Completos

### Fibonacci
```c
int n = 10;
int a = 0;
int b = 1;
int i;

for (i = 0; i < n; i = i + 1) {
    printf("%d\n", a);
    int temp = a + b;
    a = b;
    b = temp;
}
```

### Fatorial
```c
int n = 5;
int fat = 1;
int i;

for (i = 1; i <= n; i = i + 1) {
    fat = fat * i;
}
printf("Fatorial: %d\n", fat);
```

[← Voltar](index.md)