#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

const bool darkTheme = true;
const int cellSize = 100;
const int cellsCountX = 10;
const int cellsCountY = 10;

struct Grid {
    int x, y;
    int cellSize, width, height;

    void drawCell(int x, int y, Color color) {
        DrawRectangle(this->x + x * cellSize, this->y + y * cellSize, cellSize, cellSize, color);
    }

    void drawGrid(Color color, Color color1) {
        for(int y = 0; y < height; y++) {
            for(int x = 0; x < width; x++) {
                if((x + y) % 2 == 0) {
                    drawCell(x, y, color);
                }
                else {
                    drawCell(x, y, color1);
                }
            }
        }
    }
};

struct Snake {
    int x, y;
    const float moveInterval;
    Color color;

    void drawOn(Grid *grid) {
        grid->drawCell(x, y, color);
    }

    void wrappedMove(Grid *grid, int moveX, int moveY) {
        x += moveX;
        y += moveY;
        if(x < 0) x = grid->width - 1;
        if(x >= grid->width) x = 0;
        if(y < 0) y = grid->height - 1;
        if(y >= grid->height) y = 0;
    }
};

Color clr(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
    return {r, g, b, a};
}

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

void drawGui() {
    ImGui::Begin("Test");
    ImGui::Text("Hello");
    ImGui::Button("Button");
    ImGui::Button("Button2");
    ImGui::End();
}

void render(Grid *grid, Snake *snake) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    rlImGuiBegin();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
    ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    ImGui::PopStyleColor(2);

    //drawGui();

    Color checkerLight = clr(50, 50, 50), checkerDark = clr(70, 70, 70);

    grid->drawGrid(checkerLight, checkerDark);
    snake->drawOn(grid);

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
    InitWindow(cellSize * cellsCountX, cellSize * cellsCountY, "dev: snake");
    SetTargetFPS(60);

    setupImgui(darkTheme, 2, 0.5f);


    Grid gameGrid = { 0, 0, cellSize, cellsCountX, cellsCountY };
    Snake snake = {5, 5, 0.2f, GREEN};
    Vector2 direction = {1, 0};

    float timer = snake.moveInterval;

    while (!WindowShouldClose()) {
        render(&gameGrid, &snake);

        if(IsKeyDown(KEY_LEFT)) { direction = {-1, 0}; }
        if(IsKeyDown(KEY_DOWN)) { direction = {0, 1}; }
        if(IsKeyDown(KEY_UP)) { direction = {0, -1}; }
        if(IsKeyDown(KEY_RIGHT)) { direction = {1, 0}; }

        float deltaTime = GetFrameTime();
        timer -= deltaTime;

        if(timer <= 0) {
            snake.wrappedMove(&gameGrid, direction.x, direction.y);
            timer = snake.moveInterval;
        }
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
