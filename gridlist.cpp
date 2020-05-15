// File:        gridlist.cpp
// Date:        2020-01-18 00:11
// Description: Contains partial implementation of GridList class
//              CPSC 221 2019W2 PA1
//              Function bodies to be completed by yourselves
//
// ADD YOUR FUNCTION IMPLEMENTATIONS IN THIS FILE
//

#include "gridlist.h"
#include "gridlist_given.cpp"

// Creates a PNG of appropriate pixel dimensions according to the GridList's structure
//   and colours each pixel according the each GridNode's Block data.
// The fully coloured PNG is returned.
PNG GridList::Render() const
{
  PNG image;

  // create a PNG
  int blockSize = northwest->data.Dimension();
  image = PNG(dimx * blockSize, dimy * blockSize);

  // colours each block
  GridNode* curr = northwest;
  for (int i = 0; i < dimy; i++)
    for (int j = 0; j < dimx; j++) {
      int left = j * blockSize;
      int upper = i * blockSize;
      curr->data.Render(image, upper, left);
      curr = curr->next;
    }

  return image;
}

// Allocates a new GridNode containing bdata and
//   attaches it to end of this list.
// Be careful of the special case of inserting into an empty list.
void GridList::InsertBack(const Block& bdata)
{
  GridNode* temp = new GridNode(bdata);
  if(!northwest){
    northwest = temp;
    southeast = temp;
  } else{
    GridNode* l = southeast;
    l->next = temp;
    temp->prev = l;
    southeast = temp;
  }
}

// if this list has an odd number of column blocks, then the right side will have more blocks
// PRE:  this list and inner are not the same physical list
//       this list must have at least two column blocks
//       inner list must have at least one column block
//       inner list must have the same vertical resolution, vertical block dimension, and block size
// POST: this list has nodes of the entire inner list in between the left and right halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_H(GridList& inner)
{
  if (dimy !=  inner.dimy)
    return;

  int first = dimx / 2;
  int last = dimx - first;
  GridNode* tempThisA = northwest;
  GridNode* tempThisB = northwest;
  GridNode* tempInner = inner.northwest;

  while (tempThisA != NULL) {
    for (int i = 0; i < first-1; i++) 
      tempThisA = tempThisA->next;
    tempThisB = tempThisA -> next;

    tempInner -> prev = tempThisA;
    tempThisA -> next = tempInner;

    for (int i = 0; i < inner.dimx; i++)
      tempInner = tempInner -> next;

    if(tempInner != NULL) {
      tempInner -> prev -> next = tempThisB;
      tempThisB -> prev = tempInner -> prev;
    } else {
      inner.southeast -> next = tempThisB;
      tempThisB -> prev = inner.southeast;
    }
    
    tempThisA = tempThisB;
    for (int i = 0; i < last; i++) 
      tempThisA = tempThisA->next;
  }

  inner.northwest = NULL;
  inner.southeast = NULL;
  dimx += inner.dimx;
}

// inner list must have the same horizontal resolution, horizontal block dimension, and block size
// if this list has an odd number of row blocks, then the bottom side will have more blocks
// PRE:  this list and inner are not the same physical list
//       this list must have at least two row blocks
//       inner list must have at least one row block
//       inner list must have same horizontal resolution, horizontal block dimension, and block size
// POST: this list has nodes of the entire inner list in between the upper and lower halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_V(GridList& inner)
{
  if (dimx !=  inner.dimx)
    return;
    
  int first = dimy / 2;
  GridNode* tempThisA = northwest;
  GridNode* tempThisB = northwest;
  
  for (int i = 0; i < first * dimx - 1; i++)
    tempThisA = tempThisA -> next;
  tempThisB = tempThisA -> next;

  tempThisA -> next = inner.northwest;
  inner.northwest -> prev = tempThisA;

  tempThisB -> prev = inner.southeast;
  inner.southeast -> next = tempThisB;

  inner.northwest = NULL;
  inner.southeast = NULL;
  dimy += inner.dimy;
}


// PRE:  this list and otherlist have the same pixel dimensions, block dimensions, and block size
// POST: this list and otherlist are checkered with each other's nodes
//       each list's dimensions remain the same
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::CheckerSwap(GridList& otherlist)
{
  if (Length() < 2) {
    //
  } else {
    GridNode* tempThisA = northwest;
    GridNode* tempThisB = tempThisA;
    GridNode* tempInnerA = otherlist.northwest;
    GridNode* tempInnerB = tempInnerA;

    int oscillator = 1;
    for (int i = 0; i < dimy; i++) {
      for (int j = 0; j < dimx; j++) {
        if (tempThisA->next == NULL)
          return;
        if (i == 0 && j == 0)
          continue;

        tempThisB = tempThisA -> next;
        tempInnerB = tempInnerA -> next;

        if ((i+j) % 2 == oscillator) {
          tempThisA -> next = tempInnerB;
          tempInnerA -> next = tempThisB;
          tempThisB -> prev = tempInnerA;
          tempInnerB -> prev = tempThisA;
          oscillator = !oscillator;
        }

        tempThisA = tempThisB;
        tempInnerA = tempInnerB;
      }
    }
  }
}

// POST: this list has the negative effect applied selectively to GridNodes to form
//         a checkerboard pattern.
//       The northwest block is normal (does not have the negative effect applied).
// Ensure that the checkering looks correct for both odd and even horizontal block dimensions
void GridList::CheckerN()
{
  GridNode* temp = northwest;
  for (int i = 0; i < dimy; i++) {
    for (int j = 0; j < dimx; j++) {
      if ((i+j) % 2 == 1)
        temp->data.Negative();
      temp = temp->next;
    }
  }
}

// Deallocates any dynamic memory associated with this list
//   and re-initializes this list to an empty state
void GridList::Clear()
{
  if(!northwest)
    return;
    
  GridNode* temp = northwest;
  while(temp->next != NULL){
    temp = temp->next;
    delete temp->prev;
  }
  delete temp;

  northwest = NULL;
  southeast = NULL;
  dimx = 0;
  dimy = 0;
}

// Allocates new nodes into this list as copies of all nodes from otherlist
void GridList::Copy(const GridList& otherlist)
{
  GridNode* othCurr = otherlist.northwest;
  GridNode* curr = new GridNode(otherlist.northwest->data);
  northwest = curr;

  GridNode* p;
  while(othCurr->next){
    curr->next = new GridNode(othCurr->next->data);
    othCurr = othCurr->next;
    p=curr;
    curr = curr->next;
    curr->prev = p;
  }

  southeast = curr;
}

// IMPLEMENT ANY PRIVATE FUNCTIONS YOU HAVE ADDED BELOW
//
//