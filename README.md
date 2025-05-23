# 🦖 Raptor Run (Jogo no Terminal)

Um jogo estilo Dino Run (inspirado no Dino do Google Chrome) desenvolvido em linguagem C, rodando inteiramente no terminal com animações, pulo, obstáculos, colisões e sistema de pontuação com ranking ordenado.


## 🎮 Funcionalidades

- Dinossauro controlável que pula obstáculos 🌵
- Sistema de vidas (3 chances)
- Score baseado em obstáculos desviados
- Sistema de ranking com gravação dos melhores jogadores
- Menu com opções de iniciar, ver placar e sair
- Tela de game over com arte ASCII


## 🛠️ Tecnologias e Bibliotecas

- Linguagem: **C**
- Bibliotecas externas:
  - `screen.h`, `keyboard.h`, `timer.h` (para controle de tela, tempo e teclado em tempo real)
- Terminal colorido com uso de ANSI escape codes
- Controle de tempo com `timerInit()` (frames de 100ms)


## ⚙️ Como rodar:
1º) Clone o repositório

```
git clone https://github.com/davimqz/Raptor-Run.git
```

2º) Entre na pasta do repositório

```
cd Raptor-Run/
```
3º) Compile os arquivos

```
make all
```

4º) Rode o executável do jogo

```
make run
```


