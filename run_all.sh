# Compila e roda TODAS as tarefas (task01, task02, ...) a partir da raiz do repositorio
set -e

for pasta in task*/; do
    task="${pasta%/}"   # remove a barra final (task01/ -> task01)

    echo "==> Compilando $task"
    g++ -std=c++17 -O2 -Iheaders -o "$task/$task" "$task/main.cpp" models/Color.cpp models/Vector3.cpp

    echo "==> Rodando $task"
    "./$task/$task"
    echo ""
done