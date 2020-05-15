#include "block.h"

void Block::Build(PNG& im, int upper, int left, int dimension){
    data.resize(dimension, vector<HSLAPixel>(dimension));
    for(int x = 0; x<dimension; x++){
        for(int y = 0; y<dimension; y++){
            data[x][y] = *im.getPixel(left + x, upper + y);
        }
    }
}

void Block::Render(PNG& im, int upper, int left) const{
    int l = Dimension();
    for(int x = 0; x<l; x++) {
        for(int y = 0; y<l; y++) {
            *im.getPixel(left + x, upper + y) = data[x][y];
        }
    }
}

void Block::Negative(){
    int l = Dimension();
    for(int x = 0; x<l; x++) {
        for(int y = 0; y<l; y++) {
            data[x][y].h = (int)(data[x][y].h + 180) % 360;
            data[x][y].l = 1.0 - data[x][y].l;
        }
    }
}

int  Block::Dimension() const{
    return data.size();
}