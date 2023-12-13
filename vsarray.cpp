/*
 * Title:		vsarray
 * Purpose:		Definition of the class vsarray
 * Author:		Chris Kendall
 * Date:		April 21, 2023
 */

#include "object.h"
#include "vsarray.h"
#include <sstream>

using std::stringstream;

/**
 * Copy constructor
 * @param other 'VSArray' to be copied
 */
VSArray::VSArray(const VSArray &other): List(other) {
    _capacity = other._capacity;
    _delta = other._delta;
    _data = new Object*[_capacity];

    // traverse through array using Clone() method to copy data over
    for (size_t i = 0; i < other.Size(); ++i) {
        _data[i] = other._data[i]->Clone();
    }
}

/**
 * Copy assignment operator
 * @param rhs 'VSArray' to be copied
 * @return this to enable cascade assignments
 */
const VSArray &VSArray::operator=(const VSArray &rhs) {
    // self assignment check
    if (this == &rhs) {
        return *this;
    }
    else {
        // use copy constructor to make a deep copy of 'rhs'
        VSArray copy(rhs);
        Object** swapArray;
        size_t swapSize;
        size_t swapCapacity;

        // swap contents with 'copy' so the old contents are automatically freed when method finishes
        swapArray = _data;
        swapSize = _size;
        swapCapacity = _capacity;
        _data = copy._data;
        _capacity = copy._capacity;
        _size = copy._size;
        copy._data = swapArray;
        copy._size = swapSize;
        copy._capacity = swapCapacity;
    }
    return *this;
}

/**
 * Resizes the array by the percentage 'delta'
 */
void VSArray::Resize() {
    size_t newCapacity = static_cast<size_t>(_capacity * (1 + _delta));
    // create new array with 'newCapacity'
    Object** newData = new Object*[newCapacity];

    // bring the data from the old array over
    for (size_t i = 0; i < Size(); ++i) {
        newData[i] = _data[i];
    }

    // free the old array data
    delete[] _data;
    _data = newData;
    _capacity = newCapacity;
}

/**
 * Constructor
 * @param capacity size_t max size of the array
 * @param increasePercentage double to increase capacity by whenever resize is needed
 */
VSArray::VSArray(size_t capacity, double increasePercentage): _capacity(capacity), _delta(increasePercentage) {
    _data = new Object*[_capacity];
    _size = 0;
}

/**
 * Destructor
 */
VSArray::~VSArray() {
    for (size_t i = 0; i < _size; ++i) {
        delete _data[i]; // Delete the data contained in the array at index i
    }
    delete[] _data; // Delete the array itself
}

/**
 * Inserts the given 'element' at the 'position' in the array. Resizes if needs to.
 * @param element Object* to be inserted
 * @param position size_t index to be inserted at
 * @return true or false whether or not it succeeded
 */
bool VSArray::Insert(Object* element, size_t position) {
    // make sure position is not out of bounds
    if (position > _size) {
        return false;
    }

    // check if need to resize
    if (_size == _capacity) {
        Resize();
    }

    // shift the data after 'position' up an index
    for (size_t i = _size; i > position; --i) {
        _data[i] = _data[i - 1];
    }

    // insert 'element'
    _data[position] = element;
    _size = (_size + 1);

    return true;
}

/**
 * Gets the index of the passed 'element'.
 * @param element Object* to be searched for in the array
 * @return the index of the first occurrence of the passed 'element' or -1 if not found
 */
int VSArray::IndexOf(const Object* element) const {
    for (size_t i = 0; i < _size; ++i) {
        if (_data[i]->Equals(*element)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

/**
 * Removes the data at the specified 'position' in the array.
 * @param position size_t index to be removed
 * @return a pointer to the data that was removed or nullptr if out of bounds
 */
Object* VSArray::Remove(size_t position) {
    // make sure 'position' is within bounds
    if (position >= _size) {
        return nullptr;
    }

    // store a pointer to the removed data
    Object* removedElement = _data[position];

    // shift the data down
    for (size_t i = position; i < _size - 1; ++i) {
        _data[i] = _data[i + 1];
    }

    _size = (_size - 1);
    return removedElement;
}

/**
 * Returns the pointer to the data at the specified position
 * @param position size_t index of the desired data
 * @return Object* at 'position' or nullptr
 */
Object* VSArray::Get(size_t position) const {
    // check to make sure 'position' is within bounds
    if (position >= _size) {
        return nullptr;
    }

    return _data[position];
}

/**
 * ToString method that neatly formats that data contained in the VSArray. This method will e overritten by each
 * object's respective ToString method
 * @return a String representation of that data contained in the VSArray
 */
string VSArray::ToString() const {
    stringstream ss;
    ss << "{";

    for (size_t i = 0; i < _size; ++i) {
        ss << _data[i]->ToString();
        if (i < _size - 1) {
            ss << ", ";
        }
    }

    ss << "}";
    return ss.str();
}

/**
 * Clears all data within the array, freeing their memory and setting '_size' to 0.
 */
void VSArray::Clear() {
    for (size_t i = 0; i < _size; ++i) {
        delete _data[i];
    }
    _size = 0;
}

/**
 * Returns the capacity of the VSArray
 * @return size_t '_capacity' of the VSArray
 */
size_t VSArray::GetCapacity() const {
    return _capacity;
}
