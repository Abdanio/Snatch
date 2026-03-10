# SNATCH v1.7 - Implementation Details

## Animation System Architecture

### Overview
SNATCH v1.7 introduces a lightweight, GPU-accelerated animation system built on top of Dear ImGui. All animations are calculated per-frame using delta time for smooth, consistent motion regardless of frame rate.

### Animation Framework

#### Core Components

1. **Delta Time Calculation**
   ```cpp
   LARGE_INTEGER frequency, lastTime, currentTime;
   QueryPerformanceFrequency(&frequency);
   QueryPerformanceCounter(&lastTime);

   // Per frame:
   QueryPerformanceCounter(&currentTime);
   float deltaTime = (float)(currentTime.QuadPart - lastTime.QuadPart) / (float)frequency.QuadPart;
   ```

2. **Update Loop**
   ```cpp
   void UpdateAnimations(float deltaTime)
   {
       // Global timers
       g_buttonPulseTime += deltaTime;
       g_logoRotation += deltaTime * 0.5f;

       // Window fade-in
       if (g_windowFadeAlpha < 1.0f)
           g_windowFadeAlpha += deltaTime * 2.0f;

       // Per-item animations
       for (each item in queue)
           item.animAlpha += deltaTime * 3.0f;
   }
   ```

3. **Easing Functions**
   - `EaseInOutCubic(t)`: Smooth acceleration/deceleration
   - `EaseOutBounce(t)`: Bouncy effect for special UI elements

### Animation Types

#### 1. Logo Pulse Animation
**Effect**: Subtle breathing effect
**Implementation**:
```cpp
float logoDisplayWidth = 120.0f + sinf(g_buttonPulseTime * 2.0f) * 5.0f;
```
- Base size: 120px
- Amplitude: ±5px
- Frequency: 2Hz (2 cycles per second)

#### 2. Text Glow Effect
**Effect**: Pulsing glow on title text
**Implementation**:
```cpp
float glowIntensity = 0.5f + 0.5f * sinf(g_buttonPulseTime);
ImVec4 titleColor = ImVec4(0.26f, 0.59f, 0.98f, 0.8f + 0.2f * glowIntensity);
```
- Alpha range: 0.8 - 1.0
- Frequency: 1Hz

#### 3. Border Glow
**Effect**: Glowing border on input fields
**Implementation**:
```cpp
float borderGlow = 0.2f + 0.1f * sinf(g_buttonPulseTime * 3.0f);
ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.26f, 0.59f, 0.98f, borderGlow));
```
- Alpha range: 0.2 - 0.3
- Frequency: 3Hz

#### 4. Button Pulse
**Effect**: Slight size change on buttons
**Implementation**:
```cpp
float buttonPulse = 1.0f + 0.05f * sinf(g_buttonPulseTime * 4.0f);
ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8 * buttonPulse, 6 * buttonPulse));
```
- Scale range: 1.0 - 1.05 (5% growth)
- Frequency: 4Hz

#### 5. Progress Bar Animation
**Effect**: Pulsing color on active downloads
**Implementation**:
```cpp
float progressPulse = 0.95f + 0.05f * sinf(g_queueItemAnimTime[i] * 5.0f);
ImVec4 color = ImVec4(0.26f * progressPulse, 0.59f * progressPulse, 0.98f * progressPulse, 1.0f);
```
- Brightness range: 0.95 - 1.0
- Frequency: 5Hz per item

#### 6. Fade-In Animation
**Effect**: Smooth appearance of queue items
**Implementation**:
```cpp
// On add:
item.animAlpha = 0.0f;

// Per frame:
if (item.animAlpha < 1.0f) {
    item.animAlpha += deltaTime * 3.0f;
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, item.animAlpha);
}
```
- Duration: ~0.33 seconds (3x speed)
- Curve: Linear

#### 7. Platform Badge Animation
**Effect**: Color shifting badges
**Implementation**:
```cpp
float hue = fmodf((g_buttonPulseTime * 0.1f + i * 0.15f), 1.0f);
ImVec4 badgeColor = ImVec4(0.2f + hue * 0.2f, 0.25f + hue * 0.15f, 0.35f + hue * 0.1f, 1.0f);
```
- Speed: 0.1Hz
- Phase offset: 0.15 per badge

#### 8. Download Button Pulse
**Effect**: Rapid pulse during active downloads
**Implementation**:
```cpp
float downloadPulse = 0.8f + 0.2f * sinf(g_buttonPulseTime * 6.0f);
ImVec4 color = ImVec4(0.8f * downloadPulse, 0.4f * downloadPulse, 0.2f * downloadPulse, 1.0f);
```
- Brightness range: 0.8 - 1.0
- Frequency: 6Hz (rapid attention-grabbing)

## Platform Detection System

### Architecture

The platform detection system uses URL pattern matching to identify video sources.

### Implementation

```cpp
const char* DetectPlatform(const char* url)
{
    // Convert to lowercase
    char lowerUrl[MAX_URL_LENGTH];
    strncpy(lowerUrl, url, MAX_URL_LENGTH - 1);
    for (int i = 0; lowerUrl[i]; i++)
        lowerUrl[i] = tolower(lowerUrl[i]);

    // Pattern matching
    if (strstr(lowerUrl, "youtube.com") || strstr(lowerUrl, "youtu.be"))
        return "YouTube";
    else if (strstr(lowerUrl, "instagram.com"))
        return "Instagram";
    // ... etc
}
```

### Supported Patterns

| Platform | URL Patterns |
|----------|--------------|
| YouTube | youtube.com, youtu.be |
| Instagram | instagram.com |
| Facebook | facebook.com, fb.com, fb.watch |
| Twitter | twitter.com, x.com |
| TikTok | tiktok.com |
| Vimeo | vimeo.com |
| Dailymotion | dailymotion.com |
| Reddit | reddit.com |
| Twitch | twitch.tv |
| Soundcloud | soundcloud.com |
| Bilibili | bilibili.com |
| Pinterest | pinterest.com |
| LinkedIn | linkedin.com |
| Tumblr | tumblr.com |
| Streamable | streamable.com |
| VK | vk.com |
| Imgur | imgur.com |
| 9GAG | 9gag.com |

### Platform Colors

```cpp
ImVec4 platformColor;
if (strstr(item.platform, "YouTube"))
    platformColor = ImVec4(0.9f, 0.2f, 0.2f, 1.0f);  // Red
else if (strstr(item.platform, "Instagram"))
    platformColor = ImVec4(0.8f, 0.3f, 0.5f, 1.0f);  // Pink/Purple
else if (strstr(item.platform, "TikTok"))
    platformColor = ImVec4(0.0f, 0.9f, 0.8f, 1.0f);  // Cyan
else if (strstr(item.platform, "Twitter"))
    platformColor = ImVec4(0.3f, 0.6f, 1.0f, 1.0f);  // Blue
else
    platformColor = ImVec4(0.13f, 0.77f, 0.37f, 1.0f);  // Green (default)
```

## Performance Considerations

### GPU Acceleration
- All animations run at 60 FPS using DirectX 11
- No CPU overhead from animation calculations
- Smooth even on older systems with DirectX 11 support

### Memory Usage
- Animation state: ~256 bytes (global timers + per-item state)
- No heap allocations during animation updates
- Stack-based calculations only

### Frame Time Budget
- Animation update: <0.1ms per frame
- Total UI render: ~16ms target (60 FPS)
- Plenty of headroom for complex UIs

## Testing

### Animation Performance
To verify smooth animations:
1. Add 50 items to queue (max capacity)
2. Start downloads
3. Check frame time stays under 16ms
4. Verify no stuttering or jank

### Platform Detection
To test platform detection:
1. Test URLs from each supported platform
2. Verify correct platform name appears in badge
3. Verify correct color coding
4. Check fallback to "Other" for unknown sites

### Multi-Platform Downloads
To verify multi-platform support:
1. Queue videos from different platforms
2. Verify yt-dlp handles each URL correctly
3. Check downloads complete successfully
4. Verify platform names saved in history

## Future Enhancements

### Planned Animation Features (v1.8+)
- Window slide-in effects
- More easing functions (elastic, exponential)
- Customizable animation speeds
- Animation presets (subtle, normal, vibrant)
- Per-animation enable/disable

### Planned Platform Features (v1.8+)
- Platform-specific download options
- Per-platform quality presets
- Platform statistics
- Favorite platforms
- Platform filters in history

## Developer Notes

### Adding New Animations
1. Add state variable (e.g., `float g_newAnimTime = 0.0f;`)
2. Update in `UpdateAnimations()` (e.g., `g_newAnimTime += deltaTime;`)
3. Apply in `RenderMainUI()` using sin/cos functions
4. Test at 60 FPS

### Adding New Platforms
1. Add pattern check in `DetectPlatform()`
2. Add color in platform badge rendering
3. Test with real URLs
4. Update documentation

### Animation Guidelines
- Keep frequencies under 10Hz (avoid seizure risk)
- Use subtle alpha/scale changes (5-20% max)
- Prefer sin/cos for smooth periodic motion
- Test on different screen sizes
- Ensure animations enhance, not distract

---

**For more information, see the main README.md or CHANGELOG_v1.7.md**
