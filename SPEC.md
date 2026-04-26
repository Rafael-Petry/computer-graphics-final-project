# Especificação da Implementação

> [!CAUTION]
> - Você <ins>**não pode utilizar ferramentas de IA para escrever esta
>   especificação**</ins>

## Integrantes da dupla

- **Aluno 1 - Nome**: Rafael Petry da Silva
- **Aluno 1 - Cartão UFRGS**: 00581131

- **Aluno 2 - Nome**: Thibault Pelenc
- **Aluno 2 - Cartão UFRGS**: 00612013

## Detalhes do que será implementado

- **Título do trabalho**: Asteroids-3D
- **Parágrafo curto descrevendo o que será implementado**: 
A ideia do projeto é mesclar mecânicas de dois jogos: *Outer Wilds* e *Asteroids*. O jogador
deverá controlar uma nave espacial, em primeira pessoa, enquanto destrói asteroides que
tentam causar danos nela. Destruir um asteroide concede pontos ao jogador, sendo o objetivo
do jogo fazer o maior número de pontos antes que a nave exploda com os danos. O cenário será
um sistema solar, com alguns planetas orbitando um sol ao centro. O jogador também poderá pousar
nos planetas para realizar reparos na nave, aguardando parado por um certo tempo para iniciar a recuperação.

## Especificação visual

### Vídeo - Link

> [!IMPORTANT]
> - Coloque aqui um link para um vídeo que mostre a aplicação gráfica
>   de referência que você vai implementar. **Sua implementação deverá
>   ser o mais parecido possível com o que é mostrado no vídeo (mais
>   detalhes abaixo).**
> - **Você não pode escolher como referência: (1) algum trabalho realizado
>   por outros alunos desta disciplina, em semestres anteriores. (2) Minecraft.**
> - Por exemplo, você pode colocar um vídeo de um jogo que você gosta,
>   e seu trabalho final será uma re-implementação do jogo.
> - O vídeo pode ser um link para YouTube, Google Drive, ou arquivo mp4 dentro
>   do próprio repositório. Mas, garanta que qualquer um tenha
>   permissão de acesso ao vídeo através deste link.

**Observação:** O vídeo a seguir diz respeito ao jogo Outer Wilds porque é a principal
referência para a forma que o jogo funcionará em termos da movimentação 3D.
Um vídeo do Asteroids não foi adicionado, pois é um jogo 2D mais antigo que só será usado
como ideia de mecânicas adicionais.

**Fonte:** [/demo/outer_wilds_demo.mp4](https://github.com/Rafael-Petry/computer-graphics-final-project/blob/main/demo/outer_wilds_demo.mp4)
<video src="https://github.com/user-attachments/assets/dae59547-6338-42a8-8fd7-77b62e747d72" type="video/mp4"></video>

### Vídeo - Timestamp

> [!IMPORTANT]
> - Coloque aqui um **intervalo de ~30 segundos** do vídeo acima, que
>   será a base de comparação para avaliar se o seu trabalho final
>   conseguiu ou não reproduzir a referência.

- **Timestamp inicial**: 00:35
- **Timestamp final**: 01:05

### Imagens

> [!IMPORTANT]
> - Coloque aqui **três imagens** capturadas do vídeo acima, que você
>   irá usar como ilustração para as explicações que vêm abaixo.

___
**1) Imagem de parte do sistema solar. (Fonte: [/demo/1.png](https://github.com/Rafael-Petry/computer-graphics-final-project/blob/main/demo/1.png))**
<img width="1920" height="1080" alt="1" src="https://github.com/user-attachments/assets/fce5704c-28a1-4dce-a3a2-a9db81815588" />
___

**2) Imagem de pouso em planeta. (Fonte: [/demo/2.png](https://github.com/Rafael-Petry/computer-graphics-final-project/blob/main/demo/2.png))**
<img width="1920" height="1080" alt="2" src="https://github.com/user-attachments/assets/bd9e0129-bc77-4972-95fc-8e198dfcc8e1" />
___

**3) Imagem de exterior da nave. (Fonte: [/demo/3.png](https://github.com/Rafael-Petry/computer-graphics-final-project/blob/main/demo/3.png))**
<img width="1920" height="1080" alt="3" src="https://github.com/user-attachments/assets/dbe2f3ba-b438-4a27-97e1-369da81bfbd6" />
___

## Especificação textual

Para cada um dos requisitos abaixo (detalhados no [Enunciado do Trabalho final - Moodle](https://moodle.ufrgs.br/mod/assign/view.php?id=6018620)), escreva um parágrafo **curto** explicando como este requisito será atendido, apontando itens específicos do vídeo/imagens que você incluiu acima que atendem estes requisitos.

### Malhas poligonais complexas
Como malhas poligonais complexas, haverá a nave espacial, os corpos celestes (asteroides, planetas e sol)
e outros objetos "de enfeite" nos planetas, como árvores e arbustos.

### Transformações geométricas controladas pelo usuário
Será possível controlar a translação da nave por meio das teclas de movimento no teclado e, 
também, a rotação (*pitch*, *yaw* e *roll*) da nave com o mouse (o controle de *roll* 
será habilitado enquanto o jogador estiver pressionando a tecla **R**). 

### Diferentes tipos de câmeras
Terá uma câmera livre controlada em primeira pessoa pelo jogador e uma câmera *look-at* que será utilizada
para auxiliar o jogador na aterrissagem em planetas.

### Instâncias de objetos
Os asteroides e as árvores terão mais de uma instância copiada.

### Testes de intersecção
Haverão os testes de colisão da nave com os corpos celestes (planetas, sol e asteroides) e árvores que estiverem nos planetas.
Também terá o teste de colisão dos tiros da nave com os asteroides.

### Modelos de Iluminação em todos os objetos
Para a nave, será utilizado o *Phong Illumination* para definir uma superfície *glossy*, 
enquanto que para os demais objetos será usado o *Lambert Shading*, para superfícies difusas ideais.

### Mapeamento de texturas em todos os objetos
Haverá uma textura metálica da nave, uma textura rochosa para os corpos celestes e uma textura para o tronco das árvores.

### Movimentação com curva Bézier cúbica
A órbita dos planetas ao redor do sol será definida com curva de Bézier cúbica.

### Animações baseadas no tempo ($\Delta t$)
A movimentação da nave, a órbita dos planetas e a movimentação dos asteróides em direção à nave serão baseadas no tempo.

## Limitações esperadas

> [!IMPORTANT]
> - Coloque aqui uma lista de detalhes visuais ou de interação que
>   aparecem no vídeo e/ou imagens acima, mas que você **não pretende
>   implementar** ou que você **irá implementar parcialmente**.
> - Para cada item, **explique por que** não será implementado ou por
>   que será implementado parcialmente.

**1) Comandos visíveis no HUD, como "Equipar Onduloscópio" e "Ver Mapa":** Não serão implementados porque são mecânicas mais específicas
e complicadas do Outer Wilds que ficariam desconexos na nossa versão.

**2) Sair da nave com o personagem:** Não será implementado porque, igualmente, para a nossa versão, 
não seria uma mecânica muito necessária que só acabaria aumentando a complexidade.

**3) Detalhes dos planetas:** Será implementado parcialmente, pois os planetas do Outer Wilds são muito detalhados, 
com construções e objetos mais elaborados. A ideia é simplificar as "decorações" para termos planetas mais genéricos, mas ainda funcionais.

**4) Modelos específicos dos objetos:** Os modelos que utilizaremos na implementação, provavelmente, não serão idênticos aos que estão no vídeo, mas
buscaremos por objetos similares que cumpram o mesmo papel convincentemente.
