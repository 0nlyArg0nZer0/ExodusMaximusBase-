#include <Windows.h>
#include <ImGui/imgui.h>
#include <dwmapi.h>
#include <d3d11.h>

#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>
#include "crosshair.h" 

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool running = true;
bool showMenu = true;  // Flag to control ImGui menu visibility
int screenWidth = GetSystemMetrics(SM_CXSCREEN);  // Screen width
int screenHeight = GetSystemMetrics(SM_CYSCREEN);  // Screen height

bool showCrosshair = false;  // Flag to control crosshair visibility
bool showCrosshairCircle = false;

ImVec4 crosshairColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // Default white color for crosshair
float crosshairSize = 30.0f;  // Default size for crosshair

ImVec4 circleColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  // Standard: Red
float circleSize = 40.0f;  // Standard size for the circle

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    if (ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param))
    {
        return 0L;
    }

    //  (exit on pressing F12)
    if (message == WM_KEYDOWN)
    {
        if (w_param == VK_F12)
        {
            running = false;  // Exit on F12
            PostQuitMessage(0);
            return 0L;  // Ensure no further processing of this message
        }

        if (w_param == VK_INSERT)  // Check if Insert key is pressed
        {
            showMenu = !showMenu;  // Toggle menu visibility
        }
    }

    //  (close on WM_DESTROY)
    if (message == WM_DESTROY)
    {
        PostQuitMessage(0);  // Close the window
    }

    return DefWindowProc(window, message, w_param, l_param);  // Default window procedure
}


INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show)
{
    // Window Class Setup
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = window_procedure;
    wc.hInstance = instance;
    wc.lpszClassName = L"Overlay Class";
    RegisterClassExW(&wc);

    HWND window = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED,  // Set overlay style with layering
        wc.lpszClassName,
        L"0nlyArg0ns2 External ImGui ",
        WS_POPUP,
        0, 0, screenWidth, screenHeight,
        nullptr, nullptr, wc.hInstance, nullptr
    );

    SetWindowPos(window, HWND_TOPMOST, 0, 0, screenWidth, screenHeight, SWP_NOACTIVATE);
    SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_COLORKEY); // Set transparency
   
    // DX Setup for DirectX rendering
    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferDesc.RefreshRate.Numerator = 60U;
    sd.BufferDesc.RefreshRate.Denominator = 1U;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.SampleDesc.Count = 1U;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 2U;
    sd.OutputWindow = window;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    constexpr D3D_FEATURE_LEVEL levels[2]{
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0
    };

    ID3D11Device* device{ nullptr };
    ID3D11DeviceContext* device_contex{ nullptr };
    IDXGISwapChain* swap_chain{ nullptr };
    ID3D11RenderTargetView* render_target_view{ nullptr };
    D3D_FEATURE_LEVEL level{};

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0U,
        levels,
        2U,
        D3D11_SDK_VERSION,
        &sd,
        &swap_chain,
        &device,
        &level,
        &device_contex);

    ID3D11Texture2D* back_buffer{ nullptr };
    swap_chain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer));

    if (back_buffer)
    {
        device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
        back_buffer->Release();
    }
    else
        return 1;

    ShowWindow(window, cmd_show);
    UpdateWindow(window);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();  // Default dark theme (can customize further)
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(device, device_contex);

    while (running)
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                running = false;
        }

        if (running)
        {
            SetWindowPos(window, HWND_TOPMOST, 0, 0, screenWidth, screenHeight, SWP_DRAWFRAME);

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();
            ImGui::SetNextWindowSize(ImVec2(600, 400));  // Customize the window size

            if (showMenu)
            {
               ImGui::Begin("NoScrollbar", nullptr, ImGuiWindowFlags_NoScrollbar);



                ImGui::SetNextWindowPos(ImVec2(0, 0));

                // Tab bar customization
                if (ImGui::BeginTabBar("MainTabs"))
                {
                    // Main Page Tab (Ensure unique ID with ##)
                    ImGui::PushID("CheatsTab");
                    if (ImGui::BeginTabItem("Cheats##Cheats"))
                    {
                        ImGui::Text("Thank you for using my External <3");
                        ImGui::EndTabItem();
                    }
                    ImGui::PopID();

                    // Crosshair Settings Tab (Ensure unique ID with ##)
                    ImGui::PushID("CrosshairSettingsTab");
                    if (ImGui::BeginTabItem("Crosshair Settings##Crosshair"))
                    {
                        ImGui::Checkbox("Enable Crosshair", &showCrosshair);  // Crosshair toggle
                        ImGui::Checkbox("Enable Crosshair Circle", &showCrosshairCircle);  // Circle toggle
                        if (showCrosshair)
                        {
                            ImGui::Text("Crosshair Size");
                            ImGui::SliderFloat("Size##CrosshairSize", &crosshairSize, 10.0f, 100.0f);  // Slider to control size
                            ImGui::Text("Crosshair Color");
                            ImGui::ColorEdit4("Color##CrosshairColor", (float*)&crosshairColor);  // Color picker for crosshair
                        }

                        if (showCrosshairCircle)
                        {
                            ImGui::Text("Circle Size");
                            ImGui::SliderFloat("Size##CircleSize", &circleSize, 10.0f, 100.0f);  // Slider to control circle size
                            ImGui::Text("Circle Color");
                            ImGui::ColorEdit4("Color##CircleColor", (float*)&circleColor);  // Color picker for circle
                        }
                        ImGui::EndTabItem();
                    }
                    ImGui::PopID();

                    ImGui::EndTabBar();
                }

                // Drawing Crosshair if enabled
                if (showCrosshair)
                    Crosshair::DrawCrosshair(screenWidth, screenHeight, crosshairColor, crosshairSize);

                // Drawing Circle if enabled
                if (showCrosshairCircle)
                    Crosshair::DrawCircle(screenWidth, screenHeight, circleColor, circleSize);

                ImGui::End();
            }

            // Rendering process
            ImGui::Render();
            constexpr float color[4]{ 0.f, 0.f, 0.f, 0.f };  // Transparent background color

            device_contex->OMSetRenderTargets(1U, &render_target_view, nullptr);
            device_contex->ClearRenderTargetView(render_target_view, color);

            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }

        swap_chain->Present(1U, 0U);  // Present the render
    }

    // Cleanup and shutdown
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (swap_chain)
        swap_chain->Release();

    if (device_contex)
        device_contex->Release();

    if (device)
        device->Release();

    if (render_target_view)
        render_target_view->Release();

    DestroyWindow(window);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}
