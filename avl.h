
#ifndef AVL_H
#define AVL_H
#include <iostream>
#include <iomanip>

/*forward declaration*/
template <class T>
class Queue;

template <typename T>
class AVL{

	struct Node{
		T data_;
		Node* left_;
		Node* right_;
		int height_;
		Node(const T& data=T{},Node* left=nullptr, Node* right=nullptr){
			data_=data;
			left_=left;
			right_=right;
			height_=0;
		}
		void updateHeight(){
			int heightRight=(right_)?right_->height_:-1;
			int heightLeft=(left_)?left_->height_:-1;-
			height_=(heightRight>heightLeft)?heightRight+1:heightLeft+1;
		}
	};
	//support functions so that we don't have to keep checking for nullptrs
	int height(const Node* subtreeroot)const{
		return (subtreeroot)?subtreeroot->height_:-1;
	}
	int balanceFactor(const Node* subtreeroot)const{
		return height(subtreeroot->right_)-height(subtreeroot->left_);
	}

	Node* root_;

	/* Left Rotate
	    Goes from    ---->    to

		Aptr->	 A           Aptr->B
		    	/ \               / \
			   x   B	         A   z
			      / \           / \
			     y   z         x   y

	*/
	void leftRotate(Node*& Aptr){
		Node* Bptr=Aptr->right_;
		Node* Yptr=Bptr->left_;
		Bptr->left_=Aptr;
		Aptr->right_=Yptr;
		Aptr=Bptr;
	}
	/* Right Rotate
	    Goes from    ---->    to

		Aptr->	 A           Aptr->B
		    	/ \               / \
			   B   z	         x   A
			  / \                   / \
			 x   y                 y   z

	*/	
	void rightRotate(Node*& Aptr){
		Node* Bptr=Aptr->left_;
		Node* Yptr=Bptr->right_;
		Bptr->right_=Aptr;
		Aptr->left_=Yptr;
		Aptr=Bptr;
	}
	void insert(Node*& curr,const T& value){
		if(curr==nullptr){
			curr=new Node(value);
		}
		else{
			if(value < curr->data_){
				insert(curr->left_,value);
			}
			else{
				insert(curr->right_,value);
			}
			int bf=balanceFactor(curr);
			if(bf >= 2){
				//rotation needed, left side too tall
				if(balanceFactor(curr->right_)==-1){
					//bf signs are different, double rotation needed
					rightRotate(curr->right_);
					leftRotate(curr);
					curr->left_->updateHeight();
					curr->right_->updateHeight();
				}
				else{
					leftRotate(curr);
					curr->left_->updateHeight();
				}
			}
			//rotation needed, right side too tall
			else if(bf <= -2){
				//rotation needed, left side too tall
				if(balanceFactor(curr->left_)==1){
					//bf signs are different, double rotation needed
					leftRotate(curr->left_);
					rightRotate(curr);
					curr->left_->updateHeight();
					curr->right_->updateHeight();
				}
				else{
					rightRotate(curr);
					curr->right_->updateHeight();
				}
			}
			curr->updateHeight();
		}
	}
	//subtree is the root of the subtree I wish to print
	//inorder
	void printTreeInorder(const Node* subtree) const{
		if(subtree!=nullptr){
			printTreeInorder(subtree->left_);
			std::cout << subtree->data_ << " - ";
			printTreeInorder(subtree->right_);
		}
	}
	void printTreePreorder(const Node* subtree) const{
		if(subtree){
			std::cout << subtree->data_ << " - ";
			printTreePreorder(subtree->left_);
			printTreePreorder(subtree->right_);
		}
	}
	bool exists(const Node* subtree, const T& value) const{
		if(subtree){
			if(value == subtree->data_){
				return true;
			}
			else if(value < subtree->data_){
				return exists(subtree->left_,value);
			}
			else{
				return exists(subtree->right_,value);
			}
		}
		return false;
	}
	//this function detaches and returns the smallest node
	//with root subtree.  Note, it does not delete it, just 
	//detach
	Node* detachMin(Node*& subtree){
		if(subtree->left_){
			return detachMin(subtree->left_);
		}
		else{
			//hold onto the current node, it is the smallest
			Node* rc=subtree;
			//make the pointer to that node point to nodes right
			//subtree which may or may not exist
			subtree=subtree->right_;
			return rc;
		}
	}
	void remove(Node*& subtree,const T& value){
		if(subtree){
			if(value == subtree->data_){
				//point to the node to be removed so we don't 
				//lose it by accident
				Node* rm=subtree;   
				//if subtree has no children
				if(!subtree->left_ && !subtree->right_){
					subtree=0;
				}
				//if there is only a left subtree
				else if(subtree->left_ && !subtree->right_){
					subtree=subtree->left_;
				}
				//if there is only a right subtree
				else if(subtree->right_ && !subtree->left_){
					subtree=subtree->right_;
				}
				//two children
				else{
					Node* successor = detachMin(subtree->right_);
					successor->left_=subtree->left_;
					successor->right_=subtree->right_;
					subtree=successor;
				}

				delete rm;
			}
			else if(value < subtree->data_){
				remove(subtree->left_,value);
			}
			else{
				remove(subtree->right_,value);
			}			
		}
	}
	void destroy(Node* subtree){
		if(subtree){
			destroy(subtree->left_);
			destroy(subtree->right_);
			delete subtree;
		}
	}


public:
	//create empty AVL
	AVL(){
		root_=nullptr;
	}

	void insert(const T& value){
		insert(root_,value);

	}

	//returns true if a match value exists
	//in the AVL, false otherwise
	bool exists(const T& value) const{
		return exists(root_,value);
	}

    //removes a node with value from tree
	void remove(const T& value){
		remove(root_, value);
	}




	~AVL(){
		destroy(root_);
	}

	//this is essentially a breadth first print function
	//However, it also draws the tree structure in
	//ascii art...code is pretty ugly  to support
	//the formatting, don't worry about
	//figuring out how this works... its only to support 
	//good looking output
	void printPretty() const{
		struct Output{
			Node* node_;
			int lvl_;
			int position_;
			Output(Node* n=nullptr,int l=0, int p=0){
				node_=n;
				lvl_=l;
				position_=p;
			}
			void set(Node* n=nullptr,int l=0, int p=0){
				node_=n;
				lvl_=l;
				position_=p;
			}
		};
		Queue<Output> theNodes;
		Node* line[16];
		if(root_){
			for(int i=0;i<16;i++){
				line[i]=nullptr;
			}
			theNodes.enqueue(Output(root_,0,0));
			int currline=0;
			int width=80;
			int numInLine=1;
			while(theNodes.isEmpty()==false){
				Output curr=theNodes.front();
				if(curr.node_->left_){
					theNodes.enqueue(Output(curr.node_->left_,curr.lvl_+1,curr.position_*2));
				}
				if(curr.node_->right_){
					theNodes.enqueue(Output(curr.node_->right_,curr.lvl_+1,curr.position_*2+1));
				}
				theNodes.dequeue();


				if(curr.lvl_>currline){
					printLine(line,numInLine,width);
					width=width/2;
					numInLine=numInLine*2;
					for(int i=0;i<16;i++){
						line[i]=nullptr;
					}
					currline++;

				}
				line[curr.position_]=curr.node_;
			}
			printLine(line,numInLine,width);
			std::cout << std::endl;

		}
		else{
			std::cout << "tree is empty" << std::endl;
		}

	}
private:
	/*used by printPretty() to print all nodes at same level*/
	void printLine(Node* data[],int numNodes,int width) const{
		int half=width/2;
		int firsthalf=width%2?half+1:half;

		if(numNodes > 1){
			for(int i=0;i<numNodes;i++){
				if(i%2==0){
					if(data[i]){
						std::cout << std::right <<std::setfill(' ') << std::setw(firsthalf)<< "-";
						std::cout << std::left << std::setfill('-') << std::setw(half) << "-";
					}
					else{
						std::cout << std::right <<std::setfill(' ') << std::setw(firsthalf)<< " ";
						std::cout << std::left << std::setfill(' ') << std::setw(half) << " ";
					}
				}
				else{
					if(data[i]){
						std::cout << std::right << std::setfill('-') << std::setw(firsthalf) << "-";
						std::cout << std::left <<std::setfill(' ') << std::setw(half)<<"-";
					}
					else{
						std::cout << std::right << std::setfill(' ') << std::setw(firsthalf) << " ";
						std::cout << std::left <<std::setfill(' ') << std::setw(half)<<" ";
					}
				}
			}
			std::cout << std::endl;
		}
		for(int i=0;i<numNodes;i++){
			if(data[i]){
				if(i%2==0){
					std::cout << std::right << std::setw(firsthalf) << "|";
					std::cout << std::left <<std::setfill(' ') << std::setw(half)<<" ";
				}
				else{
					std::cout << std::right << std::setfill(' ') << std::setw(firsthalf) << " ";
					std::cout << std::left <<std::setfill(' ') << std::setw(half)<< "|";				
				}
			}
			else{
				std::cout << std::left <<std::setfill(' ') << std::setw(width)<<" ";			
			}
		}
		std::cout << std::endl;		
		for(int i=0;i<numNodes;i++){
			if(data[i]){
				if(i%2==0){
					std::cout << std::right << std::setw(firsthalf) << data[i]->data_;
					std::cout << std::left <<std::setfill(' ') << std::setw(half)<<" ";
				}
				else{
					std::cout << std::right << std::setfill(' ') << std::setw(firsthalf) << " ";
					std::cout << std::left <<std::setfill(' ') << std::setw(half)<< data[i]->data_;				
				}
			}
			else{
				std::cout << std::left <<std::setfill(' ') << std::setw(width)<<" ";			
			}
		}
		std::cout << std::endl;

	}

};
template <class T>
class Queue{
	T* theQueue_;
	int capacity_;
	int used_;
	int front_;
	int back_;
	void grow(){
		T* tmp=new T[capacity_*2];
		int j;
		for(int i=0,j=front_;i<used_;i++,j=(j+1)%capacity_){
			tmp[i]=theQueue_[j];
		}
		delete [] theQueue_;
		theQueue_=tmp;
		capacity_=capacity_*2;
		front_=0;
		back_=used_;
	}
public:
	Queue(){
		theQueue_=new T[50];
		capacity_=50;
		used_=0;
		front_=0;
		back_=0;
	}
	void enqueue(const T& data){
		if(used_==capacity_){
			grow();
		}
		theQueue_[back_]=data;
		back_=(back_+1)%capacity_;
		used_++;
	}
	void dequeue(){
		if(!isEmpty()){
			used_--;
			front_=(front_+1)%capacity_;
		}
	}
	T front() const{
		if(!isEmpty()){
			return theQueue_[front_];
		}
		return T{};

	}
	bool isEmpty() const{
		return used_==0;
	}
	~Queue(){
		delete [] theQueue_;
	}	
};
#endif