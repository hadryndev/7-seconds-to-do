# 7 Seconds to Do! — Game Design Document (GDD)

> **Memorize o caminho. Você tem 7 segundos!**  
> Um jogo independente de **Hadryn**.

| Campo | Informação |
|---|---|
| Estado do documento | Primeira consolidação do GDD (planejamento) |
| Versão jogável de referência | **Alpha v0.0.4_5** (aprovada) |
| Desenvolvedor / identidade pública | **Hadryn** |
| Gênero proposto | Plataforma com elementos de puzzle, precisão e corrida contra o tempo |
| Estado do projeto | Alpha em desenvolvimento |
| Distribuição planejada | Gratuita, sem anúncios ou compras obrigatórias |
| Narrativa | Sem história; foco exclusivo na jogabilidade |

> **Nota de transparência:** este documento reúne decisões de design e metas futuras. **Planejado não significa implementado.** A versão jogável Alpha v0.0.4_5 não foi alterada pela criação deste documento.

## 1. Visão do jogo

**7 Seconds to Do!** é um jogo independente de plataforma que une **memorização, velocidade, aprendizado e variedade de desafios**. O jogador deve primeiro conhecer e memorizar um percurso; depois, ao coletar a chave vermelha, dispõe de **exatamente 7 segundos** para chegar à porta de saída.

A experiência pretende transmitir **aprendizado e correria**. A intenção é que as falhas sejam um incentivo divertido para tentar novamente, não uma punição excessiva.

**Posicionamento:** *fácil de aprender, difícil de dominar*. O projeto é pensado tanto para jogadores casuais quanto para fãs de plataformas, speedrunners e pessoas que gostam de raciocínio e memória.

**Visão do criador:** tornar este o primeiro jogo independente oficialmente publicado por Hadryn.

## 2. Loop principal de gameplay

Todas as fases terão **duas etapas obrigatórias**, nesta ordem:

### 2.1 Apresentação

- O jogador conhece o cenário e memoriza o caminho e os obstáculos.
- Não há cronômetro de sete segundos durante a memorização.
- A etapa termina ao alcançar a porta correspondente, iniciando o Desafio.

### 2.2 Desafio

1. O jogador procura a **chave vermelha**.
2. **A coleta da chave** abre a porta verde, aumenta a velocidade do personagem e inicia imediatamente o cronômetro.
3. O jogador deve tocar a porta antes do fim do prazo de **7 segundos**, com contato válido por qualquer lado.
4. Se o tempo acaba, a porta fecha, a chave reaparece em sua posição original e o personagem **permanece onde estava**.
5. As tentativas são **ilimitadas**, sem penalidade pelo número de erros.

A contagem regressiva terá sempre sete segundos, em todas as fases. Não estão previstos tempos extras ou redução do limite conforme a dificuldade.

### 2.3 Puzzles futuros

Em fases futuras, a chave poderá estar escondida ou exigir o uso de mecânicas aprendidas anteriormente para ser encontrada ou alcançada. O tempo de sete segundos continua iniciando **somente após a coleta**.

## 3. Física e controles — base aprovada

A **Alpha v0.0.4_5** foi aprovada em teste prático pelo desenvolvedor, com destaque para maior responsividade e correção do problema de salto próximo às extremidades das plataformas.

| Regra | Especificação consolidada |
|---|---|
| Personagem | Quadrado, 16 × 16 px na base atual |
| Movimento horizontal | Resposta imediata ao comando, sem aceleração ou inércia horizontal |
| Velocidade normal | **175 px/s** |
| Velocidade após pegar a chave | **230 px/s** enquanto o cronômetro estiver ativo |
| Salto | Altura fixa de cerca de **6 blocos / 96 px** em espaço livre |
| Controle no ar | Total, inclusive reversão imediata de direção |
| Coyote time | Habilitado; valor inicial de referência **0,10 s** |
| Jump buffer | Habilitado; valor inicial de referência **0,10 s** |
| Plataformas | Completamente sólidas nos quatro lados |
| Porta verde | A etapa conclui ao encostar por qualquer lado, inclusive na borda |
| Cronômetro | **7 segundos rigorosos**, sem tolerância extra de conclusão |
| Falha por tempo | Reposiciona somente a chave, fecha a porta e restaura a velocidade normal |

Os tempos de coyote time e jump buffer são parâmetros de referência para ajustes técnicos; a decisão consolidada é que ambos permaneçam presentes.

## 4. Estrutura de campanha

| Meta para a versão final | Quantidade |
|---|---:|
| Mundos | **6** |
| Fases por mundo | **5** |
| Total de fases | **30** |
| Etapas por fase | **2**: Apresentação + Desafio |
| Total de etapas jogáveis previstas | **60** |
| Duração pretendida da primeira conclusão | **30 minutos a 1 hora** |

A campanha será sequencial e apresentará dificuldade progressiva. Cada mecânica deve ser introduzida, praticada e depois combinada com outras. A duração é **meta de design**, não resultado de testes com o jogo completo.

### 4.1 Mundos

Os mundos serão temáticos e terão cinco fases cada. Ideias mencionadas até agora:

- **Mundo inicial / fundamentos:** corrida, salto e memorização do percurso; inclui as duas fases atuais.
- **Fogo:** temática e obstáculos associados ao fogo.
- **Gelo:** temática e obstáculos associados ao gelo.
- **Ventania:** temática e obstáculos associados ao vento.
- **Dois outros mundos:** ainda não definidos.

**Atenção:** nomes definitivos, ordem dos mundos e mecânicas específicas por mundo ainda não foram aprovados.

### 4.2 Fases existentes na Alpha

| Fase | Nome | Conteúdo |
|---|---|---|
| 1 | **Zigue-Zagueando** | Apresentação + Desafio |
| 2 | **Parkour** | Apresentação + Desafio |

Ainda restam **28 fases planejadas**. Não confundir essa proporção com o percentual de conclusão do desenvolvimento total.

## 5. Mecânicas futuras planejadas

As seguintes mecânicas foram selecionadas para **planejamento**, sem autorização de implementação imediata:

- Plataformas móveis;
- Plataformas que desabam;
- Espinhos e armadilhas;
- Botões e mecanismos;
- Teletransportes;
- Alteração de gravidade;
- Inimigos ou obstáculos móveis;
- Dash (avanço rápido);
- Puzzles com chave escondida e uso das mecânicas aprendidas.

**Princípio de design:** oferecer desafios variados e renovados ao jogador, preservando a mecânica central dos sete segundos.

## 6. Progressão, pontuação e competição — planejamento

O projeto prevê:

- Progressão sequencial com desbloqueio de fases;
- Salvamento do progresso;
- Recordes pessoais;
- Medalhas ou estrelas de desempenho;
- Ranking entre jogadores.

**Ainda a definir:** como os recordes serão calculados, os critérios de medalhas, o funcionamento técnico do ranking e a persistência do progresso. Essas funcionalidades **não devem ser descritas como disponíveis na Alpha atual**.

## 7. Direção visual

A direção artística escolhida é **minimalista**, com possibilidade de elementos **geométricos modernos**. Pixel Art é uma referência de interesse, mas não uma exigência: a prioridade é preservar clareza e estabilidade da jogabilidade.

### 7.1 Elementos visuais atuais

- Fundo predominantemente branco;
- Personagem como quadrado preto;
- Plataformas amarelas;
- Chave vermelha;
- Porta vermelha quando fechada e verde quando aberta.

### 7.2 Personagem — planejamento

O protagonista continuará sendo um **quadrado com personalidade**, ganhando expressões e pequenas animações. Isso **não implica** uma história: foi decidido manter foco exclusivo na jogabilidade.

Os mundos futuros poderão ter identidades visuais distintas, preservando a leitura rápida dos obstáculos e objetos importantes.

## 8. Áudio — planejamento

### 8.1 Música

Trilha sonora **dinâmica**, com mudança de sensação entre:

- **Apresentação:** favorece observação e memorização;
- **Desafio:** reforça urgência, especialmente durante a contagem regressiva.

O gênero musical definitivo ainda não foi fechado.

### 8.2 Efeitos sonoros

Estilo escolhido: **retrô / 8-bit**.

Sons previstos para movimentação e saltos, coleta da chave, contagem regressiva, abertura/fechamento da porta, conclusão de fase, tempo esgotado, ambientes/obstáculos e expressões do protagonista.

Como o jogo permite repetição ilimitada, os efeitos devem ser perceptíveis sem se tornarem cansativos.

## 9. Público, plataformas e publicação

**Públicos desejados:** jogadores casuais, fãs de plataforma, speedrunners, fãs de raciocínio e memória, crianças e adolescentes e público geral. A classificação indicativa oficial ainda não foi determinada.

**Plataformas planejadas:**

1. Navegador (**Web / HTML5**);
2. **Windows**;
3. **Android**;
4. **Consoles** (possibilidade futura, sujeita a requisitos e adaptações).

A estratégia definida é **lançamento gradual**; a ordem exata ainda não foi formalizada. A implementação atual é em HTML5/JavaScript, para navegador.

**Modelo comercial:** gratuito, sem anúncios e sem compras obrigatórias.

**Idiomas desde o lançamento pretendido:** português (Brasil), inglês e espanhol.

**Data de lançamento:** não definida.

## 10. Referências criativas

As referências declaradas pelo desenvolvedor são **Celeste** e **Geometry Dash**, especialmente quanto a desafios de plataforma, memorização e possibilidades de puzzles. O projeto deve desenvolver uma identidade própria, sem reproduzir conteúdo desses jogos.

## 11. Versão atual versus visão futura

| Aspecto | Alpha v0.0.4_5 | Planejado para a versão completa |
|---|---|---|
| Fases | 2 | 30 |
| Apresentação e Desafio | Presente | Obrigatório em todas as fases |
| Cronômetro e chave | Presente | Regra central mantida |
| Física refinada | Presente e aprovada | Mantida e testada continuamente |
| Mundos temáticos | Não implementados | 6 mundos |
| Obstáculos e puzzles avançados | Não implementados | Mecânicas e chaves escondidas |
| Expressões do personagem | Planejadas | Pequenas animações e expressões |
| Música e efeitos | Planejamento | Trilha dinâmica e efeitos 8-bit |
| Recordes, medalhas, ranking | Planejamento | Sistemas a especificar |
| Internacionalização | Planejamento | PT-BR, EN e ES |
| Distribuição | Protótipo HTML | Gratuita e multiplataforma gradualmente |

## 12. Princípios que orientam o desenvolvimento

1. **Aprender antes de correr:** a Apresentação é parte fundamental da identidade do jogo.
2. **Sete segundos são sete segundos:** tempo fixo e rigoroso após a chave.
3. **Falhar não deve punir excessivamente:** tentativas ilimitadas e reinício rápido do desafio.
4. **Controles previsíveis:** a precisão depende de resposta imediata e colisões consistentes.
5. **Dificuldade progressiva:** introduzir mecânicas e combiná-las gradualmente.
6. **Variedade sem perder a identidade:** cada mundo traz situações diferentes, preservando a proposta central.
7. **Documentar com transparência:** distinguir funcionalidades existentes, aprovadas para implementação e apenas planejadas.

## 13. Próximas definições em aberto

- Nomes e ordem dos seis mundos;
- Regras de cada mundo temático;
- Critérios de recorde e medalhas;
- Funcionamento do ranking online;
- Sistema de salvamento;
- Detalhes das animações e expressões;
- Composição musical e implementação dos efeitos;
- Interface de seleção de mundos e fases;
- Adaptações de controles para mobile e consoles;
- Cronograma de lançamento e testes com jogadores.

---

**Desenvolvido por Hadryn**  
**7 Seconds to Do! — Memorize o caminho. Você tem 7 segundos!**

*Documento de planejamento. A publicação deste arquivo não altera a build Alpha v0.0.4_5.*
