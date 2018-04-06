#ifndef NODE
#define NODE

#include <vector>
#include <cassert>

using std::vector;

template <typename T>
class Node
{
public:
	
	Node();
	explicit Node(T data);
	~Node();
	
	void SetData(T data);
	void AddChild(Node<T>* child);
	void AddChildWithData(T data);
	void RemoveChild(int id);
	void RemoveChildren();
	void UnlinkParent();
	Node<T>* GetChild(int id);
	vector<Node<T>*> GetChildren();
	Node<T>* GetParent();
	T& GetData();
	int NumChildren();

private:
	
	Node<T>* parent_;
	T data_;
	vector<Node<T>*> children_;

};

template <typename T>
Node<T>::Node() :
	parent_(nullptr)
{
}

template <typename T>
Node<T>::Node(T data) :
	parent_(nullptr), 
	data_(data)
{
}

template <typename T>
Node<T>::~Node()
{
	for (Node<T>* node : children_)
		delete node;
}

template <typename T>
void Node<T>::SetData(T data)
{
	data_ = data;
}

template <typename T>
void Node<T>::AddChild(Node<T>* child)
{
	assert(child);
	assert(!child->parent_);
	child->parent_ = this;
		
	children_.push_back(child);	
}

template <typename T>
void Node<T>::AddChildWithData(T data)
{
	AddChild(new Node(data));
}

template <typename T>
void Node<T>::RemoveChild(int id)
{
	assert(id >= 0 && id < children_.size());
	children_.erase(children_.begin() + id);
}

template <typename T>
void Node<T>::RemoveChildren()
{
	children_.clear();
}

template <typename T>
void Node<T>::UnlinkParent()
{
	parent_ = nullptr;
}

template <typename T>
Node<T>* Node<T>::GetChild(int id)
{
	assert(id >= 0 && id < children_.size());
	return children_.at(id);
}

template <typename T>
vector<Node<T>*> Node<T>::GetChildren()
{
	return children_;
}

template <typename T>
T& Node<T>::GetData()
{
	return data_;
}

template <typename T>
int Node<T>::NumChildren()
{
	return children_.size();
}

#endif // NODE
