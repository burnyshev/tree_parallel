#include "Node.h"
#include <iostream>
#include <queue>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

using std::queue;
using std::cout;
using std::endl;

const int NODE_DEPTH = 23;
const int NUM_THREADS = 4;

void GenerateNode(int depth, Node<int>* parent);
int SimpleGetNodeCount(Node<int>* root);
int ThreadGetNodeCount(Node<int>* root);
int StartThreads(queue<Node<int>*> nodes);
void* ThreadFunc(void* data);

struct ThreadStartInfo
{
	Node<int>* node;
	int n;
};

int main()
{
	Node<int>* root = new Node<int>(0);
	GenerateNode(0, root);

	cout << "For binary tree with node depth " << NODE_DEPTH << " node count is " << ((1 << (NODE_DEPTH + 1)) - 1) << endl;

	struct timeval tv;
	gettimeofday(&tv, NULL);

	int n = SimpleGetNodeCount(root);

	struct timeval tv2;
	gettimeofday(&tv2, NULL);

	int n2 = ThreadGetNodeCount(root);

	struct timeval tv3;
	gettimeofday(&tv3, NULL);

	double time1 = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000.0;
	double time2 = (tv2.tv_sec) * 1000 + (tv2.tv_usec) / 1000.0;
	double time3 = (tv3.tv_sec) * 1000 + (tv3.tv_usec) / 1000.0;
	cout << "Simple algorithm result: " << n  << " (" << (time2 - time1) << " ms)" << endl;
	cout << "Thread algorithm result: " << n2 << " (" << (time3 - time2) << " ms)" << endl;
	
	delete root;

	return 0;
}

void GenerateNode(int depth, Node<int>* parent)
{
	if (depth >= NODE_DEPTH)
		return;
	
	parent->AddChildWithData(depth);
	parent->AddChildWithData(1000 + depth);

	for (Node<int>* node : parent->GetChildren())
		GenerateNode(depth + 1, node);
}

int SimpleGetNodeCount(Node<int>* root)
{
	int n = 1;
	for (Node<int>* node : root->GetChildren())
		n += SimpleGetNodeCount(node);
	
	return n;
}

int ThreadGetNodeCount(Node<int>* root)
{
	queue<Node<int>*> nodes;
	
	nodes.push(root);
	int n = 0;
	
	while (!nodes.empty())
	{
		if (nodes.size() >= NUM_THREADS)
			return n + StartThreads(nodes);
		
		Node<int>* node = nodes.front();
		nodes.pop();
		n++;

		for (Node<int>* child : node->GetChildren())
			nodes.push(child);
	}
	
	return n;
}

int StartThreads(queue<Node<int>*> nodes)
{
	int n = 0;
	
	int numThreads = nodes.size();
	pthread_t* threads = new pthread_t[numThreads];
	ThreadStartInfo* threadInfo = new ThreadStartInfo[numThreads];
	for (int i = 0; !nodes.empty(); i++)
	{
		threadInfo[i].node = nodes.front();
		threadInfo[i].n = 0;
		nodes.pop();

		pthread_create(&threads[i], 0, &ThreadFunc, (void*) &threadInfo[i]);
	}
	
	for (int i = 0; i < numThreads; i++)
		pthread_join(threads[i], nullptr);
	
	for (int i = 0; i < numThreads; i++)
		n += threadInfo[i].n;
	
	delete[] threads;
	delete[] threadInfo;
	
	return n;
}

void* ThreadFunc(void* data)
{
	ThreadStartInfo* threadInfo = (ThreadStartInfo*) data;
	threadInfo->n = SimpleGetNodeCount(threadInfo->node);
	return nullptr;
}
