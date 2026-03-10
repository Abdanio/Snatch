# SNATCH v1.7 Changelog

## Release Date: March 10, 2026

## 🎉 Major Features

### 🌐 Multi-Platform Support
- **1000+ Websites Supported**: Download videos from YouTube, Instagram, TikTok, Facebook, Twitter, Vimeo, Reddit, Twitch, Soundcloud, and 1000+ more platforms
- **Automatic Platform Detection**: Smart URL analysis detects the platform automatically
- **Color-Coded Platform Badges**: Each platform gets a unique color for easy identification
  - YouTube: Red
  - Instagram: Pink/Purple
  - TikTok: Cyan
  - Twitter/X: Blue
  - Facebook: Default Blue
  - And more!

### ✨ Smooth UI Animations
- **Logo Pulse Animation**: Subtle breathing effect on the app logo (5px amplitude)
- **Button Glow Effects**: Pulsing glow on interactive buttons and borders
- **Animated Progress Bars**: Progress indicators with smooth pulse effects
- **Fade-In Animations**: Queue items smoothly fade in when added (3x speed)
- **Window Fade-In**: Main window fades in on startup (2x speed)
- **Animated Platform Badges**: Badges change colors smoothly during operations

### 🎨 Visual Enhancements
- **Glowing Title**: App title with dynamic glow intensity
- **Pulsing Download Button**: "Downloading..." button pulses with 6x frequency
- **Animated Input Border**: URL input field border glows subtly
- **Dynamic Platform Display**: Shows 5 supported platforms with animated colors
- **Enhanced Button Padding**: Buttons expand slightly on pulse (5% growth)

## 🔧 Technical Improvements

### New Code Features
- **Animation System**: Complete animation framework with delta time support
- **Easing Functions**: `EaseInOutCubic` and `EaseOutBounce` for smooth transitions
- **Platform Detection Engine**: Comprehensive URL pattern matching for 18+ platforms
- **Performance Timer**: High-precision timing using `QueryPerformanceCounter`
- **Animation State Management**: Per-item and global animation states

### New Structs & Variables
- Added `platform[32]` field to `DownloadItem` struct
- Added `animAlpha` field to `DownloadItem` for fade-in effects
- Animation state variables:
  - `g_buttonPulseTime`: Global pulse timer
  - `g_logoRotation`: Logo rotation state
  - `g_windowFadeAlpha`: Main window fade-in alpha
  - `g_queueItemAnimTime[MAX_QUEUE_SIZE]`: Per-item animation timers
  - `g_animationsEnabled`: Animation toggle flag

### New Functions
- `UpdateAnimations(float deltaTime)`: Main animation update loop
- `EaseInOutCubic(float t)`: Cubic easing function
- `EaseOutBounce(float t)`: Bounce easing function
- `DetectPlatform(const char* url)`: Platform detection from URL

## 📝 Code Changes

### Modified Files
1. **snatch_v1.6_dark.cpp** (1,313 lines)
   - Updated from v1.6.0 to v1.7.0
   - Added 239 new lines of code
   - Modified 49 existing lines
   - Added animation system
   - Enhanced UI rendering
   - Integrated platform detection

2. **README.md**
   - Updated to reflect v1.7 features
   - Added "Supported Platforms" section
   - Updated installation instructions
   - Enhanced feature descriptions
   - Updated roadmap and version comparison

## 🎯 Supported Platforms

### Video Platforms
- YouTube, Vimeo, Dailymotion, Twitch

### Social Media
- Instagram, TikTok, Facebook, Twitter/X, Reddit, LinkedIn, Pinterest, Tumblr

### Audio Platforms
- SoundCloud, Bandcamp, Mixcloud

### Regional Platforms
- Bilibili, VK, 9GAG

### And 1000+ More!
Complete list: [yt-dlp Supported Sites](https://github.com/yt-dlp/yt-dlp/blob/master/supportedsites.md)

## 📊 Statistics

- **Lines of Code Added**: ~239
- **Lines of Code Modified**: ~49
- **New Functions**: 4
- **Animation Effects**: 8+
- **Supported Platforms**: 1000+
- **Platform Detection Patterns**: 18

## 🔄 Migration Guide

### From v1.6 to v1.7

No breaking changes! Simply replace your `snatch_v1.6.exe` with `snatch_v1.7.exe`. All settings and history will be preserved.

**What's Changed:**
- YouTube-only → Multi-platform support
- Static UI → Animated UI
- No platform detection → Automatic detection
- Basic progress bars → Animated progress bars

**What's the Same:**
- Same settings file format
- Same database schema
- Same download quality options
- Same keyboard shortcuts

## 🐛 Known Issues

None reported yet!

## 💡 Tips & Tricks

1. **Platform Detection**: Just paste any video URL - the app automatically detects the platform
2. **Smooth Performance**: Animations are GPU-accelerated and won't slow down downloads
3. **Color Coding**: Use platform badge colors to quickly identify video sources in your queue
4. **Fade Effects**: Watch the smooth fade-in when adding new items to queue

## 🙏 Credits

- **yt-dlp Team**: For supporting 1000+ websites
- **Dear ImGui**: For the amazing UI framework
- **Community**: For feature requests and feedback

## 📞 Support

- GitHub Issues: https://github.com/oktramdani/snatch/issues
- Email: cheeksparchezzi@gmail.com
- Instagram: @oktramdani

---

**SNATCH v1.7 - Download from Anywhere with Style! ✨**
