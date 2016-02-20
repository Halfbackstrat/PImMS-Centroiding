#include <vector>
#include <iostream>
#include <thread>
#include <fstream>

// Declare Functions
void build_cluster(std::vector < std::vector<int> > list_of_hits, int shot);
void print();

// Declare Vector Structures
std::vector<std::vector<int> > input_holder;
std::vector < std::vector < std::vector<std::vector<int> > > > shot_list;
std::vector<std::vector<std::vector<int> > > cluster_list;
std::vector<std::vector<int> > hit_list;
std::vector<int> hit(4);


// Declare Threads
std::vector<std::thread> threads;

// Integer Declarations
int x, y, t, shot, bin;
int ending;

// Integer Definitions
int thread_count = 1;
int shot_count = 1;
int main()
{
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
				
				if (thread_count == 2)
				{
					for (int i = 0; i < threads.size(); ++i)
					{
						//std::cout << "Ending thread: " << i << std::endl;
						threads[i].std::thread::join();
						threads.erase(threads.begin(), threads.end());
						thread_count = 1;
					}

				}
				//std::cout << "New shot." << std::endl;
				shot_list.push_back(cluster_list);
				threads.push_back(std::thread(print/*build_cluster, input_holder, shot - 1*/)); // Create a new thread and launch it
				//std::cout << "Thread launched for shot: " << shot-1 << std::endl;
				//std::cout << "Input holder size before erase: " << input_holder.size() << std::endl;
				input_holder.erase(input_holder.begin(), input_holder.end()); // Clears the input_holder vector for the next shot
				//std::cout << "Input holder after erase: " << input_holder.size() << std::endl;
				++shot_count;
				get_thread:;
				/*try
				{
					//threads[0].join();
				}
				catch (int e)
				{
					std::cout << e;
					goto get_thread;
				}*/
				
			}

			input_holder.push_back(hit); // Creates a raw list of hits for the shot
			input_holder[input_holder.size() - 1][0] = t;
			input_holder[input_holder.size() - 1][1] = x;
			input_holder[input_holder.size() - 1][2] = y;
			input_holder[input_holder.size() - 1][4] = bin;
		}

		/*for (int i = 0; i < threads.size(); ++i)
		{
			std::cout << "Ending thread: " << i << std::endl;
			threads[i].std::thread::join();
		}*/
		std::cout << "Print size" << std::endl;
		std::cout << "Shot_list size = " << shot_list.size() << "Clusters in shot 1: " << shot_list[0].size();// << "Hits in the first cluster = " << shot_list[0][0].size(); // << std::endl;
		std::cout << "End" << std::endl;

		char ending;
		std::cin >> ending;
	}

	

}

void print()
{
	std::cout << "This function works." << std::endl;
}

void build_cluster(std::vector < std::vector<int> > list_of_hits, int shot)
{
	//Declarations
	std::vector<std::vector<std::vector<int> > > cluster_list;

		cluster_list.push_back(hit_list); //Handles the initial case
		cluster_list[0].push_back(hit);
		cluster_list[0][0][0] = list_of_hits[0][0];
		cluster_list[0][0][1] = list_of_hits[0][1];
		cluster_list[0][0][2] = list_of_hits[0][2];
		cluster_list[0][0][3] = list_of_hits[0][3];
		/*
	
		for (int i = 1; i < list_of_hits.size(); ++i) // Cycle through unprocessed hits hits
		{
			for (int ccount = 0; ccount < cluster_list.size(); ++ccount) // Cycle through the clusters
			{
				for (int hitcount = 0; hitcount < cluster_list[ccount].size(); ++hitcount) // Cycle through the hitcounts
				{
					if (hit_list[i][0] >= cluster_list[ccount][hitcount][0] - 1 && hit_list[i][0] <= cluster_list[ccount][hitcount][0] + 1 && hit_list[i][1] >= cluster_list[ccount][hitcount][1] - 1 && hit_list[i][1] <= cluster_list[ccount][hitcount][1] + 1 && hit_list[i][2] >= cluster_list[ccount][hitcount][2] - 1 && hit_list[i][2] <= cluster_list[ccount][hitcount][2] + 1)
					{
						cluster_list[ccount].push_back(hit);
						cluster_list[ccount][cluster_list[ccount].size() - 1][0] = hit_list[i][0]; // t
						cluster_list[ccount][cluster_list[ccount].size() - 1][1] = hit_list[i][1]; // x
						cluster_list[ccount][cluster_list[ccount].size() - 1][2] = hit_list[i][2]; // y
						cluster_list[ccount][cluster_list[ccount].size() - 1][3] = hit_list[i][3]; // bin
						break;
					}
				}
			}
			cluster_list.push_back(hit_list);
			cluster_list[cluster_list.size() - 1].push_back(hit);
			cluster_list[cluster_list.size() - 1][0][0] = hit_list[i][0];
			cluster_list[cluster_list.size() - 1][0][1] = hit_list[i][1];
			cluster_list[cluster_list.size() - 1][0][2] = hit_list[i][2];
			cluster_list[cluster_list.size() - 1][0][3] = hit_list[i][3];

			shot_list[shot] = cluster_list; // Accesses the entry for this shot (no race condition as that shot can only be accessed by this function call)n
		}

*/
shot_list[shot] = cluster_list; 
}
