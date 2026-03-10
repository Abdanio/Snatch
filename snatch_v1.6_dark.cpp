// SNATCH v1.7 - Modern Multi-Platform Downloader with Dear ImGui - DARK THEME
// Enhanced with smooth animations and multi-platform support

#define UNICODE
#define _UNICODE

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <gdiplus.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <time.h>
#include <ctype.h>
#include <cmath>
#include "sqlite3.h"

#pragma comment(lib, "gdiplus.lib")

// Link necessary libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "comctl32.lib")

// Version Information
#define APP_VERSION "1.7.0"
#define APP_VERSION_INT 170

// Constants
#define MAX_QUEUE_SIZE 50
#define MAX_URL_LENGTH 512
#define MAX_PATH_LENGTH 260

// Forward declarations
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI DownloadThread(LPVOID lpParam);

// Global variables for DirectX
static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

// Logo texture for ImGui
static ID3D11ShaderResourceView* g_logoTexture = nullptr;
static int g_logoWidth = 0;
static int g_logoHeight = 0;

// Settings structure
typedef struct {
    int defaultQuality;           // 0-7 (Best, 4K, 2K, 1080p, 720p, 480p, 360p, 240p)
    char downloadPath[MAX_PATH_LENGTH];
    bool autoClearQueue;
    bool enableNotifications;
    int maxConcurrentDownloads;   // 1-5
    bool autoCheckUpdates;
    int totalDownloads;
    long long totalBytes;
} AppSettings;

// History entry
typedef struct {
    int id;
    char url[MAX_URL_LENGTH];
    char title[256];
    char type[32];
    char quality[32];
    char timestamp[64];
    long long size;
} HistoryEntry;

// Download item
typedef struct {
    char url[MAX_URL_LENGTH];
    int quality;
    int type; // 0=Video, 1=Audio
    bool isDownloading;
    float progress;
    char status[128];
    char platform[32];  // Platform name: YouTube, Instagram, TikTok, etc.
    float animAlpha;    // Animation alpha for fade-in effect
} DownloadItem;

// Global state
static AppSettings g_settings = {0};
static std::vector<DownloadItem> g_queue;
static sqlite3* g_db = nullptr;
static bool g_isDownloading = false;

// UI State
static bool g_showSettingsWindow = false;
static bool g_showHistoryWindow = false;
static bool g_showAboutWindow = false;
static bool g_showStatsWindow = false;
static bool g_showWelcomeWindow = true; // Show on first launch

// Animation state
static float g_buttonPulseTime = 0.0f;
static float g_logoRotation = 0.0f;
static float g_windowFadeAlpha = 0.0f;
static float g_queueItemAnimTime[MAX_QUEUE_SIZE] = {0};
static bool g_animationsEnabled = true;

// Input buffers
static char g_urlInput[MAX_URL_LENGTH] = "";
static char g_historySearch[256] = "";

// Constants
const char* SETTINGS_FILE = "snatch_settings.ini";
const char* DB_FILE = "snatch_history.db";
const char* g_qualityOptions[] = { "Best", "4K (2160p)", "2K (1440p)", "1080p", "720p", "480p", "360p", "240p" };
const char* g_typeOptions[] = { "Video", "Audio" };

// Supported platforms - yt-dlp supports 1000+ sites!
const char* g_supportedPlatforms[] = {
    "YouTube", "Instagram", "Facebook", "Twitter/X", "TikTok",
    "Vimeo", "Dailymotion", "Reddit", "Twitch", "Soundcloud",
    "Bilibili", "Pinterest", "LinkedIn", "Tumblr", "And 1000+ more!"
};
const int g_numPlatforms = sizeof(g_supportedPlatforms) / sizeof(g_supportedPlatforms[0]);

// Function declarations
void LoadSettings();
void SaveSettings();
void InitDefaultSettings();
bool InitDatabase();
void CloseDatabase();
bool AddToHistory(const char* url, const char* title, const char* type, const char* quality, long long size);
std::vector<HistoryEntry> GetHistory(const char* searchTerm = nullptr);
bool ClearHistory();
void AddToQueue();
void RemoveFromQueue(int index);
void ClearQueue();
void StartDownloads();
void CheckDependencies();
void EnsureDownloadsFolder();
void OpenDownloadsFolder();
char* GetCurrentTimestamp();
void PlayNotificationSound();
void RenderMainUI();
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
void LoadCustomFont();
void ApplyModernStyle();
bool LoadLogoTexture();
void CleanupLogoTexture();

// Animation helper functions
float EaseInOutCubic(float t);
float EaseOutBounce(float t);
void UpdateAnimations(float deltaTime);
const char* DetectPlatform(const char* url);

// Window procedure
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

// Main code
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Load application icon
    HICON hIcon = (HICON)LoadImageA(GetModuleHandle(NULL), MAKEINTRESOURCEA(101), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
    
    // Create application window
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), hIcon, nullptr, nullptr, nullptr, L"SNATCH v1.7", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"SNATCH v1.7 - Multi-Platform Downloader", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 720, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load custom font and apply style
    LoadCustomFont();
    ApplyModernStyle();
    
    // Load logo texture
    LoadLogoTexture();

    // Initialize app
    LoadSettings();
    InitDatabase();
    CheckDependencies();
    EnsureDownloadsFolder();

    // Main loop
    bool done = false;
    LARGE_INTEGER frequency, lastTime, currentTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastTime);

    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Calculate delta time for animations
        QueryPerformanceCounter(&currentTime);
        float deltaTime = (float)(currentTime.QuadPart - lastTime.QuadPart) / (float)frequency.QuadPart;
        lastTime = currentTime;

        // Update animations
        UpdateAnimations(deltaTime);

        // Start ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Render our UI
        RenderMainUI();

        // Rendering
        ImGui::Render();
        const float clear_color[4] = { 0.10f, 0.10f, 0.13f, 1.00f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    // Cleanup
    CloseDatabase();
    SaveSettings();
    CleanupLogoTexture();
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions to create DirectX device
bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Load professional font (Segoe UI)
void LoadCustomFont()
{
    ImGuiIO& io = ImGui::GetIO();
    
    // Try to load Segoe UI from Windows
    const char* windowsFonts[] = {
        "C:\\Windows\\Fonts\\segoeui.ttf",
        "C:\\Windows\\Fonts\\arial.ttf"
    };
    
    bool fontLoaded = false;
    for (const char* fontPath : windowsFonts) {
        if (GetFileAttributesA(fontPath) != INVALID_FILE_ATTRIBUTES) {
            io.Fonts->AddFontFromFileTTF(fontPath, 18.0f);
            fontLoaded = true;
            break;
        }
    }
    
    // Fallback to default if loading failed
    if (!fontLoaded) {
        io.Fonts->AddFontDefault();
    }
}

// Apply premium dark professional style
void ApplyModernStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Premium Dark Theme - Professional & Modern
    colors[ImGuiCol_Text]                   = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.13f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.12f, 0.12f, 0.15f, 0.98f);
    colors[ImGuiCol_Border]                 = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.24f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.22f, 0.22f, 0.27f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.08f, 0.08f, 0.10f, 0.50f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.30f, 0.30f, 0.35f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.45f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.36f, 0.69f, 1.00f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.20f, 0.25f, 0.35f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.20f, 0.25f, 0.35f, 0.55f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.20f, 0.25f, 0.35f, 0.86f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.20f, 0.25f, 0.35f, 0.25f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.26f, 0.59f, 0.98f, 0.53f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.36f, 0.69f, 1.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.13f, 0.77f, 0.37f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.13f, 0.77f, 0.37f, 0.80f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.24f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.26f, 0.59f, 0.98f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.10f, 0.10f, 0.13f, 0.80f);

    // Style tweaks
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.ScrollbarRounding = 6.0f;
    style.TabRounding = 6.0f;
    style.WindowPadding = ImVec2(15, 15);
    style.FramePadding = ImVec2(8, 6);
    style.ItemSpacing = ImVec2(12, 8);
}

// Load logo texture from PNG file
bool LoadLogoTexture()
{
    // Initialize GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    
    // Load PNG file
    Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(L"..\\Logo.png");
    if (!bitmap || bitmap->GetLastStatus() != Gdiplus::Ok) {
        if (bitmap) delete bitmap;
        Gdiplus::GdiplusShutdown(gdiplusToken);
        return false;
    }
    
    // Get image dimensions
    g_logoWidth = bitmap->GetWidth();
    g_logoHeight = bitmap->GetHeight();
    
    // Create RGBA buffer
    Gdiplus::BitmapData bitmapData;
    Gdiplus::Rect rect(0, 0, g_logoWidth, g_logoHeight);
    bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);
    
    // Create D3D11 texture
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = g_logoWidth;
    desc.Height = g_logoHeight;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    
    D3D11_SUBRESOURCE_DATA subResource = {};
    subResource.pSysMem = bitmapData.Scan0;
    subResource.SysMemPitch = bitmapData.Stride;
    
    ID3D11Texture2D* pTexture = nullptr;
    HRESULT hr = g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);
    
    bitmap->UnlockBits(&bitmapData);
    delete bitmap;
    Gdiplus::GdiplusShutdown(gdiplusToken);
    
    if (FAILED(hr)) return false;
    
    // Create shader resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    
    hr = g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &g_logoTexture);
    pTexture->Release();
    
    return SUCCEEDED(hr);
}

void CleanupLogoTexture()
{
    if (g_logoTexture) {
        g_logoTexture->Release();
        g_logoTexture = nullptr;
    }
}

// Main UI rendering
void RenderMainUI()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

    // Apply fade-in animation to main window
    if (g_windowFadeAlpha < 1.0f) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, g_windowFadeAlpha);
    }

    ImGui::Begin("Main Window", nullptr, window_flags);
    {
        // Logo and Header with pulse animation
        if (g_logoTexture) {
            // Center logo with subtle pulse effect
            float logoDisplayWidth = 120.0f + sinf(g_buttonPulseTime * 2.0f) * 5.0f;
            float logoDisplayHeight = (float)g_logoHeight * (logoDisplayWidth / (float)g_logoWidth);
            float posX = (ImGui::GetContentRegionAvail().x - logoDisplayWidth) * 0.5f;
            ImGui::SetCursorPosX(posX);
            ImGui::Image((void*)g_logoTexture, ImVec2(logoDisplayWidth, logoDisplayHeight));
            ImGui::Spacing();
        }

        // Header with glow effect
        float glowIntensity = 0.5f + 0.5f * sinf(g_buttonPulseTime);
        ImVec4 titleColor = ImVec4(0.26f, 0.59f, 0.98f, 0.8f + 0.2f * glowIntensity);
        ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
        ImGui::SetWindowFontScale(1.8f);
        ImGui::Text("SNATCH v%s", APP_VERSION);
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopStyleColor();
        ImGui::Text("Multi-Platform Downloader - Supports 1000+ Websites!");

        // Platform badges with animated colors
        ImGui::Spacing();
        ImGui::Text("Supported:");
        ImGui::SameLine();
        for (int i = 0; i < 5 && i < g_numPlatforms; i++) {
            float hue = fmodf((g_buttonPulseTime * 0.1f + i * 0.15f), 1.0f);
            ImVec4 badgeColor = ImVec4(0.2f + hue * 0.2f, 0.25f + hue * 0.15f, 0.35f + hue * 0.1f, 1.0f);
            ImGui::PushStyleColor(ImGuiCol_Button, badgeColor);
            ImGui::SmallButton(g_supportedPlatforms[i]);
            ImGui::PopStyleColor();
            if (i < 4) ImGui::SameLine();
        }

        ImGui::Separator();
        ImGui::Spacing();

        // URL Input with animated border
        ImGui::Text("Video URL (YouTube, Instagram, TikTok, etc):");
        float borderGlow = 0.2f + 0.1f * sinf(g_buttonPulseTime * 3.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.26f, 0.59f, 0.98f, borderGlow));
        ImGui::SetNextItemWidth(-200);
        ImGui::InputText("##url", g_urlInput, MAX_URL_LENGTH);
        ImGui::PopStyleColor();
        ImGui::SameLine();

        // Animated Add to Queue button
        float buttonPulse = 1.0f + 0.05f * sinf(g_buttonPulseTime * 4.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8 * buttonPulse, 6 * buttonPulse));
        if (ImGui::Button("Add to Queue", ImVec2(180, 0))) {
            AddToQueue();
        }
        ImGui::PopStyleVar();

        ImGui::Spacing();

        // Quality and Type selection
        ImGui::Text("Quality:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::Combo("##quality", &g_settings.defaultQuality, g_qualityOptions, 8);
        ImGui::SameLine();
        ImGui::Text("Type:");
        ImGui::SameLine();
        static int tempType = 0;
        ImGui::RadioButton("Video", &tempType, 0); ImGui::SameLine();
        ImGui::RadioButton("Audio", &tempType, 1);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Queue Section with animations
        ImGui::Text("Download Queue (%d/%d):", (int)g_queue.size(), MAX_QUEUE_SIZE);
        ImGui::BeginChild("QueueList", ImVec2(0, 250), true);
        {
            for (size_t i = 0; i < g_queue.size(); i++) {
                auto& item = g_queue[i];
                ImGui::PushID((int)i);

                // Apply fade-in animation to queue items
                if (item.animAlpha < 1.0f) {
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, item.animAlpha);
                }

                // Platform badge with color
                ImVec4 platformColor = ImVec4(0.13f, 0.77f, 0.37f, 1.0f);
                if (strstr(item.platform, "YouTube")) platformColor = ImVec4(0.9f, 0.2f, 0.2f, 1.0f);
                else if (strstr(item.platform, "Instagram")) platformColor = ImVec4(0.8f, 0.3f, 0.5f, 1.0f);
                else if (strstr(item.platform, "TikTok")) platformColor = ImVec4(0.0f, 0.9f, 0.8f, 1.0f);
                else if (strstr(item.platform, "Twitter")) platformColor = ImVec4(0.3f, 0.6f, 1.0f, 1.0f);

                ImGui::PushStyleColor(ImGuiCol_Button, platformColor);
                ImGui::SmallButton(item.platform);
                ImGui::PopStyleColor();
                ImGui::SameLine();

                ImGui::Text("%d. %s", (int)i+1, item.url);
                ImGui::SameLine();
                ImGui::Text("[%s - %s]", g_typeOptions[item.type], g_qualityOptions[item.quality]);

                if (item.isDownloading) {
                    // Animated progress bar with pulse effect
                    float progressPulse = 0.95f + 0.05f * sinf(g_queueItemAnimTime[i] * 5.0f);
                    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.26f * progressPulse, 0.59f * progressPulse, 0.98f * progressPulse, 1.0f));
                    ImGui::ProgressBar(item.progress / 100.0f, ImVec2(-100, 0));
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    ImGui::Text("%s", item.status);
                } else {
                    ImGui::SameLine();
                    if (ImGui::Button("Remove", ImVec2(80, 0))) {
                        RemoveFromQueue(i);
                    }
                }

                if (item.animAlpha < 1.0f) {
                    ImGui::PopStyleVar();
                }

                ImGui::Separator();
                ImGui::PopID();
            }
        }
        ImGui::EndChild();

        ImGui::Spacing();

        // Queue control buttons with animation
        if (g_isDownloading) {
            float downloadPulse = 0.8f + 0.2f * sinf(g_buttonPulseTime * 6.0f);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f * downloadPulse, 0.4f * downloadPulse, 0.2f * downloadPulse, 1.0f));
            ImGui::Button("Downloading...", ImVec2(200, 40));
            ImGui::PopStyleColor();
        } else {
            if (ImGui::Button("Start Download", ImVec2(200, 40))) {
                StartDownloads();
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Clear Queue", ImVec2(200, 40))) {
            ClearQueue();
        }

        ImGui::Spacing();
        ImGui::Spacing();

        // Action buttons
        if (ImGui::Button("Settings", ImVec2(120, 40))) {
            g_showSettingsWindow = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("History", ImVec2(120, 40))) {
            g_showHistoryWindow = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Statistics", ImVec2(120, 40))) {
            g_showStatsWindow = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("About", ImVec2(120, 40))) {
            g_showAboutWindow = true;
        }

        ImGui::Spacing();

        // Additional buttons with callbacks
        if (ImGui::Button("Open Downloads Folder", ImVec2(200, 40))) {
            OpenDownloadsFolder();
        }
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.9f, 0.6f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.7f, 0.3f, 1.0f));
        if (ImGui::Button("☕ Buy Me a Coffee", ImVec2(200, 40))) {
            ShellExecuteA(NULL, "open", "https://buymeacoffee.com/abdanio", NULL, NULL, SW_SHOWNORMAL);
        }
        ImGui::PopStyleColor(2);
        ImGui::SameLine();
        if (ImGui::Button("Show Welcome", ImVec2(200, 40))) {
            g_showWelcomeWindow = true;
        }
    }
    ImGui::End();

    if (g_windowFadeAlpha < 1.0f) {
        ImGui::PopStyleVar();
    }

    // Settings Window
    if (g_showSettingsWindow) {
        ImGui::SetNextWindowSize(ImVec2(500, 450), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Settings", &g_showSettingsWindow)) {
            ImGui::Text("Default Quality:");
            ImGui::Combo("##defaultQuality", &g_settings.defaultQuality, g_qualityOptions, 8);
            
            ImGui::Spacing();
            ImGui::Text("Download Path:");
            ImGui::InputText("##downloadPath", g_settings.downloadPath, MAX_PATH_LENGTH);
            
            ImGui::Spacing();
            ImGui::Checkbox("Auto Clear Queue", &g_settings.autoClearQueue);
            ImGui::Checkbox("Enable Notifications", &g_settings.enableNotifications);
            ImGui::Checkbox("Auto Check Updates", &g_settings.autoCheckUpdates);
            
            ImGui::Spacing();
            ImGui::Text("Max Concurrent Downloads:");
            ImGui::SliderInt("##maxConcurrent", &g_settings.maxConcurrentDownloads, 1, 5);
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            
            if (ImGui::Button("Save Settings", ImVec2(120, 0))) {
                SaveSettings();
                g_showSettingsWindow = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                LoadSettings(); // Reload settings
                g_showSettingsWindow = false;
            }
        }
        ImGui::End();
    }

    // History Window
    if (g_showHistoryWindow) {
        ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Download History", &g_showHistoryWindow)) {
            ImGui::Text("Search:");
            ImGui::SameLine();
            ImGui::InputText("##historySearch", g_historySearch, 256);
            ImGui::SameLine();
            if (ImGui::Button("Clear History")) {
                if (ClearHistory()) {
                    ImGui::OpenPopup("History Cleared");
                }
            }
            
            ImGui::Separator();
            
            std::vector<HistoryEntry> history = GetHistory(g_historySearch[0] ? g_historySearch : nullptr);
            
            ImGui::BeginChild("HistoryList", ImVec2(0, 0), true);
            for (const auto& entry : history) {
                ImGui::Text("[%s] %s - %s", entry.type, entry.quality, entry.timestamp);
                ImGui::TextWrapped("Title: %s", entry.title);
                ImGui::TextWrapped("URL: %s", entry.url);
                ImGui::Separator();
            }
            ImGui::EndChild();
        }
        ImGui::End();
    }

    // Stats Window
    if (g_showStatsWindow) {
        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Statistics", &g_showStatsWindow)) {
            ImGui::Text("Total Downloads: %d", g_settings.totalDownloads);
            ImGui::Text("Total Data: %.2f MB", g_settings.totalBytes / (1024.0 * 1024.0));
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("Current Queue: %d items", (int)g_queue.size());
        }
        ImGui::End();
    }

    // About Window
    if (g_showAboutWindow) {
        ImGui::SetNextWindowSize(ImVec2(500, 450), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("About SNATCH", &g_showAboutWindow)) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
            ImGui::Text("SNATCH v%s", APP_VERSION);
            ImGui::PopStyleColor();
            ImGui::Text("Modern Multi-Platform Downloader");
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("Features:");
            ImGui::BulletText("Download videos from 1000+ websites");
            ImGui::BulletText("Support for YouTube, Instagram, TikTok, Facebook, Twitter, and more");
            ImGui::BulletText("Multiple quality options (up to 4K)");
            ImGui::BulletText("Extract audio from videos");
            ImGui::BulletText("Queue management with animations");
            ImGui::BulletText("Download history with SQLite");
            ImGui::BulletText("Premium dark theme UI with smooth animations");
            ImGui::BulletText("Professional Segoe UI font");
            ImGui::BulletText("GPU-accelerated rendering");
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("Powered by:");
            ImGui::BulletText("yt-dlp (supports 1000+ sites)");
            ImGui::BulletText("FFmpeg");
            ImGui::BulletText("Dear ImGui");
            ImGui::BulletText("SQLite");
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("Supported Platforms:");
            for (int i = 0; i < g_numPlatforms; i++) {
                ImGui::BulletText("%s", g_supportedPlatforms[i]);
            }
        }
        ImGui::End();
    }

    // Welcome Window
    if (g_showWelcomeWindow) {
        ImGui::SetNextWindowSize(ImVec2(550, 500), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(viewport->Size.x / 2, viewport->Size.y / 2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        if (ImGui::Begin("Welcome to SNATCH v1.7", &g_showWelcomeWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
            ImGui::SetWindowFontScale(1.5f);
            ImGui::Text("Welcome to SNATCH v1.7!");
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopStyleColor();
            ImGui::Spacing();
            ImGui::TextWrapped("Thank you for using SNATCH, the modern multi-platform downloader with professional dark UI and smooth animations.");
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.13f, 0.77f, 0.37f, 1.0f));
            ImGui::Text("What's New in v1.7:");
            ImGui::PopStyleColor();
            ImGui::BulletText("Support for 1000+ websites (not just YouTube!)");
            ImGui::BulletText("Smooth animations on buttons, progress bars, and UI elements");
            ImGui::BulletText("Animated platform badges with color coding");
            ImGui::BulletText("Pulsing effects and fade-in animations");
            ImGui::BulletText("Enhanced visual feedback throughout the UI");
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("Quick Start:");
            ImGui::BulletText("Paste a video URL (YouTube, Instagram, TikTok, etc.)");
            ImGui::BulletText("Select quality and type (Video/Audio)");
            ImGui::BulletText("Click 'Add to Queue'");
            ImGui::BulletText("Click 'Start Download' to begin");
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            if (ImGui::Button("Get Started!", ImVec2(-1, 40))) {
                g_showWelcomeWindow = false;
            }
        }
        ImGui::End();
    }
}

// Settings functions
void LoadSettings()
{
    FILE* f = fopen(SETTINGS_FILE, "r");
    if (f) {
        fscanf(f, "defaultQuality=%d\n", &g_settings.defaultQuality);
        fscanf(f, "downloadPath=%[^\n]\n", g_settings.downloadPath);
        fscanf(f, "autoClearQueue=%d\n", (int*)&g_settings.autoClearQueue);
        fscanf(f, "enableNotifications=%d\n", (int*)&g_settings.enableNotifications);
        fscanf(f, "maxConcurrentDownloads=%d\n", &g_settings.maxConcurrentDownloads);
        fscanf(f, "autoCheckUpdates=%d\n", (int*)&g_settings.autoCheckUpdates);
        fscanf(f, "totalDownloads=%d\n", &g_settings.totalDownloads);
        fscanf(f, "totalBytes=%lld\n", &g_settings.totalBytes);
        fclose(f);
    } else {
        InitDefaultSettings();
    }
}

void SaveSettings()
{
    FILE* f = fopen(SETTINGS_FILE, "w");
    if (f) {
        fprintf(f, "defaultQuality=%d\n", g_settings.defaultQuality);
        fprintf(f, "downloadPath=%s\n", g_settings.downloadPath);
        fprintf(f, "autoClearQueue=%d\n", g_settings.autoClearQueue);
        fprintf(f, "enableNotifications=%d\n", g_settings.enableNotifications);
        fprintf(f, "maxConcurrentDownloads=%d\n", g_settings.maxConcurrentDownloads);
        fprintf(f, "autoCheckUpdates=%d\n", g_settings.autoCheckUpdates);
        fprintf(f, "totalDownloads=%d\n", g_settings.totalDownloads);
        fprintf(f, "totalBytes=%lld\n", g_settings.totalBytes);
        fclose(f);
    }
}

void InitDefaultSettings()
{
    g_settings.defaultQuality = 3; // 1080p
    strcpy(g_settings.downloadPath, "downloads");
    g_settings.autoClearQueue = false;
    g_settings.enableNotifications = true;
    g_settings.maxConcurrentDownloads = 3;
    g_settings.autoCheckUpdates = true;
    g_settings.totalDownloads = 0;
    g_settings.totalBytes = 0;
}

// Database functions
bool InitDatabase()
{
    int rc = sqlite3_open(DB_FILE, &g_db);
    if (rc != SQLITE_OK) {
        return false;
    }
    
    const char* sql = "CREATE TABLE IF NOT EXISTS history ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "url TEXT NOT NULL,"
                      "title TEXT,"
                      "type TEXT,"
                      "quality TEXT,"
                      "timestamp TEXT,"
                      "size INTEGER);";
    
    char* errMsg = nullptr;
    rc = sqlite3_exec(g_db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}

void CloseDatabase()
{
    if (g_db) {
        sqlite3_close(g_db);
        g_db = nullptr;
    }
}

bool AddToHistory(const char* url, const char* title, const char* type, const char* quality, long long size)
{
    if (!g_db) return false;
    
    char timestamp[64];
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
    
    const char* sql = "INSERT INTO history (url, title, type, quality, timestamp, size) VALUES (?, ?, ?, ?, ?, ?);";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, url, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, title, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, type, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, quality, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, timestamp, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 6, size);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return rc == SQLITE_DONE;
}

std::vector<HistoryEntry> GetHistory(const char* searchTerm)
{
    std::vector<HistoryEntry> history;
    if (!g_db) return history;
    
    const char* sql = searchTerm ? 
        "SELECT * FROM history WHERE url LIKE ? OR title LIKE ? ORDER BY id DESC LIMIT 100;" :
        "SELECT * FROM history ORDER BY id DESC LIMIT 100;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return history;
    
    if (searchTerm) {
        char pattern[512];
        snprintf(pattern, sizeof(pattern), "%%%s%%", searchTerm);
        sqlite3_bind_text(stmt, 1, pattern, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, pattern, -1, SQLITE_STATIC);
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        HistoryEntry entry;
        entry.id = sqlite3_column_int(stmt, 0);
        strncpy(entry.url, (const char*)sqlite3_column_text(stmt, 1), MAX_URL_LENGTH);
        strncpy(entry.title, (const char*)sqlite3_column_text(stmt, 2), 256);
        strncpy(entry.type, (const char*)sqlite3_column_text(stmt, 3), 32);
        strncpy(entry.quality, (const char*)sqlite3_column_text(stmt, 4), 32);
        strncpy(entry.timestamp, (const char*)sqlite3_column_text(stmt, 5), 64);
        entry.size = sqlite3_column_int64(stmt, 6);
        history.push_back(entry);
    }
    
    sqlite3_finalize(stmt);
    return history;
}

bool ClearHistory()
{
    if (!g_db) return false;
    
    const char* sql = "DELETE FROM history;";
    char* errMsg = nullptr;
    int rc = sqlite3_exec(g_db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// Queue functions
void AddToQueue()
{
    if (strlen(g_urlInput) == 0) return;
    if (g_queue.size() >= MAX_QUEUE_SIZE) return;

    DownloadItem item;
    strncpy(item.url, g_urlInput, MAX_URL_LENGTH);
    item.quality = g_settings.defaultQuality;
    item.type = 0; // Default to video
    item.isDownloading = false;
    item.progress = 0;
    strcpy(item.status, "Pending");

    // Detect platform from URL
    const char* platform = DetectPlatform(g_urlInput);
    strncpy(item.platform, platform, sizeof(item.platform) - 1);
    item.platform[sizeof(item.platform) - 1] = '\0';

    // Initialize animation
    item.animAlpha = 0.0f;

    g_queue.push_back(item);

    // Clear input
    g_urlInput[0] = '\0';
}

void RemoveFromQueue(int index)
{
    if (index >= 0 && index < (int)g_queue.size()) {
        g_queue.erase(g_queue.begin() + index);
    }
}

void ClearQueue()
{
    g_queue.clear();
}

// Thread function for downloading
DWORD WINAPI DownloadThread(LPVOID lpParam)
{
    for (size_t i = 0; i < g_queue.size(); i++) {
        auto& item = g_queue[i];
        item.isDownloading = true;
        strcpy(item.status, "Downloading...");
        
        // Build yt-dlp command
        char cmd[2048];
        const char* format = item.type == 0 ? "bestvideo+bestaudio/best" : "bestaudio";
        
        snprintf(cmd, sizeof(cmd), 
            "..\\yt-dlp.exe --ffmpeg-location ..\\ffmpeg.exe -f %s -o \"%s\\%%(title)s.%%(ext)s\" \"%s\"",
            format,
            g_settings.downloadPath,
            item.url);
        
        // Execute command
        STARTUPINFOA si = {0};
        PROCESS_INFORMATION pi = {0};
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;
        
        if (CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            // Wait for process and update progress
            DWORD exitCode;
            while (GetExitCodeProcess(pi.hProcess, &exitCode) && exitCode == STILL_ACTIVE) {
                item.progress += 1.0f;
                if (item.progress > 95.0f) item.progress = 95.0f;
                Sleep(500);
            }
            
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            
            item.progress = 100.0f;
            strcpy(item.status, "Completed");
            
            // Add to history
            AddToHistory(item.url, "Downloaded Content", g_typeOptions[item.type], 
                        g_qualityOptions[item.quality], 0);
            
            g_settings.totalDownloads++;
            SaveSettings();
        } else {
            strcpy(item.status, "Failed");
        }
        
        item.isDownloading = false;
    }
    
    if (g_settings.autoClearQueue) {
        g_queue.clear();
    }
    
    if (g_settings.enableNotifications) {
        PlayNotificationSound();
    }
    
    g_isDownloading = false;
    return 0;
}

void StartDownloads()
{
    if (g_queue.empty() || g_isDownloading) return;
    
    g_isDownloading = true;
    
    // Start download in separate thread
    HANDLE hThread = CreateThread(NULL, 0, DownloadThread, NULL, 0, NULL);
    if (hThread) {
        CloseHandle(hThread);
    } else {
        g_isDownloading = false;
    }
}

// Utility functions
void CheckDependencies()
{
    // Check for yt-dlp.exe and ffmpeg.exe in parent directory
    if (GetFileAttributesA("..\\yt-dlp.exe") == INVALID_FILE_ATTRIBUTES) {
        MessageBoxA(NULL, "yt-dlp.exe not found! Please place it in the parent directory.", "Warning", MB_ICONWARNING);
    }
    if (GetFileAttributesA("..\\ffmpeg.exe") == INVALID_FILE_ATTRIBUTES) {
        MessageBoxA(NULL, "ffmpeg.exe not found! Please place it in the parent directory.", "Warning", MB_ICONWARNING);
    }
}

void EnsureDownloadsFolder()
{
    CreateDirectoryA(g_settings.downloadPath, NULL);
}

void OpenDownloadsFolder()
{
    ShellExecuteA(NULL, "open", g_settings.downloadPath, NULL, NULL, SW_SHOWDEFAULT);
}

char* GetCurrentTimestamp()
{
    static char buffer[64];
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
    return buffer;
}

void PlayNotificationSound()
{
    PlaySoundA((LPCSTR)SND_ALIAS_SYSTEMASTERISK, NULL, SND_ALIAS_ID | SND_ASYNC);
}

// Animation helper functions
float EaseInOutCubic(float t)
{
    if (t < 0.5f)
        return 4.0f * t * t * t;
    else {
        float f = ((2.0f * t) - 2.0f);
        return 0.5f * f * f * f + 1.0f;
    }
}

float EaseOutBounce(float t)
{
    if (t < (1.0f / 2.75f))
        return 7.5625f * t * t;
    else if (t < (2.0f / 2.75f)) {
        t -= (1.5f / 2.75f);
        return 7.5625f * t * t + 0.75f;
    }
    else if (t < (2.5f / 2.75f)) {
        t -= (2.25f / 2.75f);
        return 7.5625f * t * t + 0.9375f;
    }
    else {
        t -= (2.625f / 2.75f);
        return 7.5625f * t * t + 0.984375f;
    }
}

void UpdateAnimations(float deltaTime)
{
    if (!g_animationsEnabled) return;

    // Update global animation timers
    g_buttonPulseTime += deltaTime;
    g_logoRotation += deltaTime * 0.5f;

    // Fade in main window
    if (g_windowFadeAlpha < 1.0f) {
        g_windowFadeAlpha += deltaTime * 2.0f;
        if (g_windowFadeAlpha > 1.0f) g_windowFadeAlpha = 1.0f;
    }

    // Update queue item animations
    for (size_t i = 0; i < g_queue.size(); i++) {
        auto& item = g_queue[i];

        // Fade in animation for new items
        if (item.animAlpha < 1.0f) {
            item.animAlpha += deltaTime * 3.0f;
            if (item.animAlpha > 1.0f) item.animAlpha = 1.0f;
        }

        // Update individual item animation time
        if (i < MAX_QUEUE_SIZE) {
            g_queueItemAnimTime[i] += deltaTime;
        }
    }
}

const char* DetectPlatform(const char* url)
{
    if (!url || strlen(url) == 0) return "Unknown";

    // Convert to lowercase for comparison
    char lowerUrl[MAX_URL_LENGTH];
    strncpy(lowerUrl, url, MAX_URL_LENGTH - 1);
    lowerUrl[MAX_URL_LENGTH - 1] = '\0';
    for (int i = 0; lowerUrl[i]; i++) {
        lowerUrl[i] = tolower(lowerUrl[i]);
    }

    // Check for platform keywords in URL
    if (strstr(lowerUrl, "youtube.com") || strstr(lowerUrl, "youtu.be"))
        return "YouTube";
    else if (strstr(lowerUrl, "instagram.com"))
        return "Instagram";
    else if (strstr(lowerUrl, "facebook.com") || strstr(lowerUrl, "fb.com") || strstr(lowerUrl, "fb.watch"))
        return "Facebook";
    else if (strstr(lowerUrl, "twitter.com") || strstr(lowerUrl, "x.com"))
        return "Twitter/X";
    else if (strstr(lowerUrl, "tiktok.com"))
        return "TikTok";
    else if (strstr(lowerUrl, "vimeo.com"))
        return "Vimeo";
    else if (strstr(lowerUrl, "dailymotion.com"))
        return "Dailymotion";
    else if (strstr(lowerUrl, "reddit.com"))
        return "Reddit";
    else if (strstr(lowerUrl, "twitch.tv"))
        return "Twitch";
    else if (strstr(lowerUrl, "soundcloud.com"))
        return "Soundcloud";
    else if (strstr(lowerUrl, "bilibili.com"))
        return "Bilibili";
    else if (strstr(lowerUrl, "pinterest.com"))
        return "Pinterest";
    else if (strstr(lowerUrl, "linkedin.com"))
        return "LinkedIn";
    else if (strstr(lowerUrl, "tumblr.com"))
        return "Tumblr";
    else if (strstr(lowerUrl, "streamable.com"))
        return "Streamable";
    else if (strstr(lowerUrl, "vk.com"))
        return "VK";
    else if (strstr(lowerUrl, "imgur.com"))
        return "Imgur";
    else if (strstr(lowerUrl, "9gag.com"))
        return "9GAG";
    else
        return "Other";
}
