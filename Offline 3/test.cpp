#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#pragma pack(push, 1)
struct BMPHeader {
    char signature[2];
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t dataSize;
    int32_t horizontalResolution;
    int32_t verticalResolution;
    uint32_t colors;
    uint32_t importantColors;
};
#pragma pack(pop)

int main() {
    const char* filename = "image.bmp";
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    BMPHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));

    if (header.signature[0] != 'B' || header.signature[1] != 'M') {
        std::cerr << "Invalid BMP file" << std::endl;
        return 1;
    }

    if (header.bitsPerPixel != 24) {
        std::cerr << "Only 24-bit BMP files are supported" << std::endl;
        return 1;
    }

    std::vector<std::vector<uint8_t>> pixel_values(header.height, std::vector<uint8_t>(header.width * 3));

    // Seek to the beginning of the pixel data
    file.seekg(header.dataOffset);

    for (int y = 0; y < header.height; ++y) {
        file.read(reinterpret_cast<char*>(pixel_values[y].data()), header.width * 3);
    }

    file.close();
    // Now you can access pixel values using pixel_values[row][column]
    return 0;
}
