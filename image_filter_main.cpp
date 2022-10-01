#include <iostream>
#include <fstream>
#include <array>
#include <sstream>
#include <string>
#include <map>

// Fixed:
	//  - manually adjusted filter now works as intended
//TO-DO:
	// - user input
	// - auto detect if file currently exists -> delete
	// - add option to use binary files
	// - add other image filtering techniques (date/time/border?)
	// - contrast/border detection

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

	while(!read_image.eof())
	{
		while (st_it != end_it)
		{
			read_image >> temp;
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

int main()
{
	std::fstream r_image, o_image;
	r_image.open("bird2.ppm", std::ios_base::in);
	o_image.open("filtered_image.ppm", std::ios_base::out);

	// read image headers & index into new image
	get_image_headers(r_image, o_image);	
	
	// go int by int to allow modification of new image based on original
	generate_image(r_image, o_image);

	r_image.close();
	o_image.close();

	return 0;
}
