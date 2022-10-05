#ifndef IMAGE_FILTER_FUNCS_HPP
#define IMAGE_FILTER_FUNCS_HPP

#include <fstream>
#include <array>

bool get_file_names_types(char&, std::array<char, 50>&, std::array<char, 50>&);

void generateOutputFile(const std::array<char, 50>&, std::array<char, 50>&);

template<typename SIZE>
void getAndPut(std::fstream&, char [], const SIZE&);

void get_image_headers(std::fstream&, std::fstream&);

void generate_image(std::fstream&, std::fstream&);



#endif
