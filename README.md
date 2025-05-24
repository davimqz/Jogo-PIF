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

## 🕹️ Como Jogar

- Inicie o jogo digitando seu nome e escolhendo a opção **"Start Game"** no menu.
- O personagem principal é o raptor (dinossauro).
- Para **pular os obstáculos (cactos)**, pressione **barra de espaço (espaço)**.
- O objetivo é sobreviver e marcar a maior pontuação possível, desviando dos obstáculos.

### Teclas

- **Espaço:** Faz o raptor pular (tanto para começar quanto durante o jogo).
- **ESC:** Encerra a partida.

---

## 🚦 Como o jogo funciona

- O raptor corre automaticamente, e obstáculos (cactos) aparecem de forma aleatória.
- **A cada 10 pontos**, a velocidade do jogo aumenta, tornando a partida mais desafiadora.
- Se você tiver menos de 3 vidas, a cada 10 pontos aparece um **coração** ("❤️") na tela. Ao pegar o coração, você recupera 1 vida (máximo de 3).
- Quando sua pontuação chega a **30 pontos ou mais**, o jogo entra no **night mode**:
  - As cores do cenário e do personagem mudam, simulando o período noturno.
- O jogo termina quando você perde todas as vidas ou aperta ESC.
- Ao final, seu placar é registrado no ranking, onde as 3 maiores pontuações ficam em destaque.


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


