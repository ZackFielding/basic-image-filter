#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

/*
 * Currently stuck in infinite loop while generating bird2_copy.ppm
 * */

struct compare_result
{
	bool dif;
	long long location;
};

void compareFiles(std::fstream& o_f, std::fstream& n_f,
		compare_result& cr)
{
	std::string o_str {}, n_str{};
	unsigned long long int count {0};
	while(!o_f.eof())
	{
		o_f >> o_str;
		n_f >> n_str;
		if (o_str == n_str)
			++count;
		else
		{
			cr.dif = true;
			cr.location = count;
			break;
		}
	}
	
	if (!o_f) // if end of file reached (i.e., not diff found)
	{	
		cr.dif = false;
		cr.location = -1;
	}
}

void readFindings(const compare_result& cr, const int run_num)
{
	std::cout << "Run " << run_num << "  results: \n{Files are different: " 
		<< std::boolalpha << cr.dif << ".  \n\tDifference incident loc.: "
		<< cr.location << "} " << std::endl;
}

int main()
{
	std::fstream o_f, n_f;
	o_f.open("bird2.ppm", std::ios_base::in);
	n_f.open("filtered_image.ppm", std::ios_base::in);
	compare_result cr1, cr2;

	// compare findings and read to user
	compareFiles(o_f, n_f, cr1);
	readFindings(cr1, 1);
		
	o_f.seekg(std::ios_base::beg); // set pointer to start
	n_f.close();
	n_f.open("bird2_copy.ppm", std::ios_base::out);
	
	std::stringstream ss;
	// prepare file 2 for compare run 2
	if (n_f)
	{
		while(!o_f.eof())
		{
			static std::string o_str;
			static int hold {};
			o_f >> o_str;
			ss << o_str;
			try 
			{
				hold = std::stoi(ss.str());
				n_f << hold;
			}
			catch (std::invalid_argument& arg)
				{ n_f << ss.str(); } // if throw invalid -> write string
			n_f << " ";
			ss.str("");
			o_str.clear();
		}	
		compareFiles(o_f, n_f, cr2);
		readFindings(cr2, 2);	
	}
	else
	{
		std::cout << "File 2 could not be opened.\n";
	}

	return 0;
}
