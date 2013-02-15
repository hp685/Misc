/****************************************************************************
                                     Trie
                 INPUT FORMAT :
                                   INTEGER N
                                   N strings
*****************************************************************************/
#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
using namespace std;



struct TrieNode{
	int depth;
	char value;
	vector<TrieNode> children;
	TrieNode(int depth, char value):depth(depth), value(value){};
};


class Trie{

	TrieNode* root;
public:
	Trie(){
		root = new TrieNode(0,'#');
	}
	~Trie(){};
	TrieNode* getRoot(){return root;}
	void addString(const string&);
	void ShowTrie(TrieNode&);


};



void Trie::addString(const string& s){

	TrieNode* current = getRoot();

	for(int i = 0; i < s.length(); i++){
		if(current->children.size()){ //child exists
			bool found = false;

			for(int j = 0; j < current->children.size(); j++){
				if(current->children[j].value == s[i]){
					found = true;
					current = &current->children[j];
					continue;
				}
			}
			if(!found){
				TrieNode* node = new TrieNode((current->depth) + 1, s[i]);
				current->children.push_back(*node);
				current = &current->children[current->children.size() - 1];
			}

		}else{ //make a new child
			TrieNode* node = new TrieNode((current->depth) + 1, s[i]);
			current->children.push_back(*node);
			current = &current->children[0];
		}
	}
}


void Trie::ShowTrie(TrieNode& current){


	cout<<current.value;
	while(current.children.size()){

		for(int i = 0; i < current.children.size(); i++){
			ShowTrie(current.children[i]);
		}
		current = current.children[0];

		//current = &current->children[0];
		cout<<endl;
	}

}

int main(){

	Trie words;
	int N;
	string S;
	cin>>N;
        while(N--){
		cin>>S;
		words.addString(S);
	}

	TrieNode* current = words.getRoot();
	words.ShowTrie(*current);

}
