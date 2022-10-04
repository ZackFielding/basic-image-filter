#include <iostream>
#include <fstream>
#include <array>
#include "image_filter_funcs.hpp"

// Fixed:
	//  - manually adjusted filter now works as intended
//TO-DO:
	// - user input
	// - auto detect if file currently exists -> delete
	// - add option to use binary files
	// - add other image filtering techniques (date/time/border?)
	// - contrast/border detection

int main()
{
	//std::ifstream r_image;
	//std::ofstream o_image;
	std::array<char, 50> read_image_file_name, open_image_file_name;

	char file_type;
	while (!file_type)
	{
		get_file_names_types(file_type, read_image_file_name,
			open_image_file_name);
	}

	/*
	r_image.open("bird2.ppm");
	o_image.open("filtered_image.ppm");

	// read image headers & index into new image
	get_image_headers(r_image, o_image);	
	
	// go int by int to allow modification of new image based on original
	generate_image(r_image, o_image);

	r_image.close();
	o_image.close();
	*/
	return 0;
}
