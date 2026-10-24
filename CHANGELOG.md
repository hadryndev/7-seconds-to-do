# Changelog — 7 Seconds to Do!

Registro das principais mudanças do jogo, desenvolvido por **Hadryn**.

> **Versão jogável atual aprovada:** `Alpha v0.0.4_5`  
> **Estado:** desenvolvimento Alpha. O número após `_` identifica a compilação dentro da versão Alpha correspondente.
>
> Este documento reúne as mudanças confirmadas no histórico do projeto. Compilações intermediárias sem registro detalhado individual são agrupadas; não representam necessariamente lançamentos públicos.

## Alpha v0.0.4_5 — Física, colisões e documentação do jogo

**Status: aprovada após teste prático.**

### Melhorias
- Movimentação horizontal com resposta imediata, sem aceleração ou inércia perceptível.
- Controle horizontal completo durante o salto, permitindo inverter a direção no ar.
- Velocidades mantidas em **175 px/s** normalmente e **230 px/s** após coletar a chave.
- Salto de altura fixa, aproximadamente **6 blocos (96 px)** em espaço livre.
- **Coyote time** de 0,10 segundo: permite saltar por um breve instante após sair da borda de uma plataforma.
- **Jump buffer** de 0,10 segundo: registra o comando de salto pouco antes do pouso.
- Colisões com plataformas sólidas nos quatro lados, resolvidas separadamente nos eixos horizontal e vertical.
- Contato com a porta verde reconhecido por qualquer lado, incluindo a borda.
- Contagem rigorosa de **7 segundos** após a coleta da chave, baseada em relógio monotônico, sem tempo extra intencional.
- Ao esgotar o tempo, a porta fecha, a chave reaparece e o personagem permanece na posição atual.

### Interface e documentação
- Nova opção **Changelog** no menu.
- Créditos atualizados com o desenvolvedor **Hadryn**, o slogan e um resumo das metas do GDD.
- Dados das fases **Zigue-Zagueando** e **Parkour** preservados.

### Resultado dos testes
- O desenvolvedor aprovou a compilação e relatou melhora significativa na responsividade.
- O problema de salto não registrado próximo às extremidades das plataformas foi corrigido na experiência de teste, graças aos ajustes de tolerância.

## Alpha v0.0.4_1 a v0.0.4_4 — Expansão e revisão das fases

**Registro consolidado das compilações anteriores à v0.0.4_5.**

- Consolidação da **Fase 1 — Zigue-Zagueando** e da **Fase 2 — Parkour** no mesmo jogo.
- Estrutura de **Apresentação** (memorizar o percurso) seguida de **Desafio** (coletar a chave e alcançar a porta).
- Ajustes nos percursos e nas plataformas, especialmente da fase Parkour, com revisões baseadas nos mapas do editor.
- Revisões de salto e posicionamento para compatibilidade com os percursos.
- Evolução da navegação entre as duas fases.

> Não há atribuição individual confiável de cada alteração às compilações `_1` a `_4`; por isso foram agrupadas aqui.

## Alpha v0.0.3_1 a v0.0.3_6 — Protótipos de jogabilidade

**Registro consolidado da série Alpha v0.0.3.**

- Desenvolvimento das bases do jogo em **HTML5, CSS e JavaScript (Canvas 2D)**.
- Proposta visual inicial: **personagem preto**, **plataformas amarelas**, **chave vermelha** e **porta verde/vermelha** sobre fundo branco.
- Construção e revisão do percurso da Fase 1.
- Implementação e refinamento de movimentação, salto, colisões e transições entre Apresentação e Desafio.
- Introdução das velocidades de referência **175 px/s** e **230 px/s**.
- Evolução do temporizador de 7 segundos e de seus alertas visuais.
- Desenvolvimento de um **editor de fases separado**, usado para desenhar e exportar cenários.
- Preparação inicial da Fase 2 — Parkour.

> Os detalhes de cada compilação `_1` a `_6` não estão documentados individualmente neste histórico.

---

## Planejado para versões futuras — não implementado

Os itens abaixo fazem parte do **Game Design Document (GDD)** e **não devem ser interpretados como recursos disponíveis na Alpha atual**:

- Campanha com **30 fases** em **6 mundos** de cinco fases cada.
- Mundos temáticos, como fogo, gelo e ventania.
- Plataformas móveis ou que desabam, armadilhas, mecanismos, teletransportes, alterações de gravidade, obstáculos móveis e dash.
- Puzzles com chaves escondidas e uso de mecânicas aprendidas.
- Protagonista geométrico com expressões e pequenas animações.
- Trilha musical dinâmica e efeitos sonoros retrô / 8-bit.
- Salvamento de progresso, recordes pessoais, medalhas ou estrelas e ranking.
- Interface em português (Brasil), inglês e espanhol no lançamento.
- Distribuição gratuita e expansão gradual para outras plataformas.

**Slogan:** *Memorize o caminho. Você tem 7 segundos!*

**Desenvolvedor:** Hadryn
