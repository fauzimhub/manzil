# Manzil 

## Description
Manzil, Unofficial Open Source Quranite Reader.   

Quran reader for Web and PC (Linux and Windows), made with Tauri and React

based on : [Official Quranite Reader](https://reader.quranite.com/) by Sam Gerrans

## References
- Quranite:

    https://quranite.com/

- Quran Svelte by [[abnewstein](https://github.com/abnewstein)], for json and quranite scraper:

    https://github.com/abnewstein/quran-svelte

- Add fonts in react:

    https://stackoverflow.com/questions/41676054/how-to-add-fonts-to-create-react-app-based-projects

- Add tauri to existing project:

    https://tauri.app/start/create-project/#manual-setup-tauri-cli

- Tauri WebKitGTK + Nvidia + Wayland issue: 

    https://github.com/tauri-apps/tauri/issues/9394

- Deploy to vercel:

    https://medium.com/better-programming/how-to-deploy-an-existing-application-with-vercel-cfcd314e5396

- Rust profiles:

    https://doc.rust-lang.org/cargo/reference/profiles.html

- Small rust binary:

    https://docs.rust-embedded.org/book/unsorted/speed-vs-size.html
    https://github.com/johnthagen/min-sized-rust
    https://nnethercote.github.io/perf-book/build-configuration.html

- Tauri NO_STRIP issue:

    https://github.com/tauri-apps/tauri/issues/13258

- Tauri Windows Installer:

    https://v2.tauri.app/distribute/windows-installer/

## Usage
### Development 
``` sh
git clone https://github.com/fauzimhub/manzil.git
cd manzil
bun tauri dev

 # for web access http://localhost:3000/ on browser
```

### Build
``` sh
git clone https://github.com/fauzimhub/manzil.git
cd manzil
bun tauri build

 # to build for Windows PC on Linux
 # refer to https://v2.tauri.app/distribute/windows-installer/
bun tauri build --runner cargo-xwin --target x86_64-pc-windows-msvc

 # browser
bunx serve dist 

 # PC Linux
./src-tauri/target/release/manzil

 # PC Windows
./src-tauri/target/x86_64-pc-windows-msvc/release/manzil.exe
```
Notes : bundle disabled on [tauri.conf.json](./src-tauri/tauri.conf.json) 

``` json
  "bundle": {
    "active": false,
    "targets": "all",
    "icon": [
      "icons/32x32.png",
      "icons/128x128.png",
      "icons/128x128@2x.png",
      "icons/icon.icns",
      "icons/icon.ico"
    ]
  }
```
you can active the bundle by changing 
``` json
       "active": true,
```
to build for AppImage, rpm and deb, but for AppImage especially beware of [Tauri NO_STRIP issue](https://github.com/tauri-apps/tauri/issues/13258)

## LICENSE 
[License](./LICENSE)
