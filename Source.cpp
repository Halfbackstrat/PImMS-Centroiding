// Working as of 26/02/2016

#include <vector> 
#include <iostream>
#include <thread>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>

// Function Declarations
void Build_Clusters(std::vector<std::vector<std::vector<int> > >* shot_loc, std::vector<std::vector<int> > list);
void Order_Clusters(std::vector<std::vector<std::vector<int> > >* shot_loc);
void Centroid_Image(std::vector<std::vector<int> >* cluster, int shot, int hit_position, std::vector<int>* out_address);
float Sum_Vector(std::vector<float> vec);
float Average_Vector(std::vector<int> vec);

// Vector Declarations
std::vector<std::vector<int> > input_holder;
std::vector<std::vector<std::vector<std::vector<int> > > > shot_list;
std::vector<std::vector<std::vector<int> > > cluster_list;
std::vector<std::vector<int> > hit_list;
std::vector<int> hit(4);
std::vector<std::vector<std::vector<int> > > final_image;
std::vector<int> cluster_sizes; // Cluster sizes for averaging

// Thread Declaration
std::vector<std::thread> threads;

// Integer Declaration
int x, y, t, shot, bin, max_shot;

// Integer Definitions
int shot_count = 1;
int thread_count = 0;
int event_counter = 0;
int centroided_events = 0; 

// Boolean Definitions
bool megacentroid = false;

int main(int argc, char* argv[])
{   
    if (argc < 2) // Checks to make sure some value for the input file is provided (scripting error handling)
    {
        return 0;
    }
    if (argc > 2)
    {
        megacentroid = true; 
    } 
    
    std::stringstream ss;
    ss << argv[1];
    std::string inputstring;
    std::string outputstring;
    std::string logstring;
    std::string outy = ".out";
    std::string logy = ".log";
    ss >> inputstring;
    outputstring = inputstring + outy;
    logstring = inputstring + logy;

    std::ifstream InputFile; // Gathers the number of shots from the file in order to reserve vector sizes.
    InputFile.open(inputstring);
    if (InputFile.is_open())
    {
        while(InputFile >> x >> y >> t >> shot >> bin)
        {max_shot = shot;}
    }
    InputFile.close();
    
   shot_list.resize(max_shot);
   final_image.resize(max_shot);

    InputFile.open(inputstring);
    if (InputFile.is_open())
    {
        while (InputFile >> x >> y >> t >> shot >> bin)
        {
	    ++event_counter;
            if (shot_count!= shot)
            {
                shot_list.push_back(cluster_list);
                //shot_list[shot - 2].reserve(input_holder.size());
                if (thread_count == 50)
                {
                    for (int i = 0; i < threads.size(); ++i)
                    {threads[i].join();}
                    threads.erase(threads.begin(), threads.end());
                    thread_count = 0;
                }
              
                threads.push_back(std::thread(Build_Clusters, &shot_list[shot - 2], input_holder));
                input_holder.erase(input_holder.begin(), input_holder.end());
                ++thread_count;
                ++shot_count;
               //goto out;
               
            }
            input_holder.push_back(hit);
            input_holder[input_holder.size() - 1][0] = t;
            input_holder[input_holder.size() - 1][1] = x;
            input_holder[input_holder.size() - 1][2] = y;
            input_holder[input_holder.size() - 1][3] = bin;
        }
    }
    else
    {
        std::cout << "Input file not found" << std::endl;
        return 0;
    }
    
    threads.push_back(std::thread(Build_Clusters, &shot_list[shot - 2], input_holder)); // Mop up the final shot (Must be a more elegant way to do this??)
    out:
    for (int i = 0; i < threads.size(); ++i)
    {
        threads[i].join();
    }
    threads.erase(threads.begin(), threads.end());
    
    for (int i = 0; i < shot_list.size(); ++i)
    {
        for (int j = 0; j < shot_list[i].size(); ++j)
        {
            cluster_sizes.push_back(shot_list[i][j].size());
        }
    }
    
    float average = Average_Vector(cluster_sizes);  
   
    //std::cout << "Clusters built" << std::endl;
////////Clusters built - Centroid the data

    //Order the clusters by time
    for (int i = 0; i < shot_list.size(); ++i)
    {
        threads.push_back(std::thread(Order_Clusters, & shot_list[i]));
    }
    // std::cout << "shot size = " << shot_list.size() << " shot[0] size = " << shot_list[0].size() << " Shot[0][0] size = " << shot_list[0][0].size() << std::endl;

    for (int i = 0; i < threads.size(); ++i)
    {
        threads[i].join();
    }
    threads.erase(threads.begin(), threads.end());
    // Centroid the clusters
    for (int i = 0; i < shot_list.size(); ++i)
    {
        final_image[i].reserve(shot_list[i].size());
        for(int j = 0; j < shot_list[i].size(); ++j)
        {
            
            final_image[i].push_back(hit);
            int hit_position = final_image[i].size() -1;
            if (shot_list[i][j].size() > 1)
            {
                threads.push_back(std::thread(Centroid_Image, &shot_list[i][j], i, hit_position, &final_image[i][hit_position]));
            }
            else
            {
                final_image[i][hit_position][0] = shot_list[i][j][0][1];
                final_image[i][hit_position][1] = shot_list[i][j][0][2];
                final_image[i][hit_position][2] = shot_list[i][j][0][0];
                final_image[i][hit_position][3] = i + 1;
            }
        }
   
        for (int j = 0; j < threads.size(); ++j)
        {
            threads[j].join();
        }
        threads.erase(threads.begin(), threads.end());
    }
    
    for (int i = 0; i < final_image.size(); ++i)
	{
	    for (int j = 0; j < final_image[i].size(); ++j)
		{
		++centroided_events;
		} 
	} 
    // Write the data to a new file 
    
    std::ofstream OutputFile(outputstring);
    for (int i = 0; i < final_image.size(); ++i)
    {
        for (int j = 0; j < final_image[i].size(); ++j)
        {
            OutputFile << final_image[i][j][0] << "\t" << final_image[i][j][1] << "\t" << final_image[i][j][2] << "\t" << final_image[i][j][3] << std::endl;
        }
    }
    OutputFile.close();

    std::ofstream LogFile(logstring); // Write the log file containing information about the data analysis.
    LogFile << "Number of recorded events: " << event_counter << std::endl;
    LogFile << "Number of centroided events: " << centroided_events << std::endl;
    LogFile << "The average cluster size was: " << average << std::endl;
    LogFile.close(); 
    
    return 0;
}




void Build_Clusters(std::vector<std::vector<std::vector<int> > >* shot_loc, std::vector<std::vector<int> > list)
{
std::vector<std::vector<std::vector<int> > > clusters;
int cluster_counter = 0;

while (list.size() > 0)
{
    
    clusters.push_back(hit_list);
    clusters[clusters.size() - 1].push_back(hit);
    clusters[clusters.size() - 1][0][0] = list[0][0];
    clusters[clusters.size() - 1][0][1] = list[0][1];
    clusters[clusters.size() - 1][0][2] = list[0][2];
    clusters[clusters.size() - 1][0][3] = list[0][3];
    
    for (int i = cluster_counter; i < clusters.size(); ++i)
    {
        for (int j = 0; j < clusters[i].size(); ++j)
        {
            for (int k = 0; k < list.size(); ++k)
            {
                if (list[k][0] >= clusters[i][j][0] - 1 && list[k][0] <= clusters[i][j][0] + 1 && list[k][1] >= clusters[i][j][1] - 1 && list[k][1] <= clusters[i][j][1] + 1 && list[k][2] >= clusters[i][j][2] - 1 && list[k][2] <= clusters[i][j][2] + 1)
                {
                    
                    clusters[i].push_back(hit);
                    clusters[i][clusters[i].size() - 1][0] = list[k][0];
                    clusters[i][clusters[i].size() - 1][1] = list[k][1];
                    clusters[i][clusters[i].size() - 1][2] = list[k][2];
                    clusters[i][clusters[i].size() - 1][3] = list[k][3];
                    list.erase(list.begin() + k);
                    --k;
                }
            }
        }
        cluster_counter = i;
    }
}
//std::cout << "Shot built." << std::endl; 
*shot_loc = clusters;
//std::cout << "Debug 2" << std::endl;
}

void Order_Clusters(std::vector<std::vector<std::vector<int> > >* shot_loc)
{
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

void Centroid_Image(std::vector<std::vector<int> >* cluster, int shot, int hit_position, std::vector<int>* out_address)
{
    std::vector<float> topx, topy, bottom;
    std::vector<std::vector<int> > cluster_loc = *cluster;
    
    for (int i = 0; i < cluster_loc. size(); ++i)
    {
        float weight = cluster_loc[i][0] - cluster_loc[0][0] + 1;
        float NRiWix = (cluster_loc.size() * cluster_loc[i][1]) / weight;
        topx.push_back(NRiWix);
        float NRiWiy = (cluster_loc.size() * cluster_loc[i][2]) / weight;
        topy.push_back(NRiWiy);
        float NWi = cluster_loc.size() / weight;
        bottom.push_back(NWi);
    }
    float sigmax = 0;
    float sigmay = 0;
    float sigmabottom = 0;
    
    sigmax = Sum_Vector(topx);
    sigmay = Sum_Vector(topy);
    sigmabottom = Sum_Vector(bottom);
    
    float cx = sigmax / sigmabottom;
    float cy = sigmay / sigmabottom;
    
    if (megacentroid = true)
    {
        cx = cx * 2;
        cy = cy * 2;
    }
    
    
    int intcx = round(cx);
    int intcy = round(cy);
    
    std::vector<int> output(4);
    output[0] = intcx;
    output[1] = intcy;
    output[2] = cluster_loc[0][0];
    output[3] = shot + 1;
    *out_address = output;
}

float Average_Vector(std::vector<int> vec)
{
    float average;
    int av_bottom = vec.size();
    int av_top = 0;
    for (int i = 0; i < vec.size(); ++i)
    {
        av_top = av_top + vec[i];
    }
    average = float(av_top) / float(av_bottom);
    return average;
}

float Sum_Vector(std::vector<float> vec)
{
    float sigma = 0;
    for (int i = 0; i < vec.size(); ++i)
    {
        sigma = sigma + vec[i];
    }
    return sigma;
}
