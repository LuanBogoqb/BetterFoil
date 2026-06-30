# BetterFoil — Handoff para o próximo Claude

> Este arquivo é o ponto de partida pra qualquer instância do Claude Code que pegar este
> projeto. Leia tudo antes de editar qualquer coisa. Escrito em PT-BR porque o autor
> (GitHub **@LuanBogoqb**) fala português. Responda a ele em PT-BR.

---

## 1. O que é o BetterFoil

Um homebrew de **Nintendo Switch** (CFW/Atmosphère) que instala jogos/apps nos formatos
**NSP, NSZ, XCI e XCZ** — uma versão melhorada do Tinfoil (tinfoil.io). É um **fork do
[Awoo Installer](https://github.com/Huntereb/Awoo-Installer)** (linhagem Tinfoil; C/C++; UI
**Plutonium** sobre SDL2; **GPLv3**). O Awoo já instala de SD/USB/rede; estamos polindo
UX e somando features.

**Objetivos do autor (em ordem de prioridade dele):**
1. **Batch confiável**: instalar base + update + DLC de uma vez (#1 killer feature — ainda NÃO feito).
2. UI/UX melhor (✅ tema dark feito).
3. Mais formatos/fontes.
4. Open-source/auditável.
5. **Stay-awake** (✅ feito): console continua instalando com a tela apagada, acende ao input.
6. **Servidor host Windows** (futuro): escaneia jogos no PC, lê `prod.keys` do usuário pra
   decriptar e mostrar nome/tamanho/capa, e instala com DLC/update.
7. Progresso com ETA (✅ feito).
8. Detecção automática de sigpatches (✅ feito — só detecção; install/manage já existia no Awoo).

---

## 2. ⚠️ Regras críticas do ambiente (NÃO esqueça)

- **O projeto NÃO pode ter espaço no caminho.** O `make` do devkitPro quebra com espaço
  (e junction não resolve — o make resolve de volta pro caminho real). Por isso o projeto
  vive em **`C:\dev\BetterFoil`** (MSYS2: `/c/dev/BetterFoil`), e NÃO em
  `C:\Users\Luan Bogo\...` (que tem espaço). O caminho antigo em `…\Documents\Pro\BetterFoil`
  é só um stub vazio com um `WHERE-IS-THE-PROJECT.txt` apontando pra cá.
  - Obs: a sessão do Claude Code pode iniciar com cwd em `Documents\Pro\BetterFoil`. **Sempre
    `cd /c/dev/BetterFoil`** antes de qualquer comando git/make.
- **devkitPro** instalado em `C:\devkitpro`.
- **Comando de build** (de qualquer shell):
  ```bash
  "C:/devkitpro/msys2/usr/bin/bash.exe" -lc 'cd /c/dev/BetterFoil && make -j4'
  ```
  Saída: `BetterFoil.nro` na raiz. Copiar pra `sdmc:/switch/` e abrir no hbmenu.
- `git config core.autocrlf false` já setado localmente (evita churn de CRLF no Windows).
- **Nunca empacotar/dumpar chaves.** O app exige `prod.keys` fornecidas pelo usuário
  (modelo nsz/hactool). Conteúdo deve ser obtido legalmente pelo usuário.

---

## 3. Estado atual (2026-06-30)

**Fase 0 (toolchain + fork + rebrand + build limpo) e Fase 1 (pacote UX) — COMPLETAS.**
Tudo compila limpo e está pushado em `main`. Branch `dev` criado pra continuar o trabalho.

GitHub: **https://github.com/LuanBogoqb/BetterFoil** — remote `origin` (branch padrão `main`);
Awoo é o remote `upstream` (pra puxar updates).

### O que já foi entregue na Fase 1 (com onde está)
| Feature | Arquivos |
|---|---|
| **Stay-awake** (`inst::power`) | `include/util/stayAwake.hpp`, `source/util/stayAwake.cpp`; init/exit em `source/util/util.cpp`; thread per-frame em `source/ui/MainApplication.cpp` (`AddThread(inst::power::update)`); `setInstalling(true/false)` no ctor/dtor de `source/install/install.cpp`; toggle em `source/ui/optionsPage.cpp`; config em `include/util/config.hpp` + `source/util/config.cpp` (`stayAwake`, default `true`). |
| **ETA no progresso** | `source/ui/instPage.cpp` (`setInstBarPerc` recalcula ETA por item via `armGetSystemTick`) + `include/ui/instPage.hpp` (`installEtaText`). |
| **Tema dark/clean** | troca de paleta em todos os `source/ui/*.cpp` (bg `#12141AFF`, barras `#1B1E27`, acento azul `#4DA3FFFF`, seleção `#4DA3FF33`). Sem `background.png` custom, usa cor sólida. |
| **Detecção de sigpatch** | `source/sigInstall.cpp` + `include/sigInstall.hpp` (`sig::sigPatchesPresent()`); status no menu em `source/ui/mainPage.cpp`. |
| **i18n** | `romfs/lang/en.json` e `romfs/lang/pt.json` (chaves `options.menu_items.stay_awake`, `main.menu.sig_ok`/`sig_missing`). **Validar JSON sempre** — `json::parse` inválido faz crash no boot. |

### Pendências
- **Teste em hardware** do `BetterFoil.nro` na OLED do autor (tema, ETA, stay-awake,
  status de sigpatch). É verificação do lado dele.
- **Fase 2 = batch base+update+DLC** (o #1 dele). Exige ciclo de teste na console a cada
  iteração — **não começar sem confirmação do autor** e sem o loop de teste dele disponível.

---

## 4. Próximos passos (roadmap)

> Plano completo: `C:\Users\Luan Bogo\.claude\plans\claude-queria-fazer-um-eventual-floyd.md`
> e `docs/superpowers/specs/2026-06-30-betterfoil-design.md`.

- **Fase 2 — Batch confiável (base + update + DLC):** aritmética de title ID
  (base `…000`, update `+0x800`, DLC `+0x1000+idx`), ler **CNMT** de cada conteúdo, registrar
  content-meta e montar o **ApplicationRecord** agregado. Fluxo via `ncm`/`ns`/`es` já existe
  na engine do Awoo — estender pra tratar os três tipos juntos. Split-files + NSZ/XCZ já
  funcionam na base.
- **Fase 2b — Servidor host Windows** (programa separado, licença própria; só comunica por
  rede/USB, não linka no `.nro`): C# + **LibHac** + ASP.NET Core (Kestrel). Escaneia pasta,
  lê `prod.keys`, abre PFS0/HFS0, acha Control NCA, decripta **NACP** (nome/versão) e
  `icon_*.dat` (capa), serve índice compatível com Tinfoil por HTTP com `Range`.
- **Fase 3 — Capas/metadados no app** + instalação completa com update/DLC.

---

## 5. Convenções e cuidados ao editar

- **GPLv3:** preservar `LICENSE` e avisos de copyright (Tinfoil/Awoo/libusbhsfs). Publicar o
  source. Declarar mudanças significativas.
- **UI Plutonium:** `pu::ui::Application/Layout`, `AddThread(std::function<void()>)` roda por
  frame, elementos `Menu/MenuItem/ProgressBar/TextBlock/Rectangle/Image`, cores via
  `pu::ui::Color::FromHex("#RRGGBBAA")`. Singleton `inst::ui::mainApp`.
- **Strings de UI** sempre via `"chave"_lang` + entrada nos DOIS jsons (en/pt). Chave faltando
  renderiza `didn't find: <key>` (não-fatal), mas JSON inválido **crasha** — valide.
- **libnx relevante:** `applet` (`appletSetAutoSleepDisabled`, `appletSetMediaPlaybackState`),
  `lbl` (`lblSwitchBacklightOn/Off`, auto-brightness), `hid/pad` (`padInitializeDefault`,
  `padUpdate`, `padGetButtonsDown`/`padGetButtons` — **não** existe `padGetButtonsHeld**),
  `armGetSystemTick`/`armGetSystemTickFreq`, pipeline `ncm/ns/es/fs`.
- **Build incremental + commit por feature.** Compile (EXIT=0, "built … BetterFoil.nro")
  antes de commitar. Mensagens de commit em inglês imperativo (segue o histórico).
- Ao alterar `config.cpp`, manter defaults graciosos em `parseConfig` (`j.value("chave",
  default)`) pra não zerar config de quem atualiza.

---

## 6. Memória persistente do Claude

Há memória em `C:\Users\Luan Bogo\.claude\projects\C--Users-Luan-Bogo-Documents-Pro-BetterFoil\memory\`
(índice em `MEMORY.md`): `user.md`, `betterfoil-project.md`, `switch-homebrew-refs.md`.
Mantenha o `betterfoil-project.md` atualizado conforme o status muda.

---

_Atribuição: fork de Awoo Installer (Huntereb) → Tinfoil (Adubbz); UI Plutonium (XorTroll);
suporte NSZ (blawar/nicoboss). GPLv3._
