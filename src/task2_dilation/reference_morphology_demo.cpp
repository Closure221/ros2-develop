// ============================================================
// 思考题三合一演示（供学习理解）
// 1) dilation1：方形核膨胀 -> 直角扩展边界
// 2) dilation2：圆形核膨胀 -> 圆角扩展边界
// 3) 腐蚀   ：反向操作 -> 前景收缩
// 全部基于"结构元素可替换"的模块化设计（思考题三）
// ============================================================

#include <iostream>
#include <vector>
#include <string>

using Image = std::vector<std::vector<int>>;

void printImage(const Image& img) {
    for (const auto& row : img) {
        for (int val : row) {
            std::cout << (val ? "1 " : "0 ");
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

// ------------------------------------------------------------
// 通用形态学操作 core
// mode = "dilate"：窗口内至少一个 1 -> 中心 1（膨胀，扩大前景）
// mode = "erode" ：窗口内全部是 1 -> 中心 1（腐蚀，收缩前景）
// kernel 作为参数传入 -> 想换核只改调用处，不用改函数（模块化）
// ------------------------------------------------------------
Image morphology(const Image& img, const Image& kernel,
                 int centerRow, int centerCol, const std::string& mode) {
    int rows = img.size();
    int cols = img[0].size();
    int kRows = kernel.size();
    int kCols = kernel[0].size();

    Image result(rows, std::vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            bool hitAny = false;   // 窗口里有 1（膨胀用）
            bool hitAll = true;    // 窗口里全是 1（腐蚀用）
            bool masked  = false;  // 是否被核遮住至少一个有效格子

            for (int ki = 0; ki < kRows; ++ki) {
                for (int kj = 0; kj < kCols; ++kj) {
                    if (kernel[ki][kj] == 0) continue;

                    int ti = i + (ki - centerRow);
                    int tj = j + (kj - centerCol);

                    if (ti < 0 || ti >= rows || tj < 0 || tj >= cols) continue;

                    masked = true;
                    if (img[ti][tj] == 1) hitAny = true; else hitAll = false;
                }
            }

            if (mode == "dilate") {
                result[i][j] = hitAny ? 1 : 0;
            } else { // erode
                result[i][j] = (masked && hitAll) ? 1 : 0;
            }
        }
    }
    return result;
}

int main() {
    // 用一个 8x8 的正方形前景来演示直角 vs 圆角（对应题目里的图）
    Image square = {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,1,1,1,1,0,0},
        {0,0,1,1,1,1,0,0},
        {0,0,1,1,1,1,0,0},
        {0,0,1,1,1,1,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
    };

    // 方形核（全 1）-> dilation1 直角
    Image kernelSquare = {
        {1,1,1},
        {1,1,1},
        {1,1,1},
    };

    // 圆形核（近似圆盘）-> dilation2 圆角
    // 5x5 圆盘：四角为 0（距中心超过半宽的格子为 0），其余为 1
    Image kernelCircle = {
        {0,1,1,1,0},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {0,1,1,1,0},
    };

    int c3r = 3 / 2, c3c = 3 / 2;
    int c5r = 5 / 2, c5c = 5 / 2;

    std::cout << "===== 原图（8x8 正方形） =====\n";
    printImage(square);

    std::cout << "===== dilation1：方形核膨胀（直角棱角） =====\n";
    printImage(morphology(square, kernelSquare, c3r, c3c, "dilate"));

    std::cout << "===== dilation2：圆形核膨胀（圆角） =====\n";
    printImage(morphology(square, kernelCircle, c5r, c5c, "dilate"));

    std::cout << "===== 腐蚀：方形核（前景收缩、细化） =====\n";
    printImage(morphology(square, kernelSquare, c3r, c3c, "erode"));

    return 0;
}
