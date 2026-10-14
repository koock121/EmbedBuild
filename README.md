# EmbedBuild

**AI-driven universal embedded build & flash system — an open-source alternative to Keil.**

One `build.json` drives the whole flow: **compile → link → artifacts → flash**.
15 toolchains, 15 flashers, parallel + incremental builds, and first-class **AI agent automation (MCP)**.

> **The killer feature: you don't operate it — you talk.**
> Tell an AI agent *"use EmbedBuild to build and flash my project"*, and it will
> locate the project, read the config, compile, diagnose failures, fix issues and
> report back — fully autonomously, end to end.

**中文简介**：一份 `build.json` 驱动 编译→链接→产物→烧录 全流程；支持 15 种工具链 + 15 种烧录器；并行 + 增量编译；原生 MCP 支持 —— **对 AI 智能体说一句话，它就能自己完成定位工程、读配置、编译、烧录、排错、汇报的全流程**。Keil 的开源替代品。

---

## ✨ Why EmbedBuild

| | EmbedBuild | Keil |
|---|---|---|
| Configuration | One readable `build.json` (git-friendly) | Binary `.uvprojx` |
| CLI / scripting | ✅ First-class | ❌ Painful |
| AI agent (MCP) | ✅ Built-in server, 8 tools | ❌ None |
| Incremental builds | ✅ mtime + `.d` deps (ms-level rebuild) | ✅ |
| Price | Free & open source | Paid |
| Toolchains | 15 (pluggable) | 1 (locked) |

## 🤖 AI Agent: full-flow automation (the highlight)

EmbedBuild ships a **built-in MCP server** — the standard protocol that AI agents
(Trae, Claude Desktop, Cursor, Cline, …) use to call tools. Register it once:

```bash
embedbuild mcp        # starts the MCP server (stdio)
```

Then simply **say it out loud**:

> “用 EmbedBuild 编译 projects/MyApp 并烧录”
> "Build and flash my project, then show me the firmware size"

The agent autonomously does the whole pipeline:

```
locate project → read build.json → compile (parallel) → parse JSON result
   → on failure: read errors → run doctor → fix → retry
   → on success: flash via OpenOCD → report size & elapsed time
```

**Agent-friendly by design:**

- `embedbuild --json build` — pure JSON on **stdout** (logs go to stderr). Success flag,
  firmware size (text/data/bss), per-file timings: all machine-readable, zero guessing.
- `embedbuild doctor` — structured health report of every toolchain & flasher
  (version, path, readiness). Agents self-troubleshoot instead of asking you.
- `embedbuild keil xxx.uvprojx` — one command converts existing **Keil projects**
  to `build.json`. The agent can onboard any legacy project in seconds.
- Python API — `from embedbuild import build` for scripted multi-project flows.
- Plugin-only-commands architecture: plugins *return* commands (`argv/cwd/env`),
  the core executes them uniformly — safe, predictable, perfectly loggable for agents.

## 🚀 Quick start

```bash
pip install -e .            # zero runtime dependencies
cd examples/STM32F103       # or any project with a build.json
embedbuild doctor           # check toolchains & flashers
embedbuild build            # compile → hex/bin/elf/map/lst
embedbuild flash            # flash via OpenOCD / J-Link / ST-Link / ...
```

### Example `build.json`

```json
{
  "project": { "name": "demo_f103", "cpu": "cortex-m3" },
  "sources": ["main.c", "startup.c"],
  "includes": ["include"],
  "toolchain": "arm-gcc",
  "c_flags": ["-O2", "-Wall"],
  "link_flags": ["-nostartfiles", "-specs=nano.specs"],
  "link_tail_flags": ["-lc", "-lm", "-lnosys"],
  "artifacts": { "hex": true, "bin": true, "map": true, "lst": true },
  "flash": { "flasher": "openocd", "cfg": "cmsis-dap", "load_addr": "0x08000000" }
}
```

## 📦 What's supported

**Toolchains (15):** arm-gcc, armclang, riscv-gcc, xtensa-gcc (ESP32), avr-gcc,
msp430, sdcc-8051, stm8, pic, rl78, ppc, mips, nios2, arc, host-gcc

**Flashers (15):** openocd, pyocd, stlink, jlink, dfu-util, avrdude, esptool,
picotool, stm8flash, uniflash, nrfutil, bossac, cc-tool, stm32-isp, openfpgaloader

**Core features:**
- Parallel compilation (CPU-count workers)
- Incremental rebuild via mtime + `.d` dependency parsing (17-file project: ~0.3 s)
- Artifacts: `.elf` / `.hex` / `.bin` / `.map` / `.lst`
- Firmware size report (text / data / bss) in every build result

## 🗂 Project layout

```
embedbuild/
├── cli.py              # command-line interface (build/flash/doctor/keil/mcp)
├── api.py              # Python API
├── mcp_server.py       # MCP server — AI agent entry point
├── core/               # engine, scheduler, incremental, config, result, reporter
├── plugins/
│   ├── toolchains/     # 15 toolchain plugins (generate commands, never execute)
│   └── flashers/       # 15 flasher plugins
├── keil/               # .uvprojx parser + mapper (Keil migration)
└── utils/              # process runner, paths, logging
```

## 🔌 MCP tools exposed to agents

| Tool | What the agent can do |
|---|---|
| `discover` | Find projects containing build.json |
| `describe` | Read a project's configuration |
| `build` | Compile a project (JSON result back) |
| `flash` | Flash firmware to the chip |
| `doctor` | Environment health check |
| `keil_convert` | Convert a Keil .uvprojx to build.json |
| … | (8 tools total) |

## 📄 License

MIT

---

*Built with ❤️ for developers who'd rather say "build and flash it" than click through an IDE.*
