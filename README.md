# Asteroids-3D

Asteroids-3D trata-se de um jogo baseado em Outer Wilds e Asteroids, desenvolvido como projeto final da disciplina de Computação Gráfica e Visualização (CGVis) da UFRGS. O jogador controla uma nave nas proximidades de um sistema solar, enquanto asteróides tentam destruí-la. Destruir asteróides concede pontos ao jogador, sendo o objetivo do jogo realizar o maior número de pontos.

## Contribuições

- **Rafael Petry da Silva:** Game design, arquitetura do projeto, renderização de meshes, implementação da física (animação, colisões, etc), implementação de câmeras e interface.
- **Thibault Pelenc:** - Game design, escolha de meshes e texturas, renderização de texturas, implementação de texturas procedurais e implementação do shader.

## Uso de IA

A dupla fez uso de IA, utilizando a ferramenta Github Copilot (com os agentes GPT-5.2-Codex, GPT-5.3-Codex e Claude Haiku) e o Claude Chat (com o agente Sonnet 4.6). Ela foi usada para gerar as implementações inciais de grande parte das funcionalidades, que foram, posteriormente, alteradas para se alinharem corretamente à especificação e arquitetura do projeto. Neste sentido (da IA ser usada como o "ponta-pé" inicial do desenvolvimento), ela se demonstrou bastante útil, poupando muito tempo que seria investido em buscar a documentação de funções específicas das bibliotecas. O que a IA mais pecou foi na organização do código, que muitas vezes, era gerado em um arquivo único e extenso, contendo "valores mágicos" de difícil compreensão e sem seguir a arquitetura geral do projeto.

## Imagens

_(Serão adicionadas na versão final)_

## Manual

### Controles

- **W:** Acelera a nave para frente.
- **A:** Acelera a nave para a esquerda.
- **S:** Acelera a nave para trás.
- **D:** Acelera a nave para a direita.
- **Shift (Esquerdo):** Acelera a nave para cima.
- **Ctrl (Esquerdo):** Acelera a nave para baixo.
- **Mouse:** Movimento do mouse controla guinada e inclinação da nave.
- **Mouse (Botão Esquerdo):** Atira um raio laser.
- **R:** Enquanto é segurado, o movimento do mouse controla a rolagem da nave.
- **C:** Alterna entre as câmeras de primeira e terceira pessoa.
- **F1:** Alterna a janela entre modo _full screen_ e _windowed_.
- **ESC:** Fecha o jogo.

### Como Jogar

O jogo funciona em um sistema de "levas" de asteróides, que são gerados para destruir a nave espacial. A primeira leva possui 5 asteróides e as próximas aumentam em incrementos de 5. Cada asteróide possui um entre três tamanhos (pequeno, médio ou grande), quanto maior nesta escala, maior o dano que ele causa à nave e destruir um asteróide de um tamanho, gera dois asteróides de um tamanho menor. Após destruir todos asteróides de uma leva, o jogador tem 30 segundos para o início da próxima. Ele pode usar este tempo para realizar um pouso em algum dos planetas, recuperando seus 5 pontos de vida. Contudo, um pouso mal-sucedido ou uma colisão com alguma das árvores do planeta causa 1 ponto de dano à nave. Uma colisão com o sol encerra o jogo imediatamente. Destruir um asetróide concede 100 pontos, sendo o objetivo realizar o maior número de pontos antes de ser destruído.

## Créditos

Para desenvolver o trabalho, foram usados assets de mesh disponíveis em sites ou repositórios do github, cujos créditos são listados à seguir:

- **Nave:** https://free3d.com/3d-model/uss-yeager-ncc-61947-spaceship-80266.html
- **Sol e Planeta:** https://gist.github.com/svmhdvn/efc5779e79780e8b72fb3040423f512f
- **Asteróide:** https://free3d.com/3d-model/moon-17150.html
- **Árvore:** https://free3d.com/3d-model/tree-74556.html
- **Arbusto:** https://www.cgtrader.com/free-3d-models/plant/bush/agave-eae5af72-2661-40e1-b475-038e3ce759a7

Além disso, os seguintes arquivos de shader (em especial o "Disney's PBR") foram obtidos de laboratórios anteriores da disciplina.

- **Vertex Shader:** https://github.com/cgvis-inf-ufrgs/cgvis-2026-1-lab-2-coelinhos-da-pascoa-cgvis-lab2-coelinhos-da-pascoa/blob/main/src/shader_vertex.glsl
- **Fragment Shader:** https://github.com/cgvis-inf-ufrgs/cgvis-2026-1-lab-2-coelinhos-da-pascoa-cgvis-lab2-coelinhos-da-pascoa/blob/main/src/shader_fragment.glsl
