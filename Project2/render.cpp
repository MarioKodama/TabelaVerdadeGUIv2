#include "render.h"
#include "incl.h"
IDXGISwapChain* swapchain;
ID3D11Device* device;
ID3D11DeviceContext* context;
ID3D11RenderTargetView* g_mainRenderTargetView;
HWND hwnd;
bool visible = true;
bool imp = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Init(HINSTANCE hInstance)
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.hInstance = hInstance;
    wc.lpszClassName = L"Tabela Verdade";
    wc.lpfnWndProc = WindowProc;

    RegisterClassEx(&wc);

    hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST, L"Tabela Verdade", nullptr, WS_POPUP,
        0, 0, 1920, 1080,
        nullptr, nullptr, hInstance, nullptr);

    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);

    MARGINS m = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &m);
    ShowWindow(hwnd, SW_SHOW);





    SetupD3D(hwnd);
    ImGuiInit();
    
}

void SetupD3D(HWND hWnd)
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
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    HRESULT res = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
        featureLevelArray, 2, D3D11_SDK_VERSION,
        &sd, &swapchain, &device, &featureLevel, &context);

    if (res == DXGI_ERROR_UNSUPPORTED)
    {
        res = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags,
            featureLevelArray, 2, D3D11_SDK_VERSION,
            &sd, &swapchain, &device, &featureLevel, &context);
    }

    ID3D11Texture2D* pBackBuffer = nullptr;
    swapchain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    device->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void ImGuiInit()
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, context);
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.6f, 0.0f, 0.0f, 1.0f);
    style.WindowShadowSize = 0.0f;
    style.FrameRounding = 10.0f;
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); 
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f); 
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); 
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); 
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 1.0f); 
    style.FrameBorderSize = 1.0f;
    style.WindowRounding = 10.f;
    


    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();




        if (GetAsyncKeyState(VK_INSERT) & 1)visible = !visible;
        if (GetAsyncKeyState(VK_DELETE) & 1)break;


        LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
        if (!visible) {

            SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_TRANSPARENT);
        }
        else {

            SetWindowLong(hwnd, GWL_EXSTYLE, exStyle & ~WS_EX_TRANSPARENT);
        }

        RenderWindow();


        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0, 0, 0, 0 };
        context->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        context->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        swapchain->Present(1, 0);
    }


    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void RenderWindow() {

    if (visible) {
        ImGui::Begin("Tabela Verdade", 0, ImGuiWindowFlags_NoTitleBar);

        static char buffer[1024] = "";
        static vector<Token> ptokens;
        static vector<char> varlist;
        static bool generated = false;
        static string error;
        static int trues;
        static int falses;
        static vector<unsigned int> minterms;      
        static string minimized;
        static bool show = false;
        ImGui::ShowDebugLogWindow();
        ImGui::InputText("Expressao", buffer, sizeof(buffer));
        ImGui::Checkbox("Implication Precedence", &imp);
        ImGui::SameLine(0, 50);
        if (ImGui::Button("Calcular", {100, 20})) {
            error.clear();
            minterms.clear();  
            minimized.clear();
            trues = 0;
            falses = 0;
            generated = false;
            show = false;
            try {
                string expr = buffer;
                auto tokens = Tokenize(expr);
                ptokens = RPN(tokens);
                set<char> temp;
                for (auto& t : ptokens) {
                    if (t.type == VAR)
                        temp.insert(t.c);
                }
                varlist = vector<char>(temp.begin(), temp.end());
                generated = true;
            }
            catch (...) {
                error = "Erro ao processar expressao";
            }
        }

        if (!error.empty()) {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", error.c_str());
        }

        if (generated) {

            vector<Var> dummy;
            for (auto c : varlist)
                dummy.push_back({ c, false });

            vector<Step> headerSteps = EvaluateRPN(ptokens, dummy);

            int totalColumns = varlist.size() + headerSteps.size();

            ImGuiTableFlags flags =
                ImGuiTableFlags_Borders |
                ImGuiTableFlags_RowBg |
                ImGuiTableFlags_ScrollY;

            if (ImGui::BeginTable("TruthTable", totalColumns, flags, ImVec2(0, 400))) {

                ImGui::TableSetupScrollFreeze(0, 1);

                ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

                int col = 0;

                for (auto& var : varlist) {
                    ImGui::TableSetColumnIndex(col++);
                    ImGui::Text("%c", var);
                }

                for (auto& step : headerSteps) {
                    ImGui::TableSetColumnIndex(col++);
                    ImGui::Text("%s", step.expr.c_str());
                }


               
                

                
                for (int i = 0; i < (1 << varlist.size()); i++) {

                    ImGui::TableNextRow();

                    vector<Var> vars;
                    col = 0;
                    for (unsigned int j = 0; j < varlist.size(); j++) {
                        ImGui::TableSetColumnIndex(col++);
                        bool bit = (i >> (varlist.size() - 1 - j)) & 1;
                        vars.push_back({ varlist[j], bit });
                        ImVec4 color = bit ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1);
                        ImVec2 pos = ImGui::GetCursorScreenPos();
                        bit ? ImGui::ShadowText("True", ImColor(0, 255, 0), ImColor(0, 255, 0), 25) : ImGui::ShadowText("False", ImColor(255, 0, 0), ImColor(255, 0, 0), 25);
                        




                    }

                    vector<Step> steps = EvaluateRPN(ptokens, vars);
                    

                    
                   
                    for (auto& step : steps) {
                        ImGui::TableSetColumnIndex(col++);
                        step.bit ? ImGui::ShadowText("True", ImColor(0, 255, 0), ImColor(0, 255, 0), 25) : ImGui::ShadowText("False", ImColor(255, 0, 0), ImColor(255, 0, 0), 25);
                        bool isLast = (&step == &steps.back());
                        if (isLast) {
                            if (step.bit) {
                                trues++;
                                minterms.push_back(i); 
                            }
                            else {
                                falses++;
                            }
                        }
                        

                    }


                }
                
                
                ImGui::EndTable();
                
                
                if (!show) {
                    vector<Implicant> prime = GetPrimeImplicants(minterms, varlist.size());
                    minimized = MinExpressionToString(prime, varlist);
                }show = true;
               
            }




            if (!falses)ImGui::ShadowText("Tautologia", ImColor(255, 255, 255), ImColor(255, 255, 255), 25);
            else if (!trues)ImGui::ShadowText("Contradicão", ImColor(255, 255, 255), ImColor(255, 255, 255), 25);
            else ImGui::ShadowText("Contingencia", ImColor(255, 255, 255), ImColor(255, 255, 255), 25);
            if (!minimized.empty()) {
                string x = "Simplificada: " + minimized;
                ImGui::ShadowText(x.c_str(), ImColor(255, 255, 255), ImColor(255, 255, 255), 25);
            }


        }

        ImGui::End();
    }
}
