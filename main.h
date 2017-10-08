/*
 * main.h
 *
 *  Created on: Sep 21, 2017
 *      Author: jasna
 */

#ifndef MAIN_H_
#define MAIN_H_


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <future>

typedef std::vector<std::string> s_vector;
typedef std::vector<int> int_vector;
typedef std::vector<int>::iterator iterator;
typedef std::vector<int_vector> ss_vector;

struct vec_score{
	int score;
	int_vector res_vec;
};

ss_vector parsetxt(std::ifstream& infile);
int_vector treeModelGen(int_vector tree_id);
ss_vector genPartialTree(int N);
int_vector genCompleteTree(int_vector prev, int_vector last);
int_vector children(int_vector model, int node);
vec_score merge(int_vector seq1, int_vector seq2);
bool compareVectorsByRange(int p, int_vector v1, int_vector v2);
vec_score fitchScore(int_vector id, ss_vector set_of_seq);
void scoreTree(ss_vector tree_models, ss_vector set_of_seq, int_vector range);
int_vector lastTree(int N);
void printVector(int_vector vec);

vec_score first, best;
ss_vector set_of_seq;

ss_vector parsetxt(std::ifstream& infile){
	int val;
	ss_vector data;
	while (infile)
		{
		std::string s;
		if (!getline( infile, s )) break;

		std::istringstream ss( s );
		int_vector record;

		while (ss)
		{
		  std::string s;
		  if (!getline( ss, s, ',' )) break;
		  val = std::stoi(s);
		  record.push_back( val );
		}

		data.push_back( record );
		}
	return data;
}

int_vector treeModelGen(int_vector tree_id){
	int_vector tree_model;
	int internal_node = 0;
	int new_el = 0;
	int io, add_to_end;

	tree_model.push_back(1);
	tree_model.push_back(2);
	for (unsigned int i = 2; i < tree_id.size(); i++){
		new_el = i + 1;
		if(tree_id[i]){
			internal_node--;
			io = tree_id[i];
			add_to_end = tree_model[io];
			tree_model[io] = internal_node;
			tree_model.push_back(add_to_end);
			tree_model.push_back(new_el);
		}
	}
	return tree_model;
}


ss_vector genPartialTree(int N){

	ss_vector matrix_id;
	int_vector init_id, last_id, id;
	int tmp;
	int flag = 1;
	unsigned int p = N - 2;
	for(int i = 0; i < N; i++){
		if((i < 2) || (i == N - 1)) {
			init_id.push_back(0);
		}
		else init_id.push_back(1);
	}
	id = init_id;

	for(int k = 0; k < N; k++){
		if(k < 2)
			last_id.push_back(0);
		else if(k == 2)
			last_id.push_back(1);
		else
			last_id.push_back(last_id.at(k-1) + 2);
	}
	last_id.back() = 0;

	while(flag){

		for(int j = 1; j <= last_id.at(N-2); j++){
			id.at(N-2) = j;
			matrix_id.push_back(id);
		}
		if(id == last_id) {
			flag = 0;
			break;
		}
		if(p > 3){
			if(compareVectorsByRange(p, id, last_id)){
				p--;
				tmp = id[p] + 1;
				id.at(p) = tmp;

			}else{
				tmp = id[p] + 1;
				id.at(p) = tmp;
				id.at(N-2) = 0;
			}
		}else{
			tmp = id[p] + 1;
			id.at(p) = tmp;
		}
	}
	return matrix_id;
}


int_vector genCompleteTree(int_vector prev, int_vector last){
	int_vector next_tree = prev;
	int_vector complete_tree;
	if(next_tree.back() != last.back()){
		next_tree.back() = prev.back() + 1;
		complete_tree = next_tree;
	}
	return complete_tree;
}

int_vector children(int_vector model, int node){

	int offset = 2*abs(node);
	int_vector ch;
	ch.push_back(model[offset]);
	ch.push_back(model[(offset + 1)]);

	return ch;
}

vec_score merge(int_vector seq1, int_vector seq2){
	vec_score out;
	out.score = 0;
	for(unsigned int i = 0; i < (unsigned)seq1.size(); i++){
		if((seq1[i] & seq2[i]) > 0){
			out.res_vec.push_back(seq1[i] & seq2[i]);
		}
		else{
			out.res_vec.push_back(seq1[i] | seq2[i]);
			out.score++;
		}
	}
	return out;
}

int_vector lastTree(int N){

	int_vector tree;

	for(int k = 0; k < N; k++){
		if(k < 2)
			tree.push_back(0);
		else if(k == 2)
			tree.push_back(1);
		else
			tree.push_back(tree.at(k-1) + 2);
	}
		return tree;

}
bool compareVectorsByRange(int p, int_vector v1, int_vector v2){

	unsigned int count = 0;

	for(unsigned int i = p; i< v1.size(); i++){
		if(v1[i] == v2[i]) {
			count++;
		}
	}
	if (count == (v1.size() - p)) return true;
	else return false;
}

vec_score fitchScore(int_vector id, ss_vector set_of_seq){

	int_vector model, out_model, int_nodes, temp_model, ch;
	int new_node, flag;
	unsigned int i = 0;
	vec_score score_and_seq, out;
	auto pos = 0;
	iterator iter, node_it;

	model = treeModelGen(id);
	out.score = 0;
	out_model = model;
	new_node = model.back();

	for(unsigned int k = 0; k < model.size(); k++){
		if(model[k] < 0){
			int_nodes.push_back(model[k]);
		}
	}
	std::sort(int_nodes.begin(), int_nodes.end());
	flag = 1;
	i = 0;
	while(flag){
		i = 0;

		while (i < int_nodes.size()){
			temp_model = out_model;
			ch = children(temp_model, int_nodes[i]);
			if((ch[0] > 0) && (ch[1] > 0)){

				score_and_seq = merge(set_of_seq[ch[0]-1], set_of_seq[ch[1]-1]);
				new_node++;
				iter = find (temp_model.begin(), temp_model.end(), int_nodes[i]);
				pos = iter - temp_model.begin();
				temp_model[pos] = new_node;

				if((unsigned)(new_node-1) == set_of_seq.size())
					set_of_seq.push_back(score_and_seq.res_vec);
				else
					set_of_seq[new_node-1] = score_and_seq.res_vec;
				out.score += score_and_seq.score;
				int_nodes.erase(int_nodes.begin() + i);
			}
			out_model = temp_model;
			i++;
		}
		if(int_nodes.size() == 0){
			flag = 0;
		}
	}
	score_and_seq = merge(set_of_seq[0], set_of_seq[new_node - 2]);
	new_node++;
	set_of_seq.push_back(out.res_vec);
	out.score += score_and_seq.score;
	out_model[0] = new_node;
	out.res_vec = out_model;
	return out;
}

void printVector(int_vector vec){
	for(const auto& i: vec) std::cout<<i<<" ";
	std::cout << "\n";
}
void scoreTree(ss_vector tree_models, ss_vector set_of_seq, int_vector range){

	vec_score new_model_score;
	int_vector prev, new_id, id;
	int_vector last = lastTree(set_of_seq.size());

	for(int j = range[0]; j < range[1]; j++){
		id = tree_models[j];
		first = fitchScore(id, set_of_seq);
		prev = id;
		if(first.score < best.score){
			for(int i = 0; i < last.back(); i++){
				new_id = genCompleteTree(prev, last);
				prev = new_id;
				new_model_score = fitchScore(new_id, set_of_seq);
				if(new_model_score.score < best.score){
					best = new_model_score;
				}
			}
		}
	}
}

#endif /* MAIN_H_ */
