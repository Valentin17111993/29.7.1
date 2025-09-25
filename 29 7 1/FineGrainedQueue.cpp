#include <iostream>
#include "FineGrainedQueue.h"

// ���������� ������� ��������� Node
Node::Node(int val) : value(val), next(nullptr), node_mutex(new std::mutex()) {}

Node::~Node() 
{
    delete node_mutex;
}

// ���������� ������� ������ FineGrainedQueue
FineGrainedQueue::FineGrainedQueue() : head(nullptr), queue_mutex(new std::mutex()) {}

FineGrainedQueue::~FineGrainedQueue() 
{
    Node* current = head;
    while (current != nullptr) 
    {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
    delete queue_mutex;
}

void FineGrainedQueue::initialize(int value) 
{
    std::lock_guard<std::mutex> lock(*queue_mutex);
    if (head == nullptr) 
    {
        head = new Node(value);
    }
}

void FineGrainedQueue::insertIntoMiddle(int value, int pos) 
{
    // ������� ����� ����
    Node* newNode = new Node(value);

    // ��������� ������� ��� ���������� ������� � head
    queue_mutex->lock();

    if (head == nullptr) 
    {
        // ���� ������ ����, ������� ������ �������
        head = newNode;
        queue_mutex->unlock();
        return;
    }

    if (pos <= 0) 
    {
        // ���� pos <= 0, ��������� ����� ������� ��������
        Node* firstNode = head;
        firstNode->node_mutex->lock();
        queue_mutex->unlock(); // ������������ �������, �.�. ��� ��������� first

        Node* secondNode = firstNode->next;
        if (secondNode) 
        {
            secondNode->node_mutex->lock();
            newNode->next = secondNode;
            firstNode->next = newNode;
            secondNode->node_mutex->unlock();
        }
        else 
        {
            // ���� ���� ������ ���� ������� � ������
            firstNode->next = newNode;
        }
        firstNode->node_mutex->unlock();
        return;
    }

    // ������� ������� ��� �������
    Node* current = head;
    current->node_mutex->lock();
    queue_mutex->unlock(); // ������������ �������, �.�. ��� ��������� ������ ����

    int currentPos = 0;
    Node* prev = nullptr;

    // ���� �� ������ �� ������ ������� ��� �� �����
    while (currentPos < pos && current->next) 
    {
        Node* nextNode = current->next;
        nextNode->node_mutex->lock();

        if (prev) 
        {
            prev->node_mutex->unlock(); // ������������ ���������� ����
        }

        prev = current;
        current = nextNode;
        currentPos++;
    }

    // ������� ����� �������� ����
    newNode->next = current->next;
    current->next = newNode;

    current->node_mutex->unlock();
    if (prev) 
    {
        prev->node_mutex->unlock();
    }
}

void FineGrainedQueue::printList() 
{
    std::lock_guard<std::mutex> lock(*queue_mutex);
    Node* current = head;
    while (current) 
    {
        std::cout << current->value << " -> ";
        current = current->next;
    }
    std::cout << "nullptr" << std::endl;
}