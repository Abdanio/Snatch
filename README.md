<div align="center">

# 🎵 SNATCH v1.0

**Fast, Simple, and Powerful Audio Downloader**

![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)
![Language](https://img.shields.io/badge/language-C-orange.svg)

*A minimal yet powerful command-line tool for downloading high-quality audio from YouTube, SoundCloud, and Spotify (via search).*

[Features](#-features) • [Quick Start](#-quick-start) • [Usage](#-usage) • [Roadmap](#-roadmap) • [Contributing](#-contributing)

</div>

---

## 📋 Preview

<div align="center">
  <img src="preview.PNG" alt="SNATCH v1.0 Preview" width="800">
  <p><i>SNATCH v1.0 in action - Beautiful, colorful, and easy to use!</i></p>
</div>

---

## ✨ Features

### 🎯 Core Capabilities
- **🎬 YouTube Support**: Direct download from YouTube videos and playlists
- **🎧 SoundCloud Integration**: Download tracks from SoundCloud
- **🎵 Spotify Search**: Search and download via YouTube Music (no premium required!)
- **🎼 High Quality Audio**: MP3 at ~320kbps using best available quality
- **📝 Auto Metadata**: Automatically embeds title, artist, and album art
- **🎨 Colorful CLI**: Beautiful, interactive command-line interface

### 🚀 Smart Features
- **⚡ Zero-Config Setup**: Auto-downloads `yt-dlp` and `ffmpeg` on first run (Windows)
- **📂 Custom Output**: Choose your download destination
- **🔄 Progress Tracking**: Real-time download progress bar
- **🛡️ Error Handling**: Smart error detection and helpful tips

---

## 📋 System Requirements

| Platform | Requirements |
|----------|-------------|
| **Windows** | • Windows 7 or later<br>• Internet connection (for auto-setup)<br>• ~30MB free disk space |
| **Linux** | • GCC compiler<br>• `yt-dlp` and `ffmpeg` installed<br>• Terminal emulator |
| **macOS** | • Xcode Command Line Tools<br>• `yt-dlp` and `ffmpeg` (via Homebrew) |

---

## 🚀 Quick Start

### Windows (Recommended for Beginners)

1. **Download** the latest release or compile from source
2. **Run** `snatch.exe`
3. **First Run**: The program will automatically download dependencies (~30MB)
4. **Paste** your link and enjoy! 🎉

```cmd
# Compile from source
gcc snatch.c -o snatch.exe

# Run
snatch.exe
```

### Linux / macOS

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt update
sudo apt install yt-dlp ffmpeg gcc

# Or on macOS with Homebrew
brew install yt-dlp ffmpeg

# Compile
gcc snatch.c -o snatch

# Run
./snatch
```

---

## 💡 Usage

### Basic Workflow

1. **Launch** the program
2. **Paste** your media link when prompted
3. **Choose** output folder (or press Enter for default)
4. **Wait** for the magic to happen! ✨

### Supported Platforms

| Platform | Input | Example |
|----------|-------|---------|
| YouTube | Direct video URL | `https://youtube.com/watch?v=...` |
| SoundCloud | Track URL | `https://soundcloud.com/artist/track` |
| Spotify | Track URL + Manual Search | Input: Spotify link → Prompted for "Artist - Title" |

### Example Session

```
 [>] Paste Link (YouTube/SoundCloud/Spotify): https://youtube.com/watch?v=dQw4w9WgXcQ
 [+] Output Folder (default: 'downloads'): my_music

 [*] Initializing SNATCH engine...
 [>] Target: https://youtube.com/watch?v=dQw4w9WgXcQ
 [+] Output: my_music
 
 [>>] Downloading & Converting... Please wait

 [SUCCESS] Download complete!
 [FILE] Saved in 'my_music' folder.
```

---

## 🗺️ Roadmap

### Version 1.0 ✅ (Current)
- [x] YouTube audio download
- [x] SoundCloud support
- [x] Spotify search integration
- [x] Auto-setup (Windows)
- [x] Metadata embedding
- [x] Colorful CLI interface

### Version 1.5 🔄 (In Progress)
- [ ] **Queue System**: Download multiple links in batch
- [ ] **Configuration File**: Save preferences (output folder, quality, etc.)
- [ ] **Download History**: Keep track of downloaded files
- [ ] **Retry Mechanism**: Auto-retry failed downloads

### Version 2.0 🎯 (Planned)
- [ ] **Video Download**: Option to download video (MP4)
- [ ] **GUI Interface**: Graphical user interface for non-CLI users
- [ ] **Playlist Support**: Download entire playlists with one command
- [ ] **Format Selection**: Choose between MP3, FLAC, WAV, etc.

### Version 3.0 🚀 (Future)
- [ ] **Instagram Support**: Download audio from Instagram videos
- [ ] **TikTok Integration**: Extract audio from TikTok videos
- [ ] **Subtitle Download**: Get subtitles/lyrics when available
- [ ] **Multi-language Support**: Interface in multiple languages

---

## ❓ FAQ

<details>
<summary><b>Q: Do I need to install yt-dlp or ffmpeg manually?</b></summary>

**A:** On Windows, NO! SNATCH will automatically download and set up everything for you on the first run. On Linux/macOS, you need to install them via your package manager.
</details>

<details>
<summary><b>Q: Can I download full playlists?</b></summary>

**A:** Not yet in v1.0. Playlist support is planned for v2.0. Currently, SNATCH downloads single tracks only.
</details>

<details>
<summary><b>Q: Why does Spotify require me to enter "Artist - Title"?</b></summary>

**A:** SNATCH doesn't directly download from Spotify (requires premium API). Instead, it searches YouTube Music for the song, which is free and legal.
</details>

<details>
<summary><b>Q: Is this legal?</b></summary>

**A:** Downloading copyrighted content may violate terms of service. SNATCH is a tool; users are responsible for complying with applicable laws and terms of service in their jurisdiction.
</details>

<details>
<summary><b>Q: The download failed. What should I do?</b></summary>

**A:** Check the troubleshooting section below. Most issues are related to missing `ffmpeg` or network problems.
</details>

---

## 🛠️ Troubleshooting

### ❌ "yt-dlp is not recognized..."
- **Windows**: Let SNATCH auto-download it on first run
- **Linux/macOS**: Install via `apt install yt-dlp` or `brew install yt-dlp`

### ❌ "Download failed" or "ffmpeg error"
- Ensure `ffmpeg.exe` is in the same folder as `snatch.exe` (Windows)
- Or install globally: `apt install ffmpeg` (Linux) / `brew install ffmpeg` (macOS)

### ❌ Colors not showing correctly
- Use Windows Terminal or a modern terminal emulator
- Standard CMD may have limited color support

### ❌ "Permission denied"
- Run as administrator (Windows)
- Use `chmod +x snatch` (Linux/macOS)

---

## 🤝 Contributing

SNATCH is an **open-source project** and contributions are welcome!

### How to Contribute
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Ideas for Contributions
- 🐛 Bug fixes
- ✨ New features from the roadmap
- 📚 Documentation improvements
- 🌍 Translations
- 🎨 UI/UX enhancements

---

## 📄 License

This project is licensed under the **MIT License** - see the LICENSE file for details.

**What this means:**
- ✅ Use commercially
- ✅ Modify and distribute
- ✅ Private use
- ⚠️ No warranty provided

---

## 🙏 Acknowledgments

- **[yt-dlp](https://github.com/yt-dlp/yt-dlp)** - The powerful backend for downloading
- **[FFmpeg](https://ffmpeg.org/)** - Media processing magic
- **Community Contributors** - Thank you for making SNATCH better!

---

## 📞 Support

Having issues or questions?

- 💬 Open an [Issue](../../issues) on GitHub
- 📧 Contact: *[your-email@example.com]*
- ⭐ Star this project if you find it useful!

---

<div align="center">

**Made with ❤️ by Abdanio Parchezzi**

⭐ Star this repository if SNATCH helped you!

[⬆ Back to Top](#-snatch-v10)

</div>
