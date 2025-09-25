#pragma once
#include <mutex>

struct Node
{
	int value;
	Node* next;
	std::mutex* node_mutex; // ћьютекс дл€ защиты доступа к данному узлу

	Node(int value);
	~Node();
};

class FineGrainedQueue //  ласс очереди с мелкогранул€рной блокировкой. –еализует односв€зный список с защитой от состо€ний гонки при параллельном доступе
{
	private:
		Node* head;
		std::mutex* queue_mutex; // ћьютекс дл€ защиты доступа к голове списка
	public:
		FineGrainedQueue();
		~FineGrainedQueue();
		void initialize(int value);
		void insertIntoMiddle(int value, int pos);
		void printList();
};


