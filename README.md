![ArchivumDocs Banner](https://github.com/LonelyGuy-SE1/ArchivumDocs/blob/main/docs/assets/archivum_banner.gif)

# ArchivumDocs

ArchivumDocs is an AST-driven documentation engine for GitHub Actions. It scans the exact Git range for a pull request or merge, builds a repository symbol graph with tree-sitter and libgit2, expands the blast radius through inferred dependencies, and updates only the documentation context that changed.

The default workflow is safe for production repositories: generated documentation is committed to a pull request. Teams that want direct writes can opt into `direct` mode after they trust their branch protections and review flow.

## What It Does

- Creates `docs/` automatically when it does not exist.
- Updates `docs/index.md`, `docs/symbols/`, and `docs/archivum-manifest.json`.
- Uses deterministic local documentation when no AI key is configured.
- Uses an OpenAI-compatible Responses endpoint when credentials are present.
- Limits context by affected symbols and byte budget so large repositories do not send full source trees to the model.
- Publishes as a composite GitHub Action through the root `action.yml`.

## Quick Start

Create `.github/workflows/archivum.yml` in your repository:

```yaml
name: ArchivumDocs

on:
  push:
    branches: [main]
  pull_request:
    branches: [main]

permissions:
  contents: write
  pull-requests: write

jobs:
  docs:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
        with:
          fetch-depth: 2
      - uses: LonelyGuy-SE1/ArchivumDocs@v1
        with:
          openai-api-key: ${{ secrets.OPENAI_API_KEY }}
```

Add `.archivum.yml` when you want to change defaults:

```yaml
docs_dir: docs
provider: auto
endpoint: https://api.openai.com/v1/responses
model: gpt-5.5
write_mode: pull_request
max_context_bytes: 180000
max_symbols: 250
```

## Configuration

| Key | Default | Purpose |
| --- | --- | --- |
| `docs_dir` | `docs` | Documentation output directory. |
| `index_file` | `index.md` | Generated documentation landing page. |
| `symbols_dir` | `symbols` | Folder for generated symbol pages. |
| `manifest_file` | `archivum-manifest.json` | Machine-readable run summary. |
| `provider` | `auto` | `auto`, `openai`, `custom`, `shadow`, or `none`. |
| `endpoint` | `https://api.openai.com/v1/responses` | OpenAI-compatible Responses endpoint. |
| `model` | `gpt-5.5` | Model used for the final documentation update. |
| `api_key_env` | `OPENAI_API_KEY` | Environment variable containing the provider key. |
| `write_mode` | `pull_request` | `pull_request`, `auto_merge`, `direct`, or `none`. |
| `max_context_bytes` | `180000` | Maximum code context sent to the model. |
| `max_symbols` | `250` | Maximum downstream symbols documented in one run. |
| `fail_on_provider_error` | `false` | Fail the run when the AI endpoint fails instead of using deterministic docs. |
| `update_docs` | `true` | Disable file writes while keeping analysis enabled. |

Environment variables with the `ARCHIVUM_` prefix can override the same settings in CI.

## GitHub Action Inputs

| Input | Default | Purpose |
| --- | --- | --- |
| `config-path` | `.archivum.yml` | Config file path. |
| `openai-api-key` | empty | Provider key, usually `${{ secrets.OPENAI_API_KEY }}`. |
| `github-token` | `${{ github.token }}` | Token for branches and pull requests. |
| `mode` | `pull_request` | `pull_request`, `auto_merge`, `direct`, or `none`. |
| `auto-merge` | `false` | Enables GitHub auto-merge for generated docs PRs. |
| `base-sha` | event or `HEAD~1` | Base commit for diffing. |
| `head-sha` | `${{ github.sha }}` | Head commit for diffing. |
| `branch-prefix` | `archivum/docs` | Branch prefix for generated docs PRs. |
| `commit-message` | `docs: update ArchivumDocs` | Commit message. |
| `pr-title` | `docs: update ArchivumDocs` | Pull request title. |

## Local Development

```powershell
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --output-on-failure
.\build\archivum.exe
```

On Linux:

```bash
scripts/verify.sh build
```

## Marketplace Release

The repository root contains `action.yml`, including branding, inputs, outputs, and composite runtime metadata. The repository is kept Marketplace-ready with a single root metadata file and no workflow files. To publish, create a release from a semantic version tag and publish the Action from GitHub’s release UI.
