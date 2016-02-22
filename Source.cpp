#include <vector>
#include <iostream>
#include <thread>
#include <fstream>

// Declare Functions
void build_cluster(std::vector<std::vector<std::vector<int> > >* shot_loc, std::vector < std::vector<int> > list_of_hits);
void print(std::vector<std::vector<std::vector<std::vector<int> > > >* shot_list);

// Declare Vector Structures
std::vector<std::vector<int> > input_holder;
std::vector < std::vector < std::vector<std::vector<int> > > > shot_list(1000);
std::vector<std::vector<std::vector<int> > > cluster_list;
std::vector<std::vector<int> > hit_list;
std::vector<int> hit(4);


// Declare Threads
std::vector<std::thread> threads;

// Integer Declarations
int x, y, t, shot, bin;
int ending;

// Integer Definitions
int thread_count = 0;
int shot_count = 1;
int main()
{
	std::cout << std::thread::hardware_concurrency() << std::endl;
	shot_list.resize(1000);
	std::cout << "Method Solved?" << std::endl;
	std::cout << "Intialising centroiding software." << std::endl << "Reading in and identifying clusters..." << std::endl;
	std::cout << shot_count;
	std::ifstream InputFile;
	InputFile.open("/users/mb15852/Data_15_2/20160215_014");
	if (InputFile.is_open())
	{
		while (InputFile >> x >> y >> t >> shot >> bin)
		{
			//std::cout << "Shot = " << shot << " shot_count = " << shot_count << std::endl;
			if (shot_count != shot) // Every time the shot number changes
			{
				//std::cout << "Building the cluster for shot: " << shot -1 << std::endl;
				//build_cluster(shot_list, input_holder, shot - 1);
				//std::cout << "Thread count = " << thread_count << std::endl;
				/*if (thread_count == 2)
				{	std::cout << "Ending threads.";
					for (int i = 0; i < threads.size(); ++i)
					{
						std::cout << "Ending thread: " << i << std::endl;
						threads[i].std::thread::join();
						threads.erase(threads.begin(), threads.end());
						std::cout << "This bit" << std::endl;
						thread_count = 0;

					}

				}*/
				//std::cout << "New shot." << std::endl;
				//shot_list.push_back(cluster_list);
				//threads.push_back(std::thread(print, &shot_list)); // Create a new thread and launch it
				threads.push_back(std::thread(build_cluster, &shot_list[shot-2], input_holder)); // shot -1 to because the shot iterator has already increased, - another one to account for index from 0 in vector.
				//std::cout << "Thread launched for shot: " << shot-1 << std::endl;
				//std::cout << "Input holder size before erase: " << input_holder.size() << std::endl;
				input_holder.erase(input_holder.begin(), input_holder.end()); // Clears the input_holder vector for the next shot
				//std::cout << "Input holder after erase: " << input_holder.size() << std::endl;
				++shot_count;
				++thread_count;
				//get_thread:;
				//threads[0].join();
				//goto exit;
			}

			input_holder.push_back(hit); // Creates a raw list of hits for the shot
			input_holder[input_holder.size() - 1][0] = t;
			input_holder[input_holder.size() - 1][1] = x;
			input_holder[input_holder.size() - 1][2] = y;
			input_holder[input_holder.size() - 1][3] = bin;
		}
		goto exit;
	}
	else
	{
		std::cout << "File not opened." << std::endl;
		return 0;
	}
	exit:
		std::cout << "Clusters built." << std::endl;

		std::cout << "Joining threads.." << std::endl;
		for (int i = 0; i < threads.size(); ++i)
		{
			threads[i].std::thread::join();
		}
		std::cout << "Completed." << std::endl;
		char ending;
		std::cin >> ending;
		return 0;


	

}

void print(std::vector<std::vector<std::vector<std::vector<int> > > >* shot_list)
{
	std::cout << "This function works." << std::endl;
}

void build_cluster(std::vector<std::vector<std::vector<int> > >* shot_loc, std::vector < std::vector<int> > list_of_hits)
{
	//Declarations
	std::vector<std::vector<std::vector<int> > > cluster_list;

	cluster_list.push_back(hit_list); //Handles the initial case
	cluster_list[0].push_back(hit);
	cluster_list[0][0][0] = list_of_hits[0][0];
	cluster_list[0][0][1] = list_of_hits[0][1];
	cluster_list[0][0][2] = list_of_hits[0][2];
	cluster_list[0][0][3] = list_of_hits[0][3];
	
	for (int list = 1; list < list_of_hits.size(); ++list)
	{
		for (int i = 0; i < cluster_list.size(); ++i)
		{
			for (int j = 0; j < cluster_list[i].size(); ++j)
			{
				if (list_of_hits[list][0] >= cluster_list[i][j][0] - 1 && list_of_hits[list][0] <= cluster_list[i][j][0] + 1 && list_of_hits[list][1] >= cluster_list[i][j][1] - 1 && list_of_hits[list][1] <= cluster_list[i][j][1] + 1 && list_of_hits[list][2] >= cluster_list[i][j][2] - 1 && list_of_hits[list][2] <= cluster_list[i][j][2] + 1)
				{
					cluster_list[i].push_back(hit);
					cluster_list[i][cluster_list[i].size() - 1][0] = list_of_hits[list][0];
					cluster_list[i][cluster_list[i].size() - 1][1] = list_of_hits[list][1];
					cluster_list[i][cluster_list[i].size() - 1][2] = list_of_hits[list][2];
					cluster_list[i][cluster_list[i].size() - 1][3] = list_of_hits[list][3];
					goto added_to_cluster;
				}
			}
		}
	cluster_list.push_back(hit_list);
	cluster_list[cluster_list.size() - 1].push_back(hit);
	cluster_list[cluster_list.size() - 1][0][0] = list_of_hits[list][0];
	cluster_list[cluster_list.size() - 1][0][1] = list_of_hits[list][1];
	cluster_list[cluster_list.size() - 1][0][2] = list_of_hits[list][2];
	cluster_list[cluster_list.size() - 1][0][3] = list_of_hits[list][3];
	added_to_cluster:;
	}

 *shot_loc= cluster_list; 
}
