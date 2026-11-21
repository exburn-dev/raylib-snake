#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

const bool darkTheme = true;

void setupImgui(bool darkTheme, int fontScale, float bgAlpha) {
    rlImGuiSetup(darkTheme);

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.FontGlobalScale = fontScale;

    ImGuiStyle &style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.Colors[ImGuiCol_WindowBg].w = bgAlpha;
    }
}

void render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    rlImGuiBegin();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
    ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    ImGui::PopStyleColor(2);


    ImGui::Begin("Test");

    ImGui::Text("Hello");
    ImGui::Button("Button");
    ImGui::Button("Button2");

    ImGui::End();

    DrawText("Congrats! You draw your first window!", 190, 200, 20, LIGHTGRAY);

    rlImGuiEnd();
    ImGuiIO &io = ImGui::GetIO();
    if(io.ConfigFlags && ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    EndDrawing();
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "raylib [core] example");
    SetTargetFPS(60);

    setupImgui(darkTheme, 2, 0.5f);

    while (!WindowShouldClose()) {
        render();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
