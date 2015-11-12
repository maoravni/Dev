#ifndef DYNAMIC_ARRAY_H_
#define DYNAMIC_ARRAY_H_

#include <stdio.h>
#include <EventData.h>

template<typename T>
class DynamicArray
{
public:
    DynamicArray()
    {
        itsLength = 0; /*pDynamicArray = new T[itsLength];*/
        pDynamicArray = NULL;
        tempDynamicArray = NULL;
    } //Constructor sets length and creates a DynamicArray
    ~DynamicArray()
    {
        if (pDynamicArray != NULL)
            delete pDynamicArray;
    } //deletes pointers

    T& operator [](int location) //allows for the [] subscript
    {
        if (location < itsLength)
            return pDynamicArray[location];
        return pDynamicArray[itsLength-1];
    }

//        void operator = (DynamicArray& myDynamicArray) //allows to copy DynamicArrays to each other
//        {
//            itsLength = myDynamicArray.size(); //DynamicArray's length is modified to fit the new elements
//            delete pDynamicArray;
//            pDynamicArray = new T[itsLength];
//
//            for (int i = 0; i < itsLength; i++) //pDynamicArray now equals the contents of myDynamicArray
//                pDynamicArray[i] = myDynamicArray[i];
//        }

    void push_back(T newElement) //adds an element to the end
    {
//        printf("vectorLength : %d", itsLength);
//        if (itsLength >= 1000)
//            EventData *e = new EventData();

        tempDynamicArray = new T[itsLength + 1];

        for (int i = 0; i < itsLength; i++) //tempDynamicArray now equals pDynamicArray
            tempDynamicArray[i] = pDynamicArray[i];

        tempDynamicArray[itsLength] = newElement;
        ++itsLength;

        if (pDynamicArray != NULL)
            delete pDynamicArray;
        pDynamicArray = tempDynamicArray;
        tempDynamicArray = NULL;
//        printf("->%d\n", itsLength);
//            itsLength++; //increases length so pDynamicArray will be able to contain the one new element
//            pDynamicArray = new T[itsLength];
//
//            for (int i = 0; i < itsLength; i++) //pDynamicArray now regains it's old contents
//                pDynamicArray[i] = tempDynamicArray[i];
//
//            pDynamicArray[itsLength-1] = newElement; //pDynamicArray now contains the new element
//
//            delete tempDynamicArray; //standard end function clean up - you will see this is most member functions
    }

//        void push_back (DynamicArray& myDynamicArray) //adds another DynamicArray to the end
//        {
//            tempDynamicArray = new T[itsLength+myDynamicArray.itsLength]; //itsLength+myDynamicArray.itsLength creates a new array with the correct size
//
//            for (int i = 0; i < itsLength; i++) //standard copy procedure
//                tempDynamicArray[i] = pDynamicArray[i];
//
//            for (int i = itsLength, j = 0; i < itsLength+myDynamicArray.itsLength; i++, j++) //because it adds onto the end, tempDynamicArray starts at the end
//                tempDynamicArray[i] = myDynamicArray[j];                                           //and myDynamicArray is added on
//            //IMPORTANT NOTE: Never use other member functions that modify the class's local array (add_on, take_off, ect)
//            //in another function as they will affect the rest of your function because all functions use the same two arrays.
//            //It is always better to re-write part of the code or simply copy it to make sure the arrays are not modified in an
//            //undesirable way.
//            itsLength += myDynamicArray.size(); //itsLength is now the appropriate size
//            delete pDynamicArray;
//            pDynamicArray = new T[itsLength];
//
//            for (int i = 0; i < itsLength; i++)
//                pDynamicArray[i] = tempDynamicArray[i];
//
//            delete tempDynamicArray;
//        }

//        void erase(int location) //takes off the element at int location
//        {
//            itsLength--; //similar to the increment in add_on
//            tempDynamicArray = new T[itsLength];
//
//            for (int i = 0, j = 0; i < itsLength+1; i++)
//            {
//                if (i != location) //this prevents the location we want to exclude from returning to the DynamicArray
//                {
//                    tempDynamicArray[j] = pDynamicArray[i];
//                    j++;
//                }
//            }
//
//            delete pDynamicArray;
//            pDynamicArray = new T[itsLength];
//
//            for (int i = 0; i < itsLength; i++)
//                pDynamicArray[i] = tempDynamicArray[i];
//
//            delete tempDynamicArray;
//        }

//        void pop_back() //takes off the element on the end
//        {
//            //NOTE: Refer to the comments for take_off(int location) for an explanation of this function as it is just overloaded
//            itsLength--;
//            tempDynamicArray = new T[itsLength];
//
//            for (int i = 0, j = 0; i < itsLength+1; i++)
//            {
//                if (i != itsLength+1)
//                {
//                    tempDynamicArray[j] = pDynamicArray[i];
//                    j++;
//                }
//            }
//
//            delete pDynamicArray;
//            pDynamicArray = new T[itsLength];
//
//            for (int i = 0; i < itsLength; i++)
//                pDynamicArray[i] = tempDynamicArray[i];
//
//            delete tempDynamicArray;
//        }

//        void erase(int start, int finish) //deletes everything from start() to finish()
//        {
//            for (int i = start; i < finish+1; i++)
//                erase(start); //okay to use other member function as wipe(int, int) does not modify either array itself
//        }

//        void insert(int location, T newElement) //Adds an element to a specified location
//        {
//            tempDynamicArray = new T[itsLength];
//
//            for (int i = 0; i < itsLength; i++)
//                tempDynamicArray[i] = pDynamicArray[i];
//
//            delete pDynamicArray;
//            itsLength++;
//            int test = itsLength;
//            pDynamicArray = new T[itsLength];
//
//            for (int i = 0, j = 0; i < test; i++)
//            {
//                if (i == location)
//                    pDynamicArray[i] = newElement; //if the counter hits the location then add in the new element
//                                             //otherwise it will simply add_on like normal
//                else
//                {
//                    pDynamicArray[i] = tempDynamicArray[j];
//                    j++;
//                }
//            }
//
//            delete tempDynamicArray;
//        }

//        void insert(int location, DynamicArray& myDynamicArray) //Adds another DynamicArray to a specified location
//        {
//            tempDynamicArray = new T[itsLength];
//
//            for (int i = 0; i < itsLength; i++)
//                tempDynamicArray[i] = pDynamicArray[i];
//
//            delete pDynamicArray;
//            itsLength+=myDynamicArray.itsLength;
//            int test = itsLength;
//            pDynamicArray = new T[itsLength];
//
//            int perm, temp, i;
//            for (int j = 0, i = 0; i < test; i++)
//            {
//                if (i == location)
//                {
//                    for (perm = location, temp = 0; temp < myDynamicArray.itsLength; temp++, perm++) //adds on another DynamicArray
//                        pDynamicArray[perm] = myDynamicArray[temp];
//                    i = perm-1;
//                }
//                else
//                {
//                    pDynamicArray[i] = tempDynamicArray[j];
//                    j++;
//                }
//            }
//
//            delete tempDynamicArray;
//        }

//        void exchange(DynamicArray& myOther) //Exchanges two DynamicArrays
//        {
//            if (itsLength > myOther.size()) //so all sizes work out
//            {
//                tempDynamicArray = new T[myOther.size()];
//
//                for (int i = 0; i < myOther.size(); i++)
//                    tempDynamicArray[i] = myOther[i];
//
//                int itsNewLen = itsLength-(itsLength-myOther.size());
//                myOther.itsLength+=(itsLength-myOther.size());
//
//                for (int i = 0; i < myOther.size(); i++)
//                    myOther[i] = pDynamicArray[i];
//
//                itsLength = itsNewLen;
//
//                for (int i = 0; i < itsLength; i++)
//                    pDynamicArray[i] = tempDynamicArray[i];
//            }
//            else
//            {
//                tempDynamicArray = new T[itsLength];
//
//                for (int i = 0; i < itsLength; i++)
//                    tempDynamicArray[i] = pDynamicArray[i];
//
//                int newLen = myOther.size()-(myOther.size()-itsLength);
//                itsLength+=(myOther.size()-itsLength);
//
//                for (int i = 0; i < itsLength; i++)
//                    pDynamicArray[i] = myOther[i];
//
//                myOther.itsLength = newLen;
//
//                for (int i = 0; i < myOther.size(); i++)
//                    myOther[i] = tempDynamicArray[i];
//            }
//        }

//        void copy_paste(int start, int finish, DynamicArray& new_DynamicArray) //Creates a cut section of a DynamicArray
//        {
//            new_DynamicArray.clean();
//            new_DynamicArray.itsLength = ((finish+1)-start);
//
//            for (int i = start, j = 0; i < finish+1; i++, j++)
//                new_DynamicArray[j] = pDynamicArray[i];
//        }

//        void cut_paste(int start, int finish, DynamicArray& new_DynamicArray) //Creates a cut section of a DynamicArray and deletes those variables from the DynamicArray
//        {
//            new_DynamicArray.clean(); //okay because cut_paste does not modify any arrays
//            new_DynamicArray.itsLength = ((finish+1)-start);
//
//            for (int i = start, j = 0; i < finish+1; i++, j++)
//                new_DynamicArray[j] = pDynamicArray[i];
//
//            int count = 0;
//
//            for (int i = start; i < finish+1; i++)
//            {
//                erase(i-count);
//                count++;
//            }
//        }

    void clean() //deletes and restarts everything
    {
        if (pDynamicArray != NULL)
            delete pDynamicArray;
        if (tempDynamicArray != NULL)
            delete tempDynamicArray;
        pDynamicArray = NULL;
        tempDynamicArray = NULL;
        itsLength = 0;
    }

    //SOME SMALL FUNCTIONS
    int size()
    {
        return itsLength;
    }
//        void return_array(T *outside_array) //Edits an array to give you local data NOTE: this version of the function requires the
//        {                                   //the array in main.cpp to be the proper size
//            outside_array = new T[itsLength];
//            for (int i = 0; i < itsLength; i++)
//                outside_array[i] = pDynamicArray[i];
//        }
//        T* replace_array(T *outside_array) //Returns an array if you want local data NOTE: this version of the function does not require
//        {                                  //the array in main.cpp to be the proper size, but it will resize it instead
//            delete outside_array;
//            outside_array = new T[itsLength];
//            for (int i = 0; i < itsLength; i++)
//                outside_array[i] = pDynamicArray[i];
//            return outside_array;
//        }
//        void reverse() //reverses order of the container
//        {
//            tempDynamicArray = new T[itsLength];
//
//            for (int i = itsLength-1, j = 0; i > -1; i--, j++)
//                tempDynamicArray[j] = pDynamicArray[i];
//
//            delete pDynamicArray;
//            pDynamicArray = new T[itsLength];
//
//            for (int i = 0; i < itsLength; i++)
//                pDynamicArray[i] = tempDynamicArray[i];
//
//            delete tempDynamicArray;
//        }
    int search(T term) //returns 1 if term exists
    {
        for (int i = 0; i < itsLength; i++)
        {
            if (pDynamicArray[i] == term)
                return 1;
        }
        return 0;
    }
    void search_results(T term, DynamicArray<int>& otherDynamicArray) //returns locations of the different terms into a DynamicArray
    {
        for (int i = 0; i < itsLength; i++)
        {
            if (pDynamicArray[i] == term)
                otherDynamicArray.push_back(i);
        }
    }
    int amount_found(T term) //returns the number of terms found
    {
        int num = 0;

        for (int i = 0; i < itsLength; i++)
        {
            if (pDynamicArray[i] == term)
                num++;
        }
        return num;
    }
    int first_result(T term) //finds the first T term and returns it's position
    {
        for (int i = 0; i < itsLength; i++)
            if (pDynamicArray[i] == term)
                return i;
        return -1; //ERROR message
    }
    int first_not_result(T term) //first result that ISN'T T term and returns it's position
    {
        for (int i = 0; i < itsLength; i++)
            if (pDynamicArray[i] != term)
                return i;
        return -1; //ERROR message
    }
    T first_not_term(T term) //first T that ISN'T T term
    {
        for (int i = 0; i < itsLength; i++)
            if (pDynamicArray[i] != term)
                return pDynamicArray[i];
        return -1; //ERROR message
    }
    int last_result(T term) //finds the last T term and returns it's position
    {
        for (int i = itsLength - 1; i > 0; i--)
            if (pDynamicArray[i] == term)
                return i;
        return -1; //ERROR message
    }
    int last_not_result(T term) //last result that ISN'T T term and returns it's position
    {
        for (int i = itsLength - 1; i > 0; i--)
            if (pDynamicArray[i] != term)
                return i;
        return -1; //ERROR message
    }
    T last_not_term(T term) //last T that ISN'T T term
    {
        for (int i = itsLength - 1; i > 0; i--)
            if (pDynamicArray[i] != term)
                return pDynamicArray[i];
        return -1; //ERROR message
    }
    bool exists(T term) //returns true if T term exists in the DynamicArray
    {
        for (int i = 0; i < itsLength; i++)
            if (pDynamicArray[i] == term)
                return true; //term exists
        return false; //ERROR message
    }

private:
    int itsLength;
    T *pDynamicArray;
    T *tempDynamicArray;
};

#endif
