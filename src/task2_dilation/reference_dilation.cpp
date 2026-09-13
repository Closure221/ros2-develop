// ============================================================
// task2_dilation 参考实现（带逐行注释，供学习理解）
// 功能：对 12x12 二值图像做 5x5 形态学膨胀
// 作者：FSWorkspace 辅导参考   —— 看完后请自己动手写一份
// ============================================================

#include <iostream>
#include <vector>

// 用别名让类型好读：Image 就是一个二维 int 数组（0/1）
using Image = std::vector<std::vector<int>>;

// ------------------------------------------------------------
// 打印二值图像：把 0/1 矩阵打印成可读的样子
// ------------------------------------------------------------
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
// 膨胀核心函数 dilate
// 参数：img      原二值图像
//       kernel   结构元素（0/1 二维数组，如 5x5 全 1）
//       centerRow、centerCol  结构元素的"中心锚点"坐标
// 返回：膨胀后的新图像（不改动原图）
// 思路：对输出图像每个位置 (i,j)，把 kernel 盖上去，
//       kernel 值为 1 的格子对应原图位置，只要有一个是 1，
//       (i,j) 就置 1；否则为 0。
// ------------------------------------------------------------
Image dilate(const Image& img, const Image& kernel, int centerRow, int centerCol) {
    int rows = img.size();
    int cols = img[0].size();
    int kRows = kernel.size();
    int kCols = kernel[0].size();

    Image result(rows, std::vector<int>(cols, 0));  // 输出全 0

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            bool hit = false;  // 邻域里有没有命中 1

            for (int ki = 0; ki < kRows && !hit; ++ki) {
                for (int kj = 0; kj < kCols && !hit; ++kj) {
                    if (kernel[ki][kj] == 0) continue;  // 核为0的格子不参与

                    // kernel 格子对应原图的绝对坐标
                    int ti = i + (ki - centerRow);
                    int tj = j + (kj - centerCol);

                    // 越界跳过（边缘朴素处理）
                    if (ti < 0 || ti >= rows || tj < 0 || tj >= cols) continue;

                    if (img[ti][tj] == 1) hit = true;
                }
            }
            result[i][j] = hit ? 1 : 0;
        }
    }
    return result;
}

// ------------------------------------------------------------
// 主程序：定义输入图像、结构元素，执行膨胀并打印对比
// ------------------------------------------------------------
int main() {
    // 文档给出的 12x12 二值图像
    Image input = {
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,0,0,0,0,0},
        {0,0,0,0,1,0,1,0,0,0,0,0},
        {0,0,0,0,0,1,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,1,0,0,0,0},
        {0,0,0,0,0,1,1,1,0,0,0,0},
        {0,0,0,0,0,0,0,1,1,0,0,0},
        {0,0,0,0,0,0,0,1,1,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
    };

    // 5x5 全 1 结构元素（任务要求）
    Image kernel5 = {
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
    };

    // 中心锚点：5x5 中心是 (2,2)
    int cRow = 5 / 2;
    int cCol = 5 / 2;

    std::cout << "===== 原图（12x12 二值） =====\n";
    printImage(input);

    Image out5 = dilate(input, kernel5, cRow, cCol);
    std::cout << "===== 5x5 全1核膨胀结果 =====\n";
    printImage(out5);

    return 0;
}
