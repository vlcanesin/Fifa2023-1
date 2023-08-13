#ifndef heap_lib
#define heap_lib

#include <iostream>
#include <vector>

using namespace std;

typedef struct review_struct{
    int id;
    float review;
}Review;

class HeapMinLimitedSize{
    public:
        // Insert element to keep Heap properties within the range of maxSize
        // If necessary, removes the minimun element
        void push(Review data){
            int i;
            if(ranking.size() == maxSize && ranking[0].review < data.review){
                    ranking[0] = data;
                    heapDown(0);       
            }
            else if(ranking.size() < maxSize){
                ranking.push_back(data);
                heapUp();
            }
        }
        // Remove last element and return it
        Review pop(){
            Review dataReturn = ranking[0];

            swap(ranking[0], ranking[ranking.size() - 1]);

            ranking.pop_back();
            heapDown(0);

            return dataReturn;
        }
        // To be used for future testing - Can be deleted
        void print(){
            int i;
            for(i = 0; i < ranking.size(); i++){
                cout << ranking[i].review << " " << ranking[i].id << endl;
            }
        }

        HeapMinLimitedSize(int maxSize): ranking(), maxSize(maxSize){}

    private:

        vector<Review> ranking;
        int maxSize;

        //Calculate the index of parent and childs in the heap vector
        int leftChild(int parent){
            return 2 * parent + 1;
        }

        int rightChild(int parent){
            return 2 * parent + 2;
        }

        int parent(int son){
            return son / 2 - 1;
        }
        // Heapify all parents nodes
        void heapUp(){
            int i = ranking.size() - 1;
            while(i > 0){
                if(ranking[parent(i)].review > ranking[i].review)
                    swap(ranking[parent(i)], ranking[i]);
               i = parent(i);
            }
        }

        // Heapify the child nodes 
        void heapDown(int i){
            int indexleftChild = leftChild(i);
            int indexrightChild = rightChild(i);
            int minValue = i;

            if(indexleftChild < ranking.size() && ranking[indexleftChild].review < ranking[minValue].review)
                minValue = indexleftChild;
            if(indexrightChild < ranking.size() && ranking[indexrightChild].review < ranking[minValue].review)
                minValue = indexrightChild;
            if(minValue != i){
                swap(ranking[i], ranking[minValue]);
                heapDown(minValue);
            }
        }
};

#endif