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

Image morphology(const Image& img, const Image& kernel,
                 int centerRow, int centerCol, const std::string& mode) {
    int rows = img.size();
    int cols = img[0].size();
    int kRows = kernel.size();
    int kCols = kernel[0].size();

    Image result(rows, std::vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            bool hitAny = false;  
            bool hitAll = true;    
            bool masked  = false;  

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
            } else { 
                result[i][j] = (masked && hitAll) ? 1 : 0;
            }
        }
    }
    return result;
}

int main() {

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


    Image kernelSquare = {
        {1,1,1},
        {1,1,1},
        {1,1,1},
    };

   
    Image kernelCircle = {
        {0,1,1,1,0},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {0,1,1,1,0},
    };

    int c3r = 3 / 2, c3c = 3 / 2;
    int c5r = 5 / 2, c5c = 5 / 2;

    std::cout << "原图（8x8正方形）\n";
    printImage(square);

    std::cout << "dilation1：方形核膨胀（直角棱角）\n";
    printImage(morphology(square, kernelSquare, c3r, c3c, "dilate"));

    std::cout << "dilation2：圆形核膨胀（圆角）\n";
    printImage(morphology(square, kernelCircle, c5r, c5c, "dilate"));

    std::cout << "腐蚀：方形核（前景收缩、细化）\n";
    printImage(morphology(square, kernelSquare, c3r, c3c, "erode"));

    return 0;
}
