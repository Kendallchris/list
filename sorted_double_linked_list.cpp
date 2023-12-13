/*
 * Title:		SortedDoubleLinkedList
 * Purpose:		A double linked list that keeps its elements sorted at all times.
 * Author:		Chris Kendall
 * Date:		April 21, 2023
 */

#include "sorted_double_linked_list.h"
#include "dlinkedlist.h"


/**
 * This method does nothing.
 */
SortedDoubleLinkedList::SortedDoubleLinkedList() = default;

/**
 * This method assumes the list is already sorted and as such traverses through it comparing its contents to
 * insert the Comparable* 'element' into the proper position.
 * @param element Comparable* element to be inserted into the list
 * @return true or false whether or not the insert was successful or not. It should never return false
 */
bool SortedDoubleLinkedList::Insert(Comparable* element) {
    Node* newNode = new Node{element, nullptr, nullptr};

    if (_head == nullptr) {
        // If the list is empty, insert the new node as the head and tail
        _head = newNode;
        _tail = newNode;
    } else if (static_cast<Comparable*>(_head->data)->CompareTo(element) >= 0) {
        // Insert the new node at the beginning of the list if it's less than or equal to the head
        newNode->next = _head;
        _head->previous = newNode;
        _head = newNode;
    } else {
        // Find the appropriate position to insert the new node
        Node* current = _head;
        while (current->next != nullptr && dynamic_cast<Comparable*>(current->next->data)->CompareTo(element) < 0) {
            current = current->next;
        }

        // Insert the new node after the current node
        newNode->next = current->next;
        newNode->previous = current;
        if (current->next != nullptr) {
            current->next->previous = newNode;
        } else {
            // If the new node is inserted at the end of the list, update the tail
            _tail = newNode;
        }
        current->next = newNode;
    }

    return true;
}
