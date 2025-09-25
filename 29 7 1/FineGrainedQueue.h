#pragma once
#include <mutex>

struct Node
{
	int value;
	Node* next;
	std::mutex* node_mutex; // ������� ��� ������ ������� � ������� ����

	Node(int value);
	~Node();
};

class FineGrainedQueue // ����� ������� � ���������������� �����������. ��������� ����������� ������ � ������� �� ��������� ����� ��� ������������ �������
{
	private:
		Node* head;
		std::mutex* queue_mutex; // ������� ��� ������ ������� � ������ ������
	public:
		FineGrainedQueue();
		~FineGrainedQueue();
		void initialize(int value);
		void insertIntoMiddle(int value, int pos);
		void printList();
};


