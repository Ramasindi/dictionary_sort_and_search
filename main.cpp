#include <iostream>
#include <fstream>
#include <vector>
#include<algorithm>
#include <cstdlib>
#include <omp.h>

using namespace std;

//Removes unwanted characters from string
string clean_string(string input_string) {
    string output_string="";
    for (unsigned int i=0;i<input_string.length();i++) {
        switch (input_string[i]) {
            case ' '  :
            case '\t'  :
            case '\r'  :
            case '\n'  :
                        break;
            default    :
                        output_string+=input_string[i];
        }
    }
    return output_string;
}

//Load a text file into a vector of strings
vector<string> load_file_into_list(string filename) {
    vector<string> word_list;
    ifstream file_handle(filename.c_str());
    if (file_handle.good()) {
        string word_string;
        while (true) {
            file_handle >> word_string;
            word_string=clean_string(word_string);
            word_list.push_back(word_string);
            if (file_handle.eof())
                break;
        }
    }
    return word_list;
}

//Write a vector of strings to a text file
bool write_list_into_file(string filename,vector<string> word_list) {
    ofstream output_File(filename.c_str());
    if (output_File.is_open()) {
        for(unsigned int w_id=0;w_id<word_list.size();w_id++) {
            output_File << word_list[w_id] << endl;
        }
        output_File.close();
        return true;
    }
    else
        return false;
}

int binary_search(string search_word,const vector<string> &sorted_word_list) {

    //TODO Complete the binary search function, it should return the index of the search word in the sorted word list
    //If the search word is not found, return -1
    int low = 0;
    int high = sorted_word_list.size() - 1;
    int mid = (low + high) / 2;
    while (low <= high) {
        if (sorted_word_list[mid] == search_word) {
            return mid;
        }
        else if (sorted_word_list[mid] < search_word) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
        mid = (low + high) / 2;
    }
    return -1;
}

bool test_if_sorted(const vector<string> &sorted_word_list) {
    for(unsigned int w_id=0;w_id<sorted_word_list.size()-1;w_id++) {
        if(sorted_word_list[w_id].compare(sorted_word_list[w_id+1])>0) {
            return false;
        }
    }
    return true;
}

int main() {
    //Load English Dictionary into Vector of Strings
    vector<string> word_list=load_file_into_list("English_medium_list.txt");

    cout << endl;
    cout << "Bucket sort the words in the dictionary" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    //TODO Sort the words of the English Dictionary using a Bucket Sort algorithm and store the results in sorted_word_list
    vector<string> sorted_word_list;
    int bucket_size = 20;
    vector<vector<string>> buckets(bucket_size);
    for (unsigned int i = 0; i < word_list.size(); i++) {
        int bucket_id = word_list[i].length() % bucket_size;
        buckets[bucket_id].push_back(word_list[i]);
    }
    for (unsigned int i = 0; i < bucket_size; i++) {
        sort(buckets[i].begin(), buckets[i].end());
    }
    for (unsigned int i = 0; i < bucket_size; i++) {
        for (unsigned int j = 0; j < buckets[i].size(); j++) {
            sorted_word_list.push_back(buckets[i][j]);
        }
    }

    //TODO Use OpenMP to parallelise and accelerate the sorting algorithm, display the processing time of the sorting algorithm
    double start_time = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < sorted_word_list.size(); i++) {
        for (int j = i + 1; j < sorted_word_list.size(); j++) {
            if (sorted_word_list[i] > sorted_word_list[j]) {
                string temp = sorted_word_list[i];
                sorted_word_list[i] = sorted_word_list[j];
                sorted_word_list[j] = temp;
            }
        }
    }
    double end_time = omp_get_wtime();
    cout << "Processing time: " << endl;
    cout << end_time - start_time << endl;
    cout << endl;

    //TODO Save sorted word list into the sorted_list.txt file
    write_list_into_file("sorted_list.txt",sorted_word_list);
    cout << endl;
    cout << "Test if Sorting was successful:" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    //TODO re-enable this code when you have completed your sorting algorithm
    if(!test_if_sorted(sorted_word_list))
       cout << "Sorted Successfully!!" << endl;
    else
      cout << "Unsorted!!" << endl;


    cout << endl;
    cout << "Binary search for words:" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    //TODO Perform a Binary Search to find the index of the word "aardwolf"
    int aardwolf_index = binary_search("aardwolf",sorted_word_list);
    cout << "Index of aardwolf: " << aardwolf_index << endl;

    system("pause");
    return 0;
}

