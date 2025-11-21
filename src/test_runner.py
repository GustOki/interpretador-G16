import os
import subprocess
import sys
import difflib

# --- CONFIGURAÇÕES ---
INTERPRETER_PATH = "./build/interpretador"
TESTS_DIR = "tests"
VALID_DIR = os.path.join(TESTS_DIR, "valid")
INVALID_DIR = os.path.join(TESTS_DIR, "invalid")

# Cores para o terminal
GREEN = "\033[92m"
RED = "\033[91m"
RESET = "\033[0m"
CYAN = "\033[96m"

def print_diff(expected, actual):
    diff = difflib.unified_diff(
        expected.splitlines(),
        actual.splitlines(),
        fromfile='Esperado',
        tofile='Recebido',
        lineterm=''
    )
    for line in diff:
        print(line)

def run_valid_tests():
    print(f"{CYAN}=== Rodando Testes Válidos ==={RESET}")
    files = sorted([f for f in os.listdir(VALID_DIR) if f.endswith(".lang")])
    passed = 0
    total = 0

    for filename in files:
        total += 1
        filepath = os.path.join(VALID_DIR, filename)
        expected_out_path = filepath.replace(".lang", ".out")

        # Verifica se existe o gabarito (.out)
        if not os.path.exists(expected_out_path):
            print(f"[SKIP] {filename}: Arquivo .out não encontrado. (Gere-o primeiro!)")
            continue

        # Roda o interpretador
        try:
            result = subprocess.run(
                [INTERPRETER_PATH, filepath],
                capture_output=True,
                text=True,
                timeout=2
            )
            
            actual_output = result.stdout.strip()
            
            with open(expected_out_path, "r") as f:
                expected_output = f.read().strip()

            # Verifica o código de saída (deve ser 0 para sucesso)
            if result.returncode != 0:
                print(f"{RED}[FAIL] {filename}{RESET}: Erro de execução (Código {result.returncode})")
                print(result.stderr)
                continue

            # Verifica a saída do texto
            if actual_output == expected_output:
                print(f"{GREEN}[PASS] {filename}{RESET}")
                passed += 1
            else:
                print(f"{RED}[FAIL] {filename}{RESET}: Saída incorreta")
                print_diff(expected_output, actual_output)

        except subprocess.TimeoutExpired:
            print(f"{RED}[FAIL] {filename}{RESET}: Timeout (Loop infinito?)")
        except Exception as e:
            print(f"{RED}[ERR] {filename}{RESET}: {e}")

    return passed, total

def run_invalid_tests():
    print(f"\n{CYAN}=== Rodando Testes Inválidos ==={RESET}")
    if not os.path.exists(INVALID_DIR):
        print("Pasta 'invalid' não encontrada.")
        return 0, 0

    files = sorted([f for f in os.listdir(INVALID_DIR) if f.endswith(".lang")])
    passed = 0
    total = 0

    for filename in files:
        total += 1
        filepath = os.path.join(INVALID_DIR, filename)

        try:
            result = subprocess.run(
                [INTERPRETER_PATH, filepath],
                capture_output=True,
                text=True,
                timeout=2
            )

            # Aqui o sucesso é FALHAR (código diferente de 0)
            # Ou imprimir "Erro" na saída de erro
            if result.returncode != 0 or "Erro" in result.stderr:
                print(f"{GREEN}[PASS] {filename}{RESET}")
                passed += 1
            else:
                print(f"{RED}[FAIL] {filename}{RESET}: Deveria ter falhado, mas rodou com sucesso!")

        except Exception as e:
            print(f"{RED}[ERR] {filename}{RESET}: {e}")

    return passed, total

if __name__ == "__main__":
    if not os.path.exists(INTERPRETER_PATH):
        print("Erro: Compile o projeto antes de testar!")
        sys.exit(1)

    p_valid, t_valid = run_valid_tests()
    p_invalid, t_invalid = run_invalid_tests()

    print("\n" + "="*30)
    print(f"RESUMO: {p_valid + p_invalid} de {t_valid + t_invalid} testes passaram.")
    print("="*30)

    if (p_valid + p_invalid) < (t_valid + t_invalid):
        sys.exit(1) # Falha no CI/Make