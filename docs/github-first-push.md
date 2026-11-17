# Publishing to GitHub: the first push

An explanation of the Git concepts and every command and option.

## How Git thinks

Your files move between four places:

```
working folder --git add--> staging area --git commit--> local history --git push--> GitHub
(files you edit)            (what the next               (commits on your            (remote copy,
                             commit will contain)         machine)                    called "origin")
```

Splitting `add` from `commit` lets you choose exactly which changes go into each commit.

## 0. One-time identity

```bash
git config --global user.name "Your Name"
git config --global user.email "you@example.com"
```

Every commit records its author. `--global` stores this in `~/.gitconfig` for all repositories. Use an email that is on your GitHub account if you want commits linked to your profile.

## 1. Create an empty repository on GitHub

On github.com click **New repository**, name it `algorithms-lab`, and choose Public or Private. Leave **"Add a README", ".gitignore" and "license" unchecked**: the project already has them, and a second initial commit on GitHub would conflict with your first push.

## 2. First commit

```bash
cd ~/dev/algorithms-lab
git init -b main
git add -A
git status
git commit -m "Initial commit: algorithms lab with 5 challenges"
```

- `git init` turns the folder into a repository by creating a hidden `.git/` folder that holds the history. `-b main` names the first branch `main`, which is GitHub's default.
- `git add -A` stages **all** changes (new, modified and deleted files). It respects `.gitignore`, so `build/` and `.DS_Store` are left out. `git add .` is similar, and `-A` is the explicit "everything in the repo" form.
- `git status` shows what is staged. **Check that no `build/` files appear.** You should see about 60 files such as `README.md`, `LICENSE` and `challenges/...`.
- `git commit -m "message"` saves a snapshot of the staging area into your local history. A good message says what the commit does, in one line.

## 3. Connect and push

Replace `YOUR_USER` with your GitHub username. Pick **one** of the options.

### Option A: SSH

```bash
ssh -T git@github.com
git remote add origin git@github.com:YOUR_USER/algorithms-lab.git
git push -u origin main
```

`ssh -T git@github.com` tests your key. Success says "Hi YOUR_USER! You've successfully authenticated". If it fails, create a key with `ssh-keygen -t ed25519`, then add the contents of `~/.ssh/id_ed25519.pub` on GitHub under Settings, SSH and GPG keys.

### Option B: HTTPS

```bash
git remote add origin https://github.com/YOUR_USER/algorithms-lab.git
git push -u origin main
```

GitHub does not accept your account password over HTTPS. When prompted, enter a **personal access token** (Settings, Developer settings, Personal access tokens), or install the GitHub CLI and run `gh auth login` first.

### Option C: GitHub CLI

This creates the repo and pushes in one step, so **skip step 1** if you use it.

```bash
brew install gh
gh auth login
gh repo create algorithms-lab --public --source=. --remote=origin --push
```

- `--public` or `--private` sets the visibility.
- `--source=.` means "use the current folder".
- `--remote=origin` names the remote `origin`.
- `--push` pushes the current branch immediately.

### What the shared options mean

- `git remote add origin <url>` gives a remote repository the nickname `origin`. By convention that is "the main remote copy".
- `git push -u origin main` uploads your local `main` branch to `origin`. `-u` (short for `--set-upstream`) remembers the link, so later a plain `git push` or `git pull` knows where to go.

## 4. Check that it worked

```bash
git remote -v          # shows the URL behind "origin"
git log --oneline      # your commits, one line each
```

Open `https://github.com/YOUR_USER/algorithms-lab` in the browser, then the **Actions** tab. The CI workflow runs on every push and builds and tests on Ubuntu and macOS. A green check means everything passed.

## 5. Everyday workflow

```bash
git status                 # what changed?
git diff                   # exact changes that are not staged yet
git add -A                 # stage everything (or: git add path/to/file)
git diff --staged          # exact changes that will be committed
git commit -m "Add challenge 06: ..."
git push                   # upload
```

More useful commands:

| Command | Meaning |
|---|---|
| `git log --oneline -10` | The last 10 commits. |
| `git restore <file>` | Throw away uncommitted edits to a file. |
| `git pull` | Download and merge new commits from GitHub. |
| `git clone <url>` | Copy a repository to a new machine. |

A good habit is one commit per challenge, for example `Add challenge 06: top K busiest fields`.

## 6. Troubleshooting

| Symptom | Likely cause | Fix |
|---|---|---|
| `Permission denied (publickey)` | SSH key not set up | Use HTTPS or `gh`, or add your key |
| `rejected ... fetch first` | The GitHub repo isn't empty (it got a README or license) | Delete the GitHub repo and recreate it empty |
| `build/` shows up in `git status` | Wrong folder, or `.gitignore` is missing | Run from the repo root and check `.gitignore` exists |
| `Author identity unknown` | Step 0 not done | Run the two `git config` commands |
| `remote origin already exists` | You ran `git remote add` twice | `git remote set-url origin <url>` |
