# Computação Gráfica — Tarefas

Repositório com as tarefas da disciplina de Computação Gráfica. Cada
tarefa fica em sua própria pasta (`task01/`, `task02/`, ...),
autocontida, com seu próprio código-fonte e instruções.

## Tarefa 01 — Esfera única via ray casting

Renderiza uma única esfera vermelha sobre fundo cinza, usando um modelo
simplificado de "janela do pintor": para cada pixel do canvas, um raio
parte do olho do observador, atravessa um plano de projeção (a janela) e
testa interseção com a esfera.

### Estrutura

\`\`\`
task01/
├── headers/
│   ├── Color.h
│   ├── Vector3.h
│   └── stb_image_write.h
├── src/
│   ├── main.cpp
│   └── models/
│       ├── Color.cpp
│       └── Vector3.cpp
└── output/              # gerado automaticamente ao rodar (não versionado)
    └── esfera_unica.jpg
\`\`\`


### Compilar e Rodar

A partir da pasta \`task01/\`:

\`\`\`bash
g++ -std=c++17 -O2 -Iheaders -o task01 src/main.cpp src/models/Color.cpp src/models/Vector3.cpp
./task01
\`\`\`

A imagem final é salva em \`task01/output/esfera_unica.jpg\` (a pasta
\`output/\` é criada automaticamente na primeira execução).

### Créditos

- Exportação da imagem final em JPEG feita com
  [stb_image_write](https://github.com/nothings/stb) (Sean Barrett,
  domínio público / MIT) — biblioteca de header único incluída em
  \`task01/headers/stb_image_write.h\`.