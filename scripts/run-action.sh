#!/usr/bin/env bash
set -euo pipefail

workspace="${GITHUB_WORKSPACE:-$PWD}"
action_path="${GITHUB_ACTION_PATH:?GITHUB_ACTION_PATH is required}"
build_dir="${RUNNER_TEMP:-/tmp}/archivum-build"
mode="${INPUT_MODE:-pull_request}"
head_sha="${INPUT_HEAD_SHA:-${GITHUB_SHA:-HEAD}}"
base_sha="${INPUT_BASE_SHA:-}"
branch_prefix="${INPUT_BRANCH_PREFIX:-archivum/docs}"
commit_message="${INPUT_COMMIT_MESSAGE:-docs: update ArchivumDocs}"
pr_title="${INPUT_PR_TITLE:-docs: update ArchivumDocs}"

if [[ -n "${INPUT_OPENAI_API_KEY:-}" ]]; then
  export OPENAI_API_KEY="$INPUT_OPENAI_API_KEY"
fi

if [[ -n "${INPUT_OPENROUTER_API_KEY:-}" ]]; then
  export OPENROUTER_API_KEY="$INPUT_OPENROUTER_API_KEY"
  export ARCHIVUM_PROVIDER="openrouter"
fi

if [[ -n "${INPUT_MODEL:-}" ]]; then
  export ARCHIVUM_MODEL="$INPUT_MODEL"
fi

if [[ -n "${INPUT_PROJECT_NAME:-}" ]]; then
  export ARCHIVUM_PROJECT_NAME="$INPUT_PROJECT_NAME"
fi

if [[ -n "${INPUT_PROJECT_TAGLINE:-}" ]]; then
  export ARCHIVUM_PROJECT_TAGLINE="$INPUT_PROJECT_TAGLINE"
fi

export ARCHIVUM_WRITE_MODE="$mode"

cmake -B "$build_dir" -S "$action_path" -DCMAKE_BUILD_TYPE=Release -DARCHIVUM_BUILD_TESTS=OFF
cmake --build "$build_dir" --config Release -j"$(nproc 2>/dev/null || echo 2)"

cd "$workspace"

if [[ -z "$base_sha" || "$base_sha" == "0000000000000000000000000000000000000000" ]]; then
  base_sha="$(git rev-parse HEAD~1 2>/dev/null || true)"
fi
if [[ -z "$base_sha" ]]; then
  base_sha="FIRST_COMMIT"
fi

"$build_dir/archivum" --config "${INPUT_CONFIG_PATH:-.archivum.yml}" "$base_sha" "$head_sha"

if [[ -z "$(git status --porcelain)" ]]; then
  echo "docs_changed=false" >> "$GITHUB_OUTPUT"
  echo "pull_request_url=" >> "$GITHUB_OUTPUT"
  exit 0
fi

echo "docs_changed=true" >> "$GITHUB_OUTPUT"

git config user.name "archivumdocs[bot]"
git config user.email "archivumdocs[bot]@users.noreply.github.com"

case "$mode" in
  none|shadow)
    echo "pull_request_url=" >> "$GITHUB_OUTPUT"
    exit 0
    ;;
  direct|commit)
    git add -A
    git commit -m "$commit_message"
    git push
    echo "pull_request_url=" >> "$GITHUB_OUTPUT"
    exit 0
    ;;
    pull_request|pr|auto_merge)
    current_branch="$(git rev-parse --abbrev-ref HEAD)"
    if [[ "$current_branch" == "${branch_prefix}"* ]]; then
      echo "Already on a documentation branch. Skipping to avoid recursion."
      exit 0
    fi

    branch="${branch_prefix}-update"
    git checkout -B "$branch"
    git add -A
    
    if git diff --staged --quiet; then
      echo "docs_changed=false" >> "$GITHUB_OUTPUT"
      exit 0
    fi

    git commit -m "$commit_message"
    git push origin "$branch" --force
    
    export GH_TOKEN="${INPUT_GITHUB_TOKEN:-${GITHUB_TOKEN:-}}"
    base_branch="${GITHUB_BASE_REF:-$(git remote show origin | sed -n '/HEAD branch/s/.*: //p')}"
    base_branch="${base_branch:-main}"
    
    # Check if PR already exists
    existing_pr="$(gh pr list --head "$branch" --base "$base_branch" --json url --jq '.[0].url')"
    if [[ -z "$existing_pr" ]]; then
      pr_url="$(gh pr create --title "$pr_title" --body "ArchivumDocs updated documentation from the current code graph." --head "$branch" --base "$base_branch")"
    else
      pr_url="$existing_pr"
    fi
    
    echo "pull_request_url=$pr_url" >> "$GITHUB_OUTPUT"
    if [[ "${INPUT_AUTO_MERGE:-false}" == "true" || "$mode" == "auto_merge" ]]; then
      gh pr merge "$pr_url" --auto --squash >/dev/null
    fi
    ;;
  *)
    echo "Unsupported ArchivumDocs mode: $mode" >&2
    exit 1
    ;;
esac
