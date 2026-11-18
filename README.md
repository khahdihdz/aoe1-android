# AoE1 Android + Web

Native Android and browser-port foundations for an open-source Age of Empires 1-compatible RTS engine. The web target is being developed alongside Android in this repository.

## Project status

- **Android:** SDL2 + OpenGL ES native shell and ARM64 APK workflow.
- **Web:** responsive launcher, browser capability checks, GitHub Pages and Cloudflare Pages deployment workflows.
- **Game engine:** the upstream C++ engine has been audited, but is not yet integrated or compiled to WebAssembly. The current web page is a development launcher, not a playable game.

## Upstream engine

The planned engine source is [FolkertVanVerseveld/aoe](https://github.com/FolkertVanVerseveld/aoe), licensed under Apache-2.0. It uses C++17, SDL2, SDL2_mixer and OpenGL. Its current README reports basic gameplay and limited computer-player behavior. A legitimate copy of AoE / Rise of Rome data is required; proprietary assets are not included here.

## Web preview

- Local preview: serve the `web/` directory with any static HTTP server.
- GitHub Pages: enable Pages with **GitHub Actions** as the build/deployment source, then run the **Web launcher - GitHub Pages** workflow.
- Cloudflare Pages: create a Pages project named `aoe1-web` and add repository secrets `CLOUDFLARE_API_TOKEN` and `CLOUDFLARE_ACCOUNT_ID`. The workflow deploys the `web/` directory.

## Porting plan

See [docs/WEB_PORT_PLAN.md](docs/WEB_PORT_PLAN.md) for the source audit, Emscripten strategy, browser networking constraints and staged milestones.

## Android build

See [docs/BUILD.md](docs/BUILD.md).