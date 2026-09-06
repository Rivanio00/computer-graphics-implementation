# Computação Gráfica — Tarefas

Repositório com as tarefas da disciplina de Computação Gráfica. Cada
tarefa fica em sua própria pasta (`task01/`, `task02/`, ...),
autocontida, com seu próprio código-fonte e instruções.

## Task 01 — Esfera única via ray casting

Renderiza uma única esfera vermelha sobre fundo cinza, usando um modelo
simplificado de "janela do pintor": para cada pixel do canvas, um raio
parte do olho do observador, atravessa um plano de projeção (a janela) e
testa interseção com a esfera.

### Estrutura

- `computer-graphics-implementation/`
  - `headers/`
    - Color.h
    - Vector3.h
    - stb_image_write.h
  - `models/`
      - Color.cpp
      - Vector3.cpp
  - `output/`
    - taskXX.jpg
  - `taskXX/`
    - main.cpp

> `output/` é gerado automaticamente ao executar o projeto e não é versionado.


### Compilar e Rodar

A partir da pasta raiz do repo:

1. Para rodar uma única task   
```bash
g++ -std=c++17 -O2 -Iheaders -o task01/task01 task01/main.cpp models/Color.cpp models/Vector3.cpp && ./task01/task01
```

2. Para rodar todas é necessário permitir a execução do .sh
```bash
chmod +x run_all.sh
``` 

Após a permissão basta rodar
```bash
./run_all.sh
```

A imagem final é salva em \`/output/taskXX.jpg\` (a pasta
\`output/\` é criada automaticamente na primeira execução).

### Créditos

- Exportação da imagem final em JPEG feita com
  [stb_image_write](https://github.com/nothings/stb) (Sean Barrett,
  domínio público / MIT) — biblioteca de header único incluída em
  \`task01/headers/stb_image_write.h\`.