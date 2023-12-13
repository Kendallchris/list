/*
 * Title:		dlinkedlist
 * Purpose:		Definition of the class DoubleLinkedList
 * Author:		Chris Kendall
 * Date:		April 21, 2023
 */


#include "object.h"
#include "dlinkedlist.h"
#include <sstream>

using std::stringstream;

/**
 * Copy Constructor
 * @param other the DoubleLinkedList to be copied
 */
DoubleLinkedList::DoubleLinkedList(const DoubleLinkedList& other): _head(nullptr), _tail(nullptr) {
    Node* current = other._head;
    while (current != nullptr) {
        // traverse through 'other' creating a deep copy
        Node* newNode = new Node;
        // Copy over the data
        newNode->data = current->data;
        newNode->previous = _tail;
        newNode->next = nullptr;

        if (_tail != nullptr) {
            _tail->next = newNode;
        } else {
            _head = newNode;
        }

        _tail = newNode;
        current = current->next;
    }
}

/**
 * Copy Assignment Operator
 * @param rhs DoubleLinkedList to be copied
 * @return 'this' new DoubleLinkedList
 */
DoubleLinkedList &DoubleLinkedList::operator=(const DoubleLinkedList& rhs) {
    // Self-assignment check
    if (this == &rhs) {
        return *this;
    }

    // Delete existing nodes in the list
    Node* current = _head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }

    // Reset head and tail
    _head = nullptr;
    _tail = nullptr;

    // Copy nodes from the other list (rhs)
    current = rhs._head;
    while (current != nullptr) {
        Node* newNode = new Node;
        newNode->data = current->data;
        newNode->previous = _tail;
        newNode->next = nullptr;

        if (_tail != nullptr) {
            _tail->next = newNode;
        } else {
            _head = newNode;
        }

        _tail = newNode;
        current = current->next;
    }

    return *this;
}

/**
 * Default Constructor, sets '_head' and '_tail' to nullptr
 */
DoubleLinkedList::DoubleLinkedList(): _head(nullptr), _tail(nullptr) {

}

/**
 * Destructor, deallocates the dynamically allocated memory.
 */
DoubleLinkedList::~DoubleLinkedList() {
    Node* current = _head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current->data; // Delete the data contained in the node
        delete current; // Delete the node itself
        current = nextNode;
    }
}

/**
 * This method inserts Object* element at 'position' in the list
 * @param element the Object* element to be inserted
 * @param position the position in the list for element to be inserted
 * @return true or false whether successful or not
 */
bool DoubleLinkedList::Insert(Object* element, size_t position) {
    // Make sure the position is within the list boundaries
    if (position > _size) {
        return false;
    }

    Node* newNode = new Node;
    newNode->data = element;
    newNode->next = nullptr;
    newNode->previous = nullptr;

    // If the list is empty
    if (_size == 0) {
        _head = newNode;
    } else if (position == 0) {
        // shift node at position 0 and set newNode as '_head'
        newNode->next = _head;
        _head->previous = newNode;
        _head = newNode;
    } else {
        // traverse to position
        Node* temp = _head;
        for (size_t i = 0; i < position - 1; i++) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        newNode->previous = temp;

        if (temp->next != nullptr) {
            temp->next->previous = newNode;
        }
        temp->next = newNode;
    }
    _size++;
    return true;
}

/**
 * Searches the list for the Object* 'element' and returns the index of the first instance of it
 * @param element the Object* to be searched for
 * @return the position in the list where the 'element' is or -1 if not found
 */
int DoubleLinkedList::IndexOf(const Object* element) const {
    Node* current = _head;
    int position = 0;

    while (current != nullptr) {
        // use respective object's Equals method to compare
        if (current->data->Equals(*element)) {
            return position;
        }

        current = current->next;
        position++;
    }

    // Value not found in the list
    return -1;
}

/**
 * Removes the object at the size_t 'position'. If out of bounds returns nullptr
 * @param position the size_t index of the Object* to be removed
 * @return the pointer to the data of the removed object or nullptr if out of bounds 'position'
 */
Object* DoubleLinkedList::Remove(size_t position) {
    // check if the list is empty (could use list.h IsEmpty() method)
    if (_head == nullptr) {
        return nullptr;
    }

    Node* current = _head;
    size_t currentPos = 0;

    // traverse through the list to the node at 'position'
    while (current != nullptr) {
        if (currentPos == position) {
            // Node found at position, remove it
            // handle case where position is at '_head'
            if (current->previous != nullptr) {
                current->previous->next = current->next;
            } else {
                // Removing the head
                _head = current->next;
            }

            if (current->next != nullptr) {
                current->next->previous = current->previous;
            } else {
                // Removing the tail
                _tail = current->previous;
            }

            Object* removedData = current->data; // Store the removed object's data
            delete current;
            _size--; // Decrement the size
            return removedData; // Return the removed object's data
        }

        current = current->next;
        currentPos++;
    }

    // Position out of bounds, nothing removed
    return nullptr;
}

/**
 * Gets the pointer to the data at the specified position in the list.
 * @param position the size_t index of the desired pointer to the data
 * @return the pointer to the data at 'position' or nullptr if out of bounds
 */
Object* DoubleLinkedList::Get(size_t position) const {
    Node* current = _head;
    size_t currentPosition = 0;

    while (current != nullptr) {
        if (currentPosition == position) {
            // Node found at position, return a pointer to its data
            return current->data;
        }

        current = current->next;
        currentPosition++;
    }

    // 'position' out of bounds, return nullptr
    return nullptr;
}

/**
 * This method will be overritten by the objects' respective class method. Nevertheless it will return a String
 * representation of the data which th object*s point to.
 * @return a String representation of the data which th object*s point to
 */
string DoubleLinkedList::ToString() const {
    stringstream ss;
    Node* current = _head;

    ss << "{";

    while (current != nullptr) {
        ss << current->data->ToString();

        if (current->next != nullptr) {
            ss << ", ";
        }

        current = current->next;
    }

    ss << "}";

    return ss.str();
}

/**
 * This method traverses through the list, freeing all memory and resetting the list to its default values
 * of _head = nullptr, _tail = nullptr, and _size = 0.
 */
void DoubleLinkedList::Clear() {
    Node* current = _head;

    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current->data; // Delete the data stored in the node
        delete current; // Delete the node itself
        current = nextNode;
    }

    _head = nullptr;
    _tail = nullptr;
    _size = 0;
}
