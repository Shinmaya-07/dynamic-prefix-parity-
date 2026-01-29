#include <iostream>
#include <cstdlib>
#include <string>
#include <cctype> 
using namespace std;

#define MAX_CHILDREN 3

struct DPP 
{
    int data;
    int numChildren;
    DPP* children[MAX_CHILDREN];

    DPP(int value, int numChildren = 0) : data(value), numChildren(numChildren) 
    {
        for (int i = 0; i < MAX_CHILDREN; i++) 
            children[i] = nullptr;
    }
};

DPP* initialize(int beg, int end) 
{
    int range = end - beg + 1;
    if (range == 1) 
        return new DPP(0);

    int numChildren = min(MAX_CHILDREN, range);
    DPP* node = new DPP(0, numChildren);

    int segmentSize = range / numChildren;
    int extra = range % numChildren;
    int start = beg;

    for (int i = 0; i < numChildren; i++) 
    {
        int childRange = (i < extra) ? segmentSize + 1 : segmentSize;
        node->children[i] = initialize(start, start + childRange - 1);
        start += childRange;
        node->data ^= node->children[i]->data;
    }
    return node;
}

int prefix_parity(DPP* node, int beg, int end, int index) 
{
    if (!node || index < beg)
        return 0;
    if (index >= end)
        return node->data;

    int range = end - beg + 1;
    int numChildren = node->numChildren;
    int segmentSize = range / numChildren;
    int extra = range % numChildren;
    int start = beg, parity = 0;

    for (int i = 0; i < numChildren; i++) 
    {
        int childRange = (i < extra) ? segmentSize + 1 : segmentSize;
        int endRange = start + childRange - 1;

        if (index >= start) 
            parity ^= prefix_parity(node->children[i], start, endRange, index);

        start += childRange;
    }
    return parity;
}

DPP* flip(DPP* node, int beg, int end, int index, int value) 
{
    if (!node)
        return nullptr;

    if (beg == end) 
    {
        node->data ^= value;
        return node;
    }

    int range = end - beg + 1;
    int numChildren = node->numChildren;
    int segmentSize = range / numChildren;
    int extra = range % numChildren;
    int start = beg;

    for (int i = 0; i < numChildren; i++) 
    {
        int childRange = (i < extra) ? segmentSize + 1 : segmentSize;
        int endRange = start + childRange - 1;

        if (index >= start && index <= endRange) 
            node->children[i] = flip(node->children[i], start, endRange, index, value);

        start += childRange;
    }

    node->data = 0;
    for (int i = 0; i < numChildren; i++) 
        if (node->children[i]) 
            node->data ^= node->children[i]->data;

    return node;
}

int main() 
{
    int n;
    cout << "Enter the number of bits: ";
    cin >> n;

    if (n <= 0) 
    {
        cout << "Number of bits must be positive." << endl;
        return 1;
    }

    DPP* root = initialize(0, n - 1);

    cout << "Initial bit array: ";
    for (int i = 0; i < n; i++)
        cout << "0 ";
    cout << endl;

    string choice;
    do 
    {
        cout << "Do you want to flip a bit? (yes/no): ";
        cin >> choice;

        for (int i = 0; i < choice.length(); i++) 
        {
            choice[i] = tolower(choice[i]);
        }

        if (choice == "yes") 
        {
            int index;
            cout << "Enter the index (0-based) of the bit to flip: ";
            cin >> index;

            if (index < 0 || index >= n) 
                cout << "Invalid index. Please enter a value between 0 and " << n - 1 << "." << endl;
            else 
                flip(root, 0, n - 1, index, 1);
        } 
        else if (choice == "no") 
            break;
        else 
            cout << "Invalid input. Please enter 'yes' or 'no'." << endl;
    } while (true);

    cout << "Prefix parities of sub-array:" << endl;
    for (int i = 0; i < n; i++)
        cout << "Prefix parity up to index " << i + 1 << " is " << prefix_parity(root, 0, n - 1, i) << endl;

    return 0;
}
