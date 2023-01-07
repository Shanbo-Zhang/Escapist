# Escapist
## Targets / Goals:
* Cross-platfrom,
* Support both GUI and Console,
* Comprehensive Framework

## Support Features:
* `ArrayList<T>`
* `ByteArray`

## Plan:
* `BasicString<T>`
* `HashMap<Key, Value>`
* Json(s)
* `LinkedList<T>`
* `DoubleLinkedList<T>`
* Socket, ServerSocket
* `Thread`

## Development Log

### January 7th 2023:
Finish: `ArrayList<T>::Insert`, `ArrayList<T>::Empty`, `ArrayList<T>::EnsureCapacity`, `ArrayList<T>::Delete`, `ArrayList<T>::Assign`, `ArrayList<T>::Left`, `ArrayList<T>::Right`, `ArrayList<T>::Middle`, `ByteArray`

Interesting Bugs:
* In ArrayList<T> constructor, I forgot to use a member variable without initialization in construcor in a member function.

### January 6th 2023:
Because I linked the old repository to the wrong directory, I have to copy and update all my code to this new repository.
  
Finish: `ArrayList<T>::Append`, `ArrayList<T>::Prepend`, `ArrayList<T>::Constructor`, `ArrayList<T>::Destructor`, `ArrayList<T>::Getters`
  
Interesting Bugs:
* I forgot to multiply sizeof(T) at the end of `ArrayList<T>::TotalCapacpty`.
* In `ArrayList<T>::GetAt` and `ArrayList<T>::GetConstAt`, I typed to data_[size_]. Ironically, I made this mistake at that time, too. And I spent 2 hours working out that.
