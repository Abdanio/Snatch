# 🦅 SNATCH v1.6 - Modern YouTube Downloader

<div align="center">

![SNATCH Logo](images/snatch_logo.png)

[![Version](https://img.shields.io/badge/version-1.6.0-blue.svg)](https://github.com/oktramdani/snatch)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)

**A powerful, modern, and professional YouTube downloader with beautiful Dear ImGui interface**

[Features](#-features) • [Installation](#-installation) • [Usage](#-usage) • [Screenshots](#-screenshots) • [Contact](#-contact)

</div>

---

## ✨ What's New in v1.6

SNATCH v1.6 represents a **complete rewrite** with a stunning modern interface powered by **Dear ImGui**! This version transforms the application from traditional Win32 dialogs to a fluid, professional, GPU-accelerated UI.

### 🎨 Major Improvements

- **🖼️ Complete UI Overhaul**: Rebuilt from scratch using Dear ImGui framework
- **🌙 Premium Dark Theme**: Professional dark color scheme with blue accents
- **⚡ GPU-Accelerated**: DirectX 11 rendering for smooth 60 FPS interface
- **🎯 Custom Font**: Integrated Segoe UI for professional typography
- **💾 Enhanced History**: SQLite-powered download history with search
- **📊 Real-time Statistics**: Track total downloads and data usage
- **🎨 Modern Visual Effects**: Rounded corners, smooth animations, gradient buttons
- **🔔 Smart Notifications**: System notifications on completion
- **⚙️ Advanced Settings**: Comprehensive configuration options
- **📝 Queue Management**: Visual queue with progress tracking

---

## 🚀 Features

### Core Functionality
- ✅ Download videos in multiple qualities (4K, 2K, 1080p, 720p, 480p, 360p, 240p)
- ✅ Extract audio from videos
- ✅ Batch download with queue system (up to 50 items)
- ✅ Real-time download progress tracking
- ✅ Automatic quality selection
- ✅ FFmpeg integration for format conversion

### Modern UI (Dear ImGui)
- 🎨 Beautiful dark theme with professional styling
- ⚡ Smooth 60 FPS GPU-accelerated interface
- 🖱️ Intuitive drag-and-drop support
- 📱 Responsive window layout
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
1. Download `snatch_v1.6.exe` from the [Releases](https://github.com/oktramdani/snatch/releases) page
2. Download `yt-dlp.exe` from [yt-dlp releases](https://github.com/yt-dlp/yt-dlp/releases)
3. Download `ffmpeg.exe` from [FFmpeg builds](https://www.gyan.dev/ffmpeg/builds/)
4. Place all three files in the same folder
5. Run `snatch_v1.6.exe`

### Option 2: Build from Source
```bash
# Clone the repository
git clone https://github.com/oktramdani/snatch.git
cd snatch/imgui

# Compile SQLite (one-time)
gcc -c sqlite3.c -o sqlite3.o -O2

# Build SNATCH
g++ -o snatch_v1.6.exe snatch_v1.6_dark.cpp imgui.cpp imgui_draw.cpp ^
    imgui_tables.cpp imgui_widgets.cpp backends/imgui_impl_win32.cpp ^
    backends/imgui_impl_dx11.cpp sqlite3.o -I. -I./backends ^
    -ld3d11 -ldxgi -ld3dcompiler -lcomctl32 -lshell32 -lgdi32 ^
    -lcomdlg32 -lole32 -lwinmm -ldwmapi -lstdc++ -std=c++11 -O2 -mwindows
```

### Required Dependencies
- **yt-dlp.exe**: Must be in the same folder as SNATCH
- **ffmpeg.exe**: Must be in the same folder as SNATCH
- Both are detected automatically on startup

---

## 🎯 Usage

### Quick Start
1. Launch **SNATCH v1.6**
2. Read the welcome message (first launch)
3. Paste a YouTube URL in the input field
4. Select desired quality and type (Video/Audio)
5. Click **"Add to Queue"**
6. Click **"Start Download"** to begin
7. Wait for completion notification

### Interface Overview

#### Main Window
- **URL Input**: Paste YouTube video URL here
- **Quality Selector**: Choose from Best/4K/2K/1080p/720p/480p/360p/240p
- **Type Selector**: Video or Audio extraction
- **Queue List**: View all pending downloads with progress bars
- **Action Buttons**: Settings, History, Statistics, About

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

### SNATCH v1.6 vs v1.5

| Feature | v1.5 (Win32) | v1.6 (ImGui) |
|---------|--------------|---------------|
| **UI Framework** | Win32 GDI+ | Dear ImGui + DirectX 11 |
| **Rendering** | CPU | GPU-accelerated |
| **Frame Rate** | ~30 FPS | 60 FPS |
| **Theme** | Light/Basic | Premium Dark |
| **Font** | System | Segoe UI Custom |
| **Animations** | Limited | Smooth transitions |
| **Scalability** | Fixed | Responsive |
| **Modern Design** | Basic | Professional |
| **Performance** | Good | Excellent |

### Key Advantages of v1.6
- ✨ **3x Smoother**: GPU rendering eliminates UI lag
- 🎨 **Professional Look**: Modern dark theme with custom styling
- 🚀 **Better Performance**: DirectX 11 offloads CPU work to GPU
- 📱 **Responsive**: Adapts to window resizing smoothly
- 🎯 **Better UX**: Improved button placement and visual hierarchy

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

- **Current Version**: 1.6.0
- **Status**: ✅ Active Development
- **Last Updated**: January 8, 2026
- **Stability**: Stable Release
- **Platform**: Windows (7/8/10/11)

---

## 🎯 Roadmap

### Planned Features
- [ ] Multi-language support (Indonesian, English, etc.)
- [ ] Playlist download support
- [ ] Custom theme editor
- [ ] Download scheduler
- [ ] Bandwidth limiter
- [ ] Browser extension integration
- [ ] Cloud storage integration
- [ ] Mobile companion app

### Future Versions
- **v1.7**: Multi-language support
- **v1.8**: Playlist & channel downloads
- **v2.0**: Cross-platform (Linux, macOS)

---

<div align="center">

### ⭐ If you like SNATCH, please give it a star!

**Made with ❤️ by [Abdanio Parchezzi](https://instagram.com/oktramdani)**

*Powered by Dear ImGui • Built with C++ • Designed for Windows*

[⬆ Back to Top](#-snatch-v16---modern-youtube-downloader)

</div>
