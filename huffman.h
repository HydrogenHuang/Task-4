const int oo=512;

#ifndef _HUFFMAN_H_
#define  _HUFFMAN_H_

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <map>

using namespace std;

struct huff_node{
	char letter;
	unsigned int weight;
	bool parent;
	int lchild,rchild;
	
	huff_node();
	huff_node(char,int);
	bool isleaf();
	bool operator > (const huff_node&);
	void clear();
};

class huffman_tree{
	public:
		huffman_tree();
		void encode(string sourcefile,string targetfile);
		void decode(string sourcefile,string targetfile);
		~huffman_tree();
	private:
		void precode(FILE* infile); //calculate the weight of each letters in the source file.
		void buildtree(); //build the huffman tree.
		void sort(); //sort the leaf at the begining.
		void update(); //update the tree when a parent node is added.
		void coding(int loc,string code); //code each leaf
		void compact(FILE* infile,FILE* outfile);
		string uits(long f,int len);//tansfer the unsigned int into binary
		void canceal();//clear all the data
		
		unsigned long source_length;//The length of the source file.
		unsigned long target_length;//The length of the target file.
		huff_node node[oo];
		map<char,string> char_code;
		int node_size; //number of the nodes
		int size;
};

#endif
