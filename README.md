# 🦅 SNATCH v1.7 - Modern Multi-Platform Downloader

<div align="center">

![SNATCH Logo](images/snatch_logo.png)

[![Version](https://img.shields.io/badge/version-1.7.0-blue.svg)](https://github.com/oktramdani/snatch)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)
[![Supported Sites](https://img.shields.io/badge/supported%20sites-1000+-brightgreen.svg)](https://github.com/yt-dlp/yt-dlp/blob/master/supportedsites.md)

**A powerful, modern, and professional multi-platform downloader with beautiful Dear ImGui interface and smooth animations**

[Features](#-features) • [Installation](#-installation) • [Usage](#-usage) • [Supported Platforms](#-supported-platforms) • [Contact](#-contact)

</div>

---

## ✨ What's New in v1.7

SNATCH v1.7 brings **major enhancements** with support for **1000+ websites** and **smooth UI animations**! This version transforms the application from a YouTube-only downloader to a universal multi-platform video downloader.

### 🎨 Major Improvements

- **🌐 Multi-Platform Support**: Download from 1000+ websites including YouTube, Instagram, TikTok, Facebook, Twitter, Vimeo, and more!
- **✨ Smooth Animations**: Beautiful fade-in, pulse, and glow effects throughout the UI
- **🎯 Animated UI Elements**: Pulsing buttons, animated progress bars, and smooth transitions
- **🏷️ Platform Badges**: Color-coded platform detection with animated badges
- **⚡ Lightweight Effects**: GPU-accelerated animations that don't impact performance
- **🎨 Enhanced Visual Feedback**: Glowing borders, pulsing icons, and animated colors
- **📱 Universal Downloads**: One tool for all your video downloading needs

---

## 🚀 Features

### Core Functionality
- ✅ Download videos from **1000+ websites** (YouTube, Instagram, TikTok, Facebook, Twitter, Vimeo, Reddit, and many more!)
- ✅ Multiple quality options (4K, 2K, 1080p, 720p, 480p, 360p, 240p)
- ✅ Extract audio from videos
- ✅ Batch download with queue system (up to 50 items)
- ✅ Real-time download progress tracking
- ✅ Automatic platform detection
- ✅ FFmpeg integration for format conversion

### Modern UI with Animations (Dear ImGui)
- 🎨 Beautiful dark theme with professional styling
- ✨ Smooth fade-in, pulse, and glow animations
- ⚡ 60 FPS GPU-accelerated interface
- 🏷️ Color-coded platform badges
- 🎯 Animated progress bars and buttons
- 📱 Responsive window layout
- 🖱️ Intuitive drag-and-drop support
- 🎯 Custom Segoe UI font for clarity

### Data Management
- 💾 SQLite database for history tracking
- 🔍 Search functionality in history
- 📊 Statistics dashboard (total downloads, data usage)
- 📁 Customizable download paths
- 🗑️ One-click history clearing

### Advanced Features
- ⚙️ Configurable settings (quality, notifications, auto-clear)
- 🔔 System notifications on completion
- 🎵 Audio extraction mode
- 📥 Concurrent downloads (1-5 simultaneous)
- 🔄 Auto-update checker
- 🌐 Welcome screen for first-time users

---

## 🌐 Supported Platforms

SNATCH v1.7 leverages **yt-dlp** to support **1000+ websites**! Here are some of the most popular:

### 📹 Video Platforms
- **YouTube** - World's largest video platform
- **Vimeo** - High-quality video hosting
- **Dailymotion** - European video platform
- **Twitch** - Live streaming and gaming videos

### 📱 Social Media
- **Instagram** - Photos, videos, stories, and reels
- **TikTok** - Short-form videos
- **Facebook** - Videos and live streams
- **Twitter/X** - Videos and GIFs
- **Reddit** - Videos from subreddits
- **LinkedIn** - Professional videos
- **Pinterest** - Video pins
- **Tumblr** - Video posts

### 🎵 Audio Platforms
- **SoundCloud** - Music and podcasts
- **Bandcamp** - Independent music
- **Mixcloud** - DJ mixes and radio shows

### 🌏 Regional Platforms
- **Bilibili** - Chinese video sharing
- **VK** - Russian social network
- **9GAG** - Memes and entertainment

### 📺 And Many More!
- Streamable, Imgur, Archive.org, BBC iPlayer, ESPN, CNN, and 1000+ more sites
- For a complete list, visit: [yt-dlp Supported Sites](https://github.com/yt-dlp/yt-dlp/blob/master/supportedsites.md)

---

## 💻 Technical Specifications

### Built With
- **UI Framework**: [Dear ImGui](https://github.com/ocornut/imgui) v1.90.1
- **Rendering**: DirectX 11
- **Database**: SQLite 3.51.1
- **Downloader**: yt-dlp (latest)
- **Media Processing**: FFmpeg (latest)
- **Language**: C++11
- **Compiler**: MinGW GCC 13.2.0

### System Requirements
- **OS**: Windows 7/8/10/11 (64-bit)
- **RAM**: 256 MB minimum
- **Storage**: 50 MB for application + space for downloads
- **Graphics**: DirectX 11 compatible GPU
- **.NET**: Not required (standalone executable)

---

## 📥 Installation

### Option 1: Pre-built Executable (Recommended)
1. Download `snatch_v1.7.exe` from the [Releases](https://github.com/oktramdani/snatch/releases) page
2. Download `yt-dlp.exe` from [yt-dlp releases](https://github.com/yt-dlp/yt-dlp/releases)
3. Download `ffmpeg.exe` from [FFmpeg builds](https://www.gyan.dev/ffmpeg/builds/)
4. Place all three files in the same folder
5. Run `snatch_v1.7.exe`

### Option 2: Build from Source
```bash
# Clone the repository
git clone https://github.com/oktramdani/snatch.git
cd snatch

# Compile SQLite (one-time)
gcc -c sqlite3.c -o sqlite3.o -O2

# Build SNATCH v1.7
g++ -o snatch_v1.7.exe snatch_v1.6_dark.cpp imgui.cpp imgui_draw.cpp ^
    imgui_tables.cpp imgui_widgets.cpp backends/imgui_impl_win32.cpp ^
    backends/imgui_impl_dx11.cpp sqlite3.o -I. -I./backends ^
    -ld3d11 -ldxgi -ld3dcompiler -lcomctl32 -lshell32 -lgdi32 ^
    -lcomdlg32 -lole32 -lwinmm -ldwmapi -lstdc++ -std=c++11 -O2 -mwindows
```

### Required Dependencies
- **yt-dlp.exe**: Must be in the same folder as SNATCH (supports 1000+ websites)
- **ffmpeg.exe**: Must be in the same folder as SNATCH
- Both are detected automatically on startup

---

## 🎯 Usage

### Quick Start
1. Launch **SNATCH v1.7**
2. Read the welcome message (first launch)
3. Paste a video URL from any supported platform (YouTube, Instagram, TikTok, etc.)
4. Select desired quality and type (Video/Audio)
5. Click **"Add to Queue"** - watch the smooth fade-in animation!
6. Click **"Start Download"** to begin
7. Enjoy the animated progress bars and wait for completion notification

### Interface Overview

#### Main Window
- **URL Input**: Paste any video URL from 1000+ supported websites
- **Platform Detection**: Automatically detects and displays the platform with color-coded badges
- **Quality Selector**: Choose from Best/4K/2K/1080p/720p/480p/360p/240p
- **Type Selector**: Video or Audio extraction
- **Queue List**: View all pending downloads with animated progress bars
- **Action Buttons**: Settings, History, Statistics, About (all with smooth hover effects)

#### Animations
- **Logo Pulse**: Subtle breathing effect on the app logo
- **Button Glow**: Pulsing glow effect on interactive buttons
- **Progress Bars**: Animated progress indicators with smooth transitions
- **Platform Badges**: Color-coded badges that pulse during downloads
- **Fade-In Effects**: Smooth fade-in when adding items to queue

#### Settings Window
- **Default Quality**: Set preferred download quality
- **Download Path**: Customize save location
- **Auto Clear Queue**: Clear completed downloads automatically
- **Enable Notifications**: System notifications on completion
- **Max Concurrent Downloads**: 1-5 simultaneous downloads
- **Auto Check Updates**: Automatic update detection

#### History Window
- **Search Bar**: Find previous downloads by URL or title
- **History List**: View all past downloads with timestamps
- **Clear History**: Remove all history entries

#### Statistics Window
- **Total Downloads**: Number of completed downloads
- **Total Data**: Cumulative download size
- **Current Queue**: Active queue items

---

## 📸 Screenshots

### Main Interface
![Main Window](images/snatch_main.png)
*Clean, modern interface with dark theme*

### Download Queue
![Queue Management](images/snatch_queue.png)
*Visual progress tracking for multiple downloads*

### Settings Panel
![Settings](images/snatch_settings.png)
*Comprehensive configuration options*

### History Database
![History](images/snatch_history.png)
*SQLite-powered download history with search*

---

## 🎨 UI/UX Design

### Color Palette
- **Background**: `#1A1A21` (Very dark blue-gray)
- **Secondary**: `#1F1F27` (Slightly lighter)
- **Accent**: `#428BFF` (Bright blue)
- **Text**: `#F3F4F8` (Near white)
- **Border**: `#33333F` (Subtle separator)

### Typography
- **Font Family**: Segoe UI
- **Font Size**: 18px base
- **Weight**: Regular (400)
- **Rendering**: ClearType antialiasing

### Visual Effects
- **Window Rounding**: 8px
- **Button Rounding**: 6px
- **Frame Rounding**: 6px
- **Shadows**: Subtle elevation
- **Animations**: Smooth transitions

---

## ⚙️ Configuration Files

### Settings File (`snatch_settings.ini`)
```ini
defaultQuality=3
downloadPath=downloads
autoClearQueue=0
enableNotifications=1
maxConcurrentDownloads=3
autoCheckUpdates=1
totalDownloads=0
totalBytes=0
```

### History Database (`snatch_history.db`)
SQLite database with schema:
```sql
CREATE TABLE history (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    url TEXT NOT NULL,
    title TEXT,
    type TEXT,
    quality TEXT,
    timestamp TEXT,
    size INTEGER
);
```

---

## 🔧 Troubleshooting

### Application Won't Start
- **Missing DLL**: Install [Visual C++ Redistributable](https://aka.ms/vs/17/release/vc_redist.x64.exe)
- **DirectX Error**: Update graphics drivers
- **Access Denied**: Run as administrator

### Download Fails
- **yt-dlp not found**: Ensure `yt-dlp.exe` is in the same folder
- **FFmpeg error**: Verify `ffmpeg.exe` is present
- **Network error**: Check internet connection
- **Age-restricted video**: yt-dlp may need cookies file

### Performance Issues
- **Slow UI**: Update graphics drivers
- **High CPU**: Reduce concurrent downloads in settings
- **Memory usage**: Clear history regularly

### Common Errors
```
Error: "yt-dlp.exe not found!"
Solution: Download yt-dlp.exe and place in same folder

Error: "ffmpeg.exe not found!"
Solution: Download ffmpeg.exe and place in same folder

Error: "Failed to initialize DirectX 11"
Solution: Update graphics drivers or check GPU compatibility
```

---

## 🆚 Version Comparison

### SNATCH v1.7 vs v1.6

| Feature | v1.6 | v1.7 |
|---------|------|------|
| **Supported Sites** | YouTube only | 1000+ websites |
| **Platform Detection** | ❌ | ✅ Automatic |
| **Animations** | Static | Smooth & Dynamic |
| **UI Effects** | Basic | Pulse, Glow, Fade-in |
| **Platform Badges** | ❌ | ✅ Color-coded |
| **Progress Bars** | Static | Animated |
| **Button Effects** | Static | Pulsing glow |
| **Logo Animation** | Static | Breathing effect |
| **Performance** | Excellent | Excellent (GPU) |

### Key Advantages of v1.7
- ✨ **1000x More Sites**: Download from anywhere, not just YouTube
- 🎨 **Better UX**: Smooth animations provide better visual feedback
- 🎯 **Smarter UI**: Platform detection helps you know what you're downloading
- ⚡ **Same Performance**: Animations are GPU-accelerated and lightweight
- 🏷️ **Better Organization**: Color-coded badges make queue management easier

---

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/AmazingFeature`)
3. **Commit** your changes (`git commit -m 'Add some AmazingFeature'`)
4. **Push** to the branch (`git push origin feature/AmazingFeature`)
5. **Open** a Pull Request

### Development Setup
```bash
# Install MinGW GCC
# Download Dear ImGui v1.90.1
# Clone repository
git clone https://github.com/oktramdani/snatch.git

# Build
cd snatch/imgui
./build_v1.6.bat
```

---

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

Special thanks to the amazing open-source projects that made SNATCH v1.6 possible:

### Core Dependencies
- **[Dear ImGui](https://github.com/ocornut/imgui)** - Omar Cornut (@ocornut)
  - *The incredible immediate mode GUI library that powers our beautiful interface*
  - *Thank you for creating such an elegant and powerful UI framework!*
  
- **[yt-dlp](https://github.com/yt-dlp/yt-dlp)** - yt-dlp team
  - *The most advanced YouTube downloader*
  
- **[FFmpeg](https://ffmpeg.org/)** - FFmpeg developers
  - *Universal media processing toolkit*
  
- **[SQLite](https://www.sqlite.org/)** - D. Richard Hipp
  - *Lightweight embedded database*

### Inspiration
- Material Design by Google
- Windows 11 Fluent Design
- Modern UI/UX principles

---

## 📞 Contact & Support

### Developer Information
- **Name**: Oktavianus Ramdani
- **Email**: [cheeksparchezzi@gmail.com](mailto:cheeksparchezzi@gmail.com)
- **Instagram**: [@oktramdani](https://instagram.com/oktramdani)
- **GitHub**: [@oktramdani](https://github.com/oktramdani)

### Get Help
- 🐛 **Bug Reports**: [GitHub Issues](https://github.com/oktramdani/snatch/issues)
- 💬 **Discussions**: [GitHub Discussions](https://github.com/oktramdani/snatch/discussions)
- 📧 **Email Support**: cheeksparchezzi@gmail.com
- 📱 **Instagram DM**: @oktramdani

### Stay Updated
- ⭐ Star this repository to show support
- 👀 Watch for new releases
- 🔔 Follow on Instagram for updates

---

## 📊 Project Status

- **Current Version**: 1.7.0
- **Status**: ✅ Active Development
- **Last Updated**: March 10, 2026
- **Stability**: Stable Release
- **Platform**: Windows (7/8/10/11)
- **Supported Sites**: 1000+ via yt-dlp

---

## 🎯 Roadmap

### Completed Features ✅
- [x] YouTube downloads (v1.0-1.6)
- [x] Dear ImGui UI with GPU acceleration (v1.6)
- [x] Multi-platform support for 1000+ sites (v1.7)
- [x] Smooth UI animations and effects (v1.7)
- [x] Platform detection and color-coded badges (v1.7)

### Planned Features
- [ ] Playlist download support (v1.8)
- [ ] Channel downloads (v1.8)
- [ ] Multi-language support (Indonesian, English, etc.) (v1.8)
- [ ] Custom theme editor (v1.9)
- [ ] Download scheduler (v1.9)
- [ ] Bandwidth limiter (v1.9)
- [ ] Browser extension integration (v2.0)
- [ ] Cloud storage integration (v2.0)
- [ ] Cross-platform support (Linux, macOS) (v2.0)
- [ ] Mobile companion app (v2.1)

### Future Versions
- **v1.8**: Playlist & channel downloads, multi-language
- **v1.9**: Advanced features (scheduler, bandwidth limiter, themes)
- **v2.0**: Cross-platform support

---

<div align="center">

### ⭐ If you like SNATCH, please give it a star!

**Made with ❤️ by [Abdanio Parchezzi](https://instagram.com/oktramdani)**

*Powered by Dear ImGui • Built with C++ • Designed for Windows • Supporting 1000+ Sites*

**SNATCH v1.7 - Download from Anywhere with Style! ✨**

[⬆ Back to Top](#-snatch-v17---modern-multi-platform-downloader)

</div>
