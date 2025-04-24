# Pixelmon Quest

Projeto desenvolvido para a disciplina de **Programação Imperativa Funcional (PIF)**. Trata-se de um jogo inspirado em *Pokémon Ruby*, com mecânicas simplificadas e foco no uso de estruturas de dados em C.

## 👾 Sobre o Projeto

O jogo permite ao jogador explorar um mundo com batalhas contra treinadores, ginásios e uma versão reduzida da Elite 4. Conta com 25 Pokémon das 3 primeiras gerações, incluindo evolução por nível, troca e uso de itens.

### Principais Funcionalidades

- Sistema de batalhas com turnos
- Dois ginásios com líderes distintos
- Batalha contra a Elite 4
- Pokédex com os 25 Pokémon suportados
- Evoluções condicionais (nível)
- Sistema de salvamento e carregamento de progresso
- Gráficos 2D utilizando a biblioteca Raylib

## 🧠 Conteúdos Aplicados

- Structs em C
- Matrizes e listas encadeadas
- Alocação dinâmica de memória
- Manipulação de arquivos (`save.dat`, `pokedex.dat`)
- Organização modular de código
- Animações e controle de sprites com Raylib

## 🖼️ Inspiração Visual

Imagens e GIFs retirados de *Pokémon Ruby & Sapphire*, que serviram como base visual para nosso projeto:

### Exploração do mapa
![Exploração](https://media4.giphy.com/media/v1.Y2lkPTc5MGI3NjExM3czaWZ0d255OWdibGszZzRrbDZyd3NrcnowZWhlc3ZzczVhczA1ZyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/QptWwfIt1yAl1NK5bu/giphy.gif)

### Batalha Pokémon
![Batalha](https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExbDQ5djhxNTR4aXoxZXd2NmRhYnE1Nm5ydXk3cTZoNjF0NmxoMmZqbSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/yYpFkCQGxS75CMdugc/giphy.gif)

### Centro Pokémon
![Centro Pokémon](https://media4.giphy.com/media/v1.Y2lkPTc5MGI3NjExbXZsYWV1a2drbmplZXpqYW1tdHo3a3hsYTI2djBvMGJ2aGFydGVqMyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/1fjD2xCMVTtQ63MFv8/giphy.gif)

## 🗂️ Estrutura de Arquivos

```
pixelmon-quest/
│
├── assets/                 # Imagens, sprites, sons, fontes, etc.
│   └── sprites/
│       ├── data/
│       │   └── player.json
│       ├── player_sheet/
│       │   ├── player_walk_down1.png
│       │   ├── ...
│       └── pokemons/
│           ├── treecko/
│           │   ├── treecko.json
│           │   ├── treecko_front.png
│           │   └── treecko_back.png
│           ├── torchic/
│           │   └── ...
│           └── ...
│   ├── tiles/
│   ├── audio/
│   └── fonts/
│
├── include/                # Arquivos de cabeçalho (.h)
│   ├── game.h              # Funções e structs gerais do jogo
│   ├── pokemon.h           # Struct e funções dos Pokémons
│   ├── player.h            # Struct do jogador, party, box
│   ├── battle.h            # Lógica de batalhas
│   ├── pokedex.h           # Funções para pokédex
│   ├── save.h              # Leitura e escrita de arquivos
│   ├── menu.h              # Menus e UI
│   └── map.h               # Mapas e movimentação
│
├── src/                    # Código-fonte (.c)
│   ├── main.c              # Ponto de entrada
│   ├── game.c              # Controle do loop e cenas
│   ├── pokemon.c           # Manipulação de Pokémon
│   ├── player.c            # Player e party
│   ├── battle.c            # Sistema de batalha
│   ├── pokedex.c           # Cadastro e visualização de pokémons
│   ├── save.c              # Load/save do jogo
│   ├── menu.c              # Menus e interfaces
│   └── map.c               # Mapa, movimentação, transições
│
├── data/                   # Arquivos persistentes
│   ├── save.dat            # Save do jogo
│   └── pokedex.dat         # Pokédex preenchida
│
├── Makefile                # Automação de build
└── README.md               # Explicação do projeto
```

## 🛠️ Como Compilar

Certifique-se de ter a [Raylib](https://www.raylib.com/) instalada.

```bash
make
./pixelmon-quest
```

## 👨‍🎓 Equipe 
- Ricardo Freitas
- Gabriel Aniceto
- Thiago Medeiros

## 📜 Licença
Projeto acadêmico sem fins lucrativos. Alguns sprites utilizados pertencem à franquia Pokémon (Nintendo/Game Freak). Todos os créditos de imagem são devidos aos criadores originais. Uso apenas educacional.
