#include <iostream>
#include <fstream>
#include <array>
#include <sstream>
#include <string>
#include <map>
#include <cstring>
#include "image_filter_funcs.hpp"

bool get_file_names_types(char& file_type, std::array<char, 50>& read_file,
		std::array<char, 50>& open_file)
{
	std::cout << "Enter file name (with file type post-fix, e.g., .ppm) and hit <ENTER>: " << std::endl;
	std::cin.getline(&read_file.at(0), (sizeof read_file)-1, '\n');
	generateOutputFile(read_file, open_file); // auto gen new open file name
	std::cout << "Is the file BINARY [B] or DECIMAL [D]. " << std::endl;
	const std::map<char, std::string> type_return {{'B', "Binary"}, {'D', "Decimal"}};
	char ans {};
	constexpr char b[]{"B"}, d[]{"D"};
	while (std::cin >> ans)
	{
		if (std::strncmp(&ans, b, 1) == 0 || std::strncmp(&ans, d, 1) == 0)
		{
			std::cout << type_return.at(ans) << " file type selected." << std::endl;
			return true;
		}
		else
		{
			std::cout << "Enter valid selection - [B] BINARY or [D] DECIMAL." 
				<< std::endl;
		}
	}
	return false;
}

void generateOutputFile(const std::array<char, 50>& read_file, std::array<char, 50>& open_file)
{
	constexpr char append_str[] {"_updated."};
	unsigned ind {0}, cor_fac {0};

	while (read_file.at(ind) != '\0')
	{
		if (read_file.at(ind) != '.')
			open_file.at(ind++ + cor_fac) = read_file.at(ind);
		else 
		{
			std::strcat(&open_file.at(0), append_str);
			++ind;
			cor_fac = (sizeof append_str) - 1; // set correction fac for next loop
		}
	}
	std::cout.write(&open_file.at(0), sizeof open_file) << " [new file].\n"; 
}

template<typename SIZE>
void getAndPut(std::fstream& stream, char str[], const SIZE& size)
{
	unsigned int i {0};
	while (i < ((size_t)size-1) && stream.peek() != '\n' && stream.peek() != ' ')
		stream.get(str[i++]);

	stream.get(); // remove spacer
	str[i] = '\0'; // add null term (why loop condition is size-1)
}

void get_image_headers(std::fstream& read_image, std::fstream& open_image)
{
	const int max_str_size {4};

	char type[4], width[4], height[4], RGB[4];
	const int sub_ar_sizes {sizeof type};
	std::array<char*, 4> arr {type, width, height, RGB};

	// get image headers from original image
	for(unsigned int j {0}; j < arr.size(); ++j)
		getAndPut<int>(read_image, arr.at(j), sub_ar_sizes);
	
	// index header info into new image
	open_image << arr.at(0) << "\n" << arr.at(1) << " "
		<< arr.at(2) << "\n" << arr.at(3) << "\n";
}

void generate_image(std::fstream& read_image, std::fstream& open_image)
{
	std::string temp;
	std::array<std::stringstream,3> ss_array;	
	auto ss_array_s_it {ss_array.begin()};
	std::map<char, const int>RGB_map{
			{'R', 0},
			{'G', 75},
			{'B', 0}};
	std::array<char, 3> char_array {'R', 'G', 'B'};
	auto st_it {char_array.begin()}, end_it {char_array.end()};
	int non_adj_val {0}, adj_val {0}; // will hold corrected RGB value

	while(read_image >> temp)
	{
		while (st_it != end_it)
		{
			if (RGB_map.find(*st_it) != RGB_map.end())	
			{
				// if key value does not exist -> returns end iterator
				// key exists -> check if filter can be applied 
				*ss_array_s_it << temp; // pipe temp as dec into array of stringstreams
				non_adj_val = std::stoi(ss_array_s_it->str());
				adj_val = non_adj_val + RGB_map.at(*st_it);
				if (adj_val <= 250)
				{
					open_image << adj_val << " ";
				}
				else
				{
					open_image << non_adj_val << " ";	
				}
				ss_array_s_it->str(""); // reset string stream
			}else
			{
				open_image << temp << " ";
			}
			++st_it; // need to ++ start it even if no key found on current loop
			++ss_array_s_it;
		}
		st_it = char_array.begin(); // reset to start prior to next RGB loop
		ss_array_s_it = ss_array.begin(); // reset stringstream array iterator
	}
}
