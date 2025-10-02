## Quick orientation for AI coding assistants

This repository is the Godot Engine (C++). Key facts an automated contributor
needs to know to be productive:

- Build system: SCons driven via the top-level `SConstruct`. The repo requires
  Python >= 3.8 and SCons >= 4.0 (see `SConstruct` top: `EnsurePythonVersion(3, 8)` /
  `EnsureSConsVersion(4, 0)`).
- Targets: `target` has three common values: `editor`, `template_release`, and
  `template_debug`. `dev_build=yes` enables engine-dev-only code (`DEV_ENABLED`).
- Platform detection and per-platform flags live under `platform/*/detect.py`.

Important directories to consult for architecture and conventions:

- `core/`, `scene/`, `servers/`, `drivers/`, `editor/`, `main/` — primary
  engine subsystems and their `SCsub` build scripts.
- `modules/` — optional engine modules. Each module typically exposes
  `config.py` and `SCsub` files; enable/disable with `module_<name>_enabled=yes`.
- `thirdparty/` — vendored libs: many platform-specific behaviours depend on
  bundled versions here.
- `SConstruct` — central build flags and conventions (search for `optimize`,
  `debug_symbols`, `scu_build`, `ninja`, `compiledb`).

Common build flags and examples (use these exact forms as templates):

- Build editor (Linux example):

  scons platform=linuxbsd target=editor dev_mode=yes -j8

- Build a release export template:

  scons platform=linuxbsd target=template_release -j8

- Turn off most modules for a minimal template:

  scons platform=linuxbsd target=template_release modules_enabled_by_default=no module_text_server_fb_enabled=no -j8

- Create compilation DB (for IDEs):

  scons compiledb=yes

- Use ninja backend:

  scons ninja=yes

CI and test commands (how repository exercises the engine in CI):

- Unit tests / smoke tests used in CI:

  ./bin/godot.* --headless --test --force-colors

- Check class reference / doctool (update docs when public API changes):

  ./bin/godot.* --doctool --headless

- Generate Mono glue (C#) after building an editor with Mono support:

  ./bin/godot.* --headless --generate-mono-glue ./modules/mono/glue

- API compatibility helper (used in CI):

  ./misc/scripts/validate_extension_api.sh ./bin/godot.*

Project conventions and patterns to follow (discoverable in the repo):

- Compilation defines: `TOOLS_ENABLED` (editor), `DEBUG_ENABLED` (debug
  features), `DEV_ENABLED` (engine developer-only code). Prefer following
  existing #ifdef usage in `core/` and `editor/`.
- Godot targets C++17. Exceptions are disabled by default (`disable_exceptions`
  is true in `SConstruct`), and the codebase avoids throwing exceptions.
- Modules expose configuration in `modules/<module>/config.py` and use
  `SCsub` / `SConscript` files — change the module's `config.py` when adding
  new build-time flags.
- Platform-specific build logic lives in `platform/<platform>/detect.py`.

Where to look for tests, docs, and style guidance:

- Unit tests: `tests/` (built with `scons tests=yes`). See `CONTRIBUTING.md`
  for testing guidance.
- Documentation generation and class reference helpers are invoked via the
  engine binary and XML doc files (see `doc/` and `editor/` helpers).
- Code style and contribution workflow: `CONTRIBUTING.md` and the project's
  online contributing guide linked from the top-level `README.md`.

If you modify public API (add methods/properties/signals): include
corresponding documentation updates (class reference XML) and run `--doctool` to
verify no missing doc changes.

When in doubt or changing build behavior: search `SConstruct` for how flags
propagate, and check `*/SCsub` files for how targets are wired together.

If this file is missing something you need (specific platform workflow, a
local SDK path, or environment setup), please point to the exact failing
command and I will expand these instructions.

## LLM integration & "Origin Engine" plan (user intent)

The current plan is to add LLM support to the project and eventually rename
the engine to "Origin". For now the immediate scope is to integrate an LLM
into the built-in script editor (the same area that provides inline
completions and features similar to Cursor). Keep the change incremental,
non-destructive, and opt-in.

Recommendations and concrete integration points:

- Where to implement: prefer an editor-side plugin or module that doesn't
  change core engine behaviour. Two good options:
  - `editor/` — implement as an editor feature/plugin that only affects the
    editor binary (recommended for fastest iteration).
  - `modules/llm/` — implement as a module exposing a clear C++ API and
    optional scripting bindings (preferred if the feature should be available
    to exported tools or runtime in some limited form).

- Key files to touch for editor integration: `editor/` (script editor UI code),
  and `editor/SCsub` to wire build, plus `SConstruct` only if you need new top-
  level build flags (avoid if possible; use module flags instead).

- API key and secrets handling: never hardcode keys. Use environment variables
  (e.g., `ORIGIN_LLM_API_KEY`) and a user-facing editor settings entry that
  stores keys in the OS secure store if necessary. CI must always mock or skip
  any network calls — never put real keys in workflow files.

- Minimal prototype checklist:
  1. Add `editor/llm_plugin/` (or `modules/llm/`) with an opt-in toggle.
  2. Provide a pluggable provider interface (e.g., `LLMProvider` C++ abstract
     class) and one simple provider implementation that reads an env var and
     calls a mockable HTTP client.
  3. Hook completion UI in the script editor to call the provider for
     suggestions (non-blocking; show spinner and allow fallback to existing
     completion engine).
  4. Tests: unit-test the provider with mocked responses; add an editor smoke
     test that verifies the editor doesn't crash when the provider is enabled.

- Naming / rename: renaming the project repository and public names is a
  major, breaking change (CI, docs, build scripts). For now, prefer to add a
  brand/feature layer called "Origin" for the LLM features (menus, settings,
  plugin IDs) and only propose a repo-level rename after core stakeholders,
  CI, and release managers are onboard.

Be explicit in PRs about scope: "Add LLM provider plugin for script editor
— opt-in, mocked in CI, no secrets in repo". This will make review and CI
approval much easier.
