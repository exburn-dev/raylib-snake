#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include <vector>

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

struct Fruit {
    int x, y;
    Color color;
};

struct Snake {
    std::vector<Vector2> segments;
    const float moveInterval;
    Color color;

    Snake(int x, int y, const float moveInterval, Color color)
        : moveInterval(moveInterval), color(color) {

        segments.push_back({(float) x, (float) y});
    }

    Vector2* head() {
        return &segments[0];
    }

    void addSegment() {
        segments.push_back(segments[segments.size() - 1]);
    }

    void drawOn(Grid *grid) {
        for(int i = 0; i < segments.size(); i++) {
            grid->drawCell(segments[i].x, segments[i].y, color);
        }
    }

    void wrappedMove(Grid *grid, int moveX, int moveY) {
        if(segments.size() > 1) {
            for(int i = segments.size() - 1; i > 0; i--) {
                segments[i] = segments[i - 1];
            }
        }

        Vector2 *head = &segments[0];

        head->x += moveX;
        head->y += moveY;
        if(head->x < 0) head->x = grid->width - 1;
        if(head->x >= grid->width) head->x = 0;
        if(head->y < 0) head->y = grid->height - 1;
        if(head->y >= grid->height) head->y = 0;
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

Fruit spawnFruit(Grid *grid, Snake *snake, Color fruitColor) {
    Fruit fruit = {0, 0, fruitColor};
    do {
        fruit.x = GetRandomValue(0, grid->width - 1);
        fruit.y = GetRandomValue(0, grid->height - 1);
    } while(fruit.x == snake->head()->x && fruit.y == snake->head()->y);

    return fruit;
}

void render(Grid *grid, Snake *snake, Fruit *fruit) {
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
    grid->drawCell(fruit->x, fruit->y, fruit->color);
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


    Grid grid = { 0, 0, cellSize, cellsCountX, cellsCountY };
    Snake snake = {5, 5, 0.2f, GREEN};
    Fruit fruit = spawnFruit(&grid, &snake, RED);
    Vector2 direction = {1, 0};

    float timer = snake.moveInterval;

    while (!WindowShouldClose()) {
        render(&grid, &snake, &fruit);

        if(IsKeyDown(KEY_LEFT)) { direction = {-1, 0}; }
        if(IsKeyDown(KEY_DOWN)) { direction = {0, 1}; }
        if(IsKeyDown(KEY_UP)) { direction = {0, -1}; }
        if(IsKeyDown(KEY_RIGHT)) { direction = {1, 0}; }

        float deltaTime = GetFrameTime();
        timer -= deltaTime;

        if(timer <= 0) {
            snake.wrappedMove(&grid, direction.x, direction.y);
            timer = snake.moveInterval;
        }

        if(snake.head()->x == fruit.x && snake.head()->y == fruit.y) {
            snake.addSegment();
            fruit = spawnFruit(&grid, &snake, fruit.color);
        }
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
