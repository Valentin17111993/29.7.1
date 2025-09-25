#include <iostream>
#include "FineGrainedQueue.h"

// Реализация методов структуры Node
Node::Node(int val) : value(val), next(nullptr), node_mutex(new std::mutex()) {}

Node::~Node() 
{
    delete node_mutex;
}

// Реализация методов класса FineGrainedQueue
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
    // Создаем новый узел
    Node* newNode = new Node(value);

    // Блокируем очередь для начального доступа к head
    queue_mutex->lock();

    if (head == nullptr) 
    {
        // Если список пуст, создаем первый элемент
        head = newNode;
        queue_mutex->unlock();
        return;
    }

    if (pos <= 0) 
    {
        // Если pos <= 0, вставляем после первого элемента
        Node* firstNode = head;
        firstNode->node_mutex->lock();
        queue_mutex->unlock(); // Разблокируем очередь, т.к. уже захватили first

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
            // Если есть только один элемент в списке
            firstNode->next = newNode;
        }
        firstNode->node_mutex->unlock();
        return;
    }

    // Находим позицию для вставки
    Node* current = head;
    current->node_mutex->lock();
    queue_mutex->unlock(); // Разблокируем очередь, т.к. уже захватили первый узел

    int currentPos = 0;
    Node* prev = nullptr;

    // Идем по списку до нужной позиции или до конца
    while (currentPos < pos && current->next) 
    {
        Node* nextNode = current->next;
        nextNode->node_mutex->lock();

        if (prev) 
        {
            prev->node_mutex->unlock(); // Разблокируем предыдущий узел
        }

        prev = current;
        current = nextNode;
        currentPos++;
    }

    // Вставка после текущего узла
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