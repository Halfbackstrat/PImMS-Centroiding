#include <vector>
#include <iostream>
#include <thread>
#include <fstream>
#include <algorithm>

// Declare Functions
void build_cluster(std::vector<std::vector<std::vector<int> > >* shot_loc, std::vector < std::vector<int> > list_of_hits);
//void Centroid_image(vector<vector<int> > &cluster, int shot, int cluster_no);
void print(std::vector<std::vector<std::vector<std::vector<int> > > >* shot_list);
void order_clusters(std::vector<std::vector<std::vector<int> > >* shot_loc);

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

// Integer Definitions
int shot_count = 1;
int main()
{
	int max_threads = std::thread::hardware_concurrency();
	shot_list.resize(1000);
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
				/*if (thread_count >= max_threads - 1)
				{	//std::cout << "Refresh threads" << std::endl;
					for (int i = 0; i < threads.size(); ++i)
					{
						//std::cout << "Ending thread: " << i << std::endl;
						threads[i].std::thread::join();
					}
					threads.erase(threads.begin(), threads.end());
					thread_count = 0;

				}*/
				//std::cout << "Submit shot " << shot  - 1 << std::endl; 
				threads.push_back(std::thread(build_cluster, &shot_list[shot-2], input_holder)); // shot -1 to because the shot iterator has already increased, - another one to account for index from 0 in vector.
				input_holder.erase(input_holder.begin(), input_holder.end()); // Clears the input_holder vector for the next shot
				++shot_count;



			}

			input_holder.push_back(hit); // Creates a raw list of hits for the shot
			input_holder[input_holder.size() - 1][0] = t;
			input_holder[input_holder.size() - 1][1] = x;
			input_holder[input_holder.size() - 1][2] = y;
			input_holder[input_holder.size() - 1][3] = bin;
		}
		InputFile.close();
	}
		
	else
	{
		std::cout << "File not opened." << std::endl;
		return 0;
	}

	std::cout << "Clusters built." << std::endl << "Joining threads.." << std::endl;
		for (int i = 0; i < threads.size(); ++i)
		{
			threads[i].std::thread::join();
		}
		threads.erase(threads.begin(), threads.end());
		std::cout << "Threads joined." << std::endl;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
																							/* File Imported & Clusters Built - Centroiding Code */
		char pause;
		std::cout << "Ready to centroid." << std::endl;
		std::cin >> pause;

		// Sort the clusters by time
		std::cout << "Arranging the clusters by time..." << std::endl;
		for (int i = 0; i < shot_list.size(); ++i)
			{
				threads.push_back(std::thread(order_clusters, &shot_list[i]));
				goto temp1;
			}
		temp1:
		std::cout << "Joining threads..." << std::endl;
		for (int i = 0; i < threads.size(); ++i)
		{
			threads[i].std::thread::join();
		}
		threads.erase(threads.begin(), threads.end());
		std::cout << "Threads joined" << std::endl;
		

		for (int i = 0; i < shot_list[0].size(); ++i) // cycle through clusters
		{
			std::cout << "Cluster: " << i+1 << std::endl;
			for (int j = 0; j < shot_list[0][i].size(); ++j)
			{
				std::cout << "Hit: " << j+1 << std::endl;
				std::cout <<"t = " << shot_list[0][i][j][0] << " x = " << shot_list[0][i][j][1] << " y = " << shot_list[0][i][j][2] << std::endl;
			}
			std::cout << std::endl;
		}

		char ending;
		std::cin >> ending;
		return 0;
}

void print(std::vector<std::vector<std::vector<std::vector<int> > > >* shot_list)
{
	std::cout << "This function works." << std::endl;
}

void build_cluster(std::vector<std::vector<std::vector<int> > >* shot_loc, std::vector<std::vector<int> > list_of_hits)
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

void order_clusters(std::vector<std::vector<std::vector<int> > >* shot_loc)
{
	//std::cout << cluster->size() << std::endl;
	std::vector<std::vector<std::vector<int> > > cluster_loc;
	cluster_loc = *shot_loc;
	for (int i = 0; i < cluster_loc.size(); ++i)
	{
		if (cluster_loc[i].size() > 1)
		{
		sort(cluster_loc[i].begin(), cluster_loc[i].end());
		}
	}
	*shot_loc = cluster_loc;
}

/*void Centroid_image(vector<vector<int> > &cluster, int shot, int cluster_no)
{
	vector<float> topx, topy, bottom;

	for (int i - 0; i < cluster.size(); ++i)
	{
		int weight = cluster[i][0] - cluster[0][0] + 1;
		float NRiWix = (cluster.size() * cluster[i][1]) / weight;
		topx.push_back(NRiWix);
		float NRiWiy = (cluster.size() * cluster[i][2]) / weight;
		topy.push_back(NRiWiy);
		float NWi = cluster.size() / weight;
		bottom.push_back(NWi);
	}

	float sigmax = 0;
	float sigmay = 0;
	float sigmabottom = 0;

	sigmax = sum_vector(topx);
	sigmay = sum_vector(topy);
	sigmabottom = sum_vector(bottom);

	float cx = sigmax / sigmabottom;
	float cy = sigmay / sigmabottom;

	if (megacentroid = 1)
	{
		cx = cx * 2;
		cy = cy * 2;	
	}

	int intcx = round(cx);
	int intcy = round(cy);


}*/
