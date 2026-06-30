# BetterFoil — Design & Plano de Implementação

> Spec versionada no repositório. A cópia de trabalho do plano também fica em
> `~/.claude/plans/`. Atualize este arquivo conforme o projeto evolui.

## Contexto

Criar o **BetterFoil**: homebrew de Nintendo Switch (CFW/Atmosphère) para instalar
jogos/apps em **NSP, NSZ, XCI e XCZ**, como um Tinfoil melhorado. Melhorias: **UI/UX
melhor**, **mais formatos/fontes**, **open-source/auditável**, **stay-awake** (instala com
a tela apagada e acende ao detectar input) e, em fases futuras, um **servidor host para
Windows** que escaneia jogos no PC, lê metadados (nome, tamanho, capa) com as `prod.keys` e
serve pro console, além de instalação com **DLC/updates**.

### Correções de premissa
1. **SDK:** o `NX-SDK` da CiscoDevNet é dos switches de rede Cisco Nexus — sem relação com o
   Nintendo Switch. Stack correto: **devkitPro/devkitA64 + libnx**, app `.nro`.
2. **Base:** fork do **Awoo Installer** (C/C++, SDL2 + Plutonium, GPLv3) — já instala
   NSP/NSZ/XCI/XCZ por SD/USB/rede. Goldleaf descartado (recusa NSZ, sem instalação por rede).
3. **Chaves:** o app **não dumpa nem empacota** chaves. Exige `prod.keys` do usuário
   (modelo nsz/hactool). Conteúdo deve ser legalmente obtido.

### Licença (GPLv3)
`.nro` derivado → código GPLv3, preservar `LICENSE`/`Tinfoil.LICENSE` e avisos de copyright,
declarar mudanças. Servidor Windows pode ser programa separado (comunica só por rede/USB).

## Fases (MVP = app do console primeiro)

- **Fase 0 — Toolchain + fork + build limpo.** devkitPro + portlibs; fork do Awoo
  rebrandizado (`Makefile` APP_TITLE/AUTHOR/VERSION, README, icon); `make` gera
  `BetterFoil.nro`; instalar NSP de teste do SD.
- **Fase 1 — MVP: stay-awake + polish de UI.** Módulo `source/util/stayAwake.*`
  (`appletSetAutoSleepDisabled` + `appletSetMediaPlaybackState`; `lblDisableAutoBrightnessControl`
  + `lblSwitchBacklightOff`; acordar tela via `padUpdate`/`padGetButtonsDown` + toque). Toggle
  nas opções. Polish nas telas Plutonium. Instalar de SD + USB.
- **Fase 2 — Rede + Servidor Windows.** App: instalação HTTP ranged compatível com índice
  Tinfoil (HTML autoindex e JSON/`.tfl`, title ID no nome). Servidor (repo separado): escaneia
  pasta, lê `prod.keys`, decripta NACP + `icon_*.dat`, serve por HTTP com `Range`. Stack
  recomendado: **C# + LibHac + ASP.NET Core**.
- **Fase 3 — Capas/metadados no app + DLC/updates.** Exibir capa/metadados; aritmética de
  title ID (base `...000`, update `+0x800`, DLC `+0x1000+idx`); ler CNMT e montar
  ApplicationRecord agregado via `ncm`/`ns`/`es`.

## Referências
libnx: `ncm`/`ns`/`es`/`fs`, `applet.h`, `lbl.h`, `hid.h`. Formatos: NSZ/NCZ (nicoboss/nsz),
NACP/CNMT (switchbrew, LibHac, hactool). Projetos: Awoo (base), CyberFoil, Goldleaf/Plutonium,
NUT/LibHac (servidor).

## Pré-requisitos
Switch OLED com Atmosphère (LHSP); devkitPro instalado no PC; `prod.keys` próprias.
