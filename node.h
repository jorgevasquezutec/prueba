#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <typename T>
class Node {
protected:
    T data;
    int posX;
    int posY;
    Node<T> *next, *down;

public:
    Node(int posX, int posY,T data) {
        this->data = data;
        this->posX = posX;
        this->posY = posY;
        this->next=nullptr;
        this->down=nullptr;
    }
    void setValue(T data){
        this->data=data;
    }
    T getValue(){
        return this->data;
    }
    int getPosX(){
        return posX;
    }
    int getPosY(){
        return posY;
    }
    void setNextNodeRow(Node<T>* nextRow){
        this->next=nextRow;
    }
    void setNextNodeCol(Node<T>* nexColumn){
        this->down=nexColumn;
    }
    Node<T>* getNextRowNode(){
        return next;
    }
    Node<T>* getNextColNode(){
        return down;
    }

    explicit Node();

    friend class Matrix<T>;
};

#endif //SPARSE_MATRIX_NODE_H
