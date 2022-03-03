Thanks for your interest! Be sure to check the [forum post](https://powdertoy.co.uk/Discussions/Thread/View.html?Thread=21251) for bugs and features already scheduled for the next release. Or join us on #powder-subframe (on [IRC](https://powdertoy.co.uk/Wiki/W/Irc_setup.html), or on [Discord](https://discord.gg/fjF24Hc)) for discussion about issues, features, or development.

This mod is compiled the same way as the original game. See [the game's wiki](https://powdertoy.co.uk/Wiki/W/Main_Page.html#Powder_Toy_Development_Help) for instructions.

There are two important branches in this repository &mdash; `main` and `dev-rebase`. In short, __make pull requests against `main`__.

To elaborate, `main` has a _stable history_. This means that commits will only ever be appended to it, so you can safely `git pull origin main` to update your local branch. Apart from tracking contributors and regressions, its commit history is generally very uninformative since most development takes place on `dev-rebase`.

`dev-rebase` has an unstable history. Commits are amended, rearranged and squashed all the time. This is necessary because the mod aims to regularly keep up-to-date with the latest upstream (vanilla) version. Merging upstream changes can result in conflicts with features spanning the entire history of the mod, resulting in extra work or even bugs.

The solution is to maintain a clean commit history with a minimal footprint on the game's codebase. Whenever a feature is refactored without a significant change in functionality, the refactor is squashed with the feature's original commit and cleaned to minimize opportunities for merge conflicts. As a side effect, this makes `dev-rebase` a great branch to study to understand the mod's internal features and modifications, though note that contributor information is naturally absent.

Since `dev-rebase` has an unstable history, you cannot pull it directly to update a local branch. It's unlikely that you'll ever need to maintain a local copy of `dev-rebase`, but if you do find yourself wanting to update one, you'll need to do a hard pull:

```
git fetch origin dev-rebase
git reset --hard origin/dev-rebase
```

__WARNING:__ Make sure that you're on the right branch. Note that any work on your local branch, even committed work, will be lost.
