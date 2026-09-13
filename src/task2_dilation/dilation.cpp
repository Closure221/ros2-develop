#include <iostream>
#include <vector>
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

Image dilate(const Image& img, const Image& kernel, int centerRow, int centerCol) {
    int rows = img.size();
    int cols = img[0].size();
    int kRows = kernel.size();
    int kCols = kernel[0].size();

    Image result(rows, std::vector<int>(cols, 0));  

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            bool hit = false;  

            for (int ki = 0; ki < kRows && !hit; ++ki) {
                for (int kj = 0; kj < kCols && !hit; ++kj) {
                    if (kernel[ki][kj] == 0) continue;  

                   
                    int ti = i + (ki - centerRow);
                    int tj = j + (kj - centerCol);

            
                    if (ti < 0 || ti >= rows || tj < 0 || tj >= cols) continue;

                    if (img[ti][tj] == 1) hit = true;
                }
            }
            result[i][j] = hit ? 1 : 0;
        }
    }
    return result;
}

int main() {
   
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

  
    Image kernel5 = {
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
    };

   
    int cRow = 5 / 2;
    int cCol = 5 / 2;

    std::cout << "原图（12x12二值）\n";
    printImage(input);

    Image out5 = dilate(input, kernel5, cRow, cCol);
    std::cout << "5x5全1核膨胀结果\n";
    printImage(out5);

    return 0;
}
