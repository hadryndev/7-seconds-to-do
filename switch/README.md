# 7 Seconds to Do! — Nintendo Switch (Homebrew)

Port nativo da Alpha v0.0.4_7, escrito em C com devkitPro/libnx.

## Compilação

Necessita devkitPro, devkitA64 e libnx configurados. Execute `make` a partir desta pasta.
O resultado esperado é `7STD-Switch.nro`.

## Controles

- D-pad / analógico esquerdo: mover
- B: pular
- X: reiniciar etapa
- +: voltar ao menu
- A: confirmar opções no menu

## Fases

Zigzagging, Parkour e The Gauntlet. Progressão automática: apresentação → desafio → próxima fase. Ao concluir a terceira fase, aparece a tela final.

## Observação

Esta pasta reúne somente os arquivos necessários ao desenvolvimento. Os arquivos gerados (`build/`, `.elf`, `.nacp`) não devem ser versionados. O `.nro` deve ser publicado como anexo da Release após validação em console.
