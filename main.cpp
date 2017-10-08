/*
 * main.cpp
 *
 *  Created on: Sep 14, 2017
 *      Author: jasna
 */
#include "main.h"
#include <cassert>
#include <chrono>


int main(int argc, char* argv[]){

	int num_of_seq, is_p, num_thread;
	ss_vector tree_models;
	ss_vector set_of_seq;
	std::ifstream infile("longula_seq.txt" );
	set_of_seq = parsetxt(infile);
	int_vector id, comp_vec, new_id, last_id, prev_id;
	vec_score fitch, new_model_score;
	int_vector range;

	num_of_seq = set_of_seq.size();

	is_p = 1;

	//generate all partial trees
	tree_models = genPartialTree(num_of_seq);
	if(tree_models.size()%2 == 0)
		num_thread = 2;
	else
		num_thread = 3;
	//Sequential implementation
	range.push_back(0);
	range.push_back(tree_models.size());
	if(!is_p){
		best.score = 1000;
		auto seq_start = std::chrono::steady_clock::now();
		last_id = lastTree(num_of_seq);
		scoreTree(tree_models, set_of_seq, range);
		auto seq_end = std::chrono::steady_clock::now();

		auto seq_diff = seq_end - seq_start;

		std::cout << "Final best tree with score "<< best.score << "\n";
		printVector(best.res_vec);
		std::cout << "Sequential implementation executed in: ";
		std::cout << std::chrono::duration<double, std::milli> (seq_diff).count()<< "ms" << "\n";
	}else{
	//Parallel implementation
		best.score = 1000;
		int offset = tree_models.size()/num_thread;
		std::vector<std::thread> workers;
		auto start = std::chrono::steady_clock::now();

		for(int i = 0; i < num_thread ; i++){
			range.at(1) = range.at(0) + offset;
			auto t = std::thread(&scoreTree, tree_models, set_of_seq, range);
			workers.push_back(std::move(t));
			range.at(0) +=offset;
		}
		std::for_each(workers.begin(), workers.end(), [](std::thread &t){
				assert(t.joinable());
				t.join();
			});
		auto end = std::chrono::steady_clock::now();

		auto diff = end - start;

		std::cout << "Final best tree with score "<< best.score << "\n";
		printVector(best.res_vec);
		std::cout << "Parallel implementation executed in: ";
		std::cout << std::chrono::duration<double, std::milli> (diff).count()<< "ms" << "\n";
	}
	 return 0;
}


