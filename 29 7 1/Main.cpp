#include "FineGrainedQueue.h"
#include <iostream>
#include <thread>
#include <vector>

void insertElements(FineGrainedQueue& queue, int startValue, int count, int pos) 
{
    for (int i = 0; i < count; i++) 
    {
        queue.insertIntoMiddle(startValue + i, pos);
    }
}

int main() 
{
    setlocale(LC_ALL, "");

    FineGrainedQueue queue;

    // ������������� ������
    queue.initialize(1);

    // ������� ����
    std::cout << "������� ���� ���������������� �������:" << std::endl;
    queue.insertIntoMiddle(2, 0); // ��������� ����� ������� ��������
    queue.insertIntoMiddle(3, 1); // ��������� ����� ������� ��������
    queue.insertIntoMiddle(4, 10); // ��������� � �����, �.�. ������� ������ ����� ������
    queue.printList();

    // ���� ������������ �������
    std::cout << "\n���� ������������ �������:" << std::endl;
    FineGrainedQueue parallelQueue;
    parallelQueue.initialize(0);

    // ������� ��������� ������� ��� ������������ �������
    std::vector<std::thread> threads;
    int threadsCount = 3;

    for (int i = 0; i < threadsCount; i++) 
    {
        threads.push_back(std::thread(insertElements, std::ref(parallelQueue), i * 10 + 1, 3, i));
    }

    // ���� ���������� ���� �������
    for (auto& t : threads) 
    {
        t.join();
    }

    // ������� ��������� ������������ �������
    parallelQueue.printList();

    return 0;
}