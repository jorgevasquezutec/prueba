#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include "node.h"
#include <vector>
#include <fstream>
using namespace std;

template <typename T>
class Matrix {
private:
    unsigned rows, columns;
    vector<Node<T>* > X;
    vector<Node<T>* > Y;
    static bool toDelete(int _data){
        if(_data==0){
            return true;
        }
        return false;
    }
    static bool toDelete(string _data){
        if(_data=="0"){
            return true;
        }
        return false;
    }
    bool isRowEmpty(unsigned _x){ //row,col
        if(X[_x]==nullptr){
            return true;
        }
        return false;
    }
    bool isColumnEmpty(unsigned _y){
        if(Y[_y]==nullptr){
            return true;
        }
        return false;
    }
    Node<T>* nodeExist(unsigned x,unsigned y){
        if(!isRowEmpty(x)){
              auto closeNode= traverseToNodeRow(x,y);
              if(closeNode==X[x] && closeNode->getPosY()==y){
                  return closeNode;
              }
              else if (closeNode->getNextRowNode()==X[x]){
                  return nullptr;
              }
              else if (closeNode->getNextRowNode()->getPosY()==y){
                  return closeNode->getNextRowNode();
              }
              else{
                  return nullptr;
              }
        }
        return nullptr;
    }

    Node<T>* getLastNodeRow(unsigned row,Node<T>* oldNode){
        Node<T>* tmp=oldNode;
        if(isRowEmpty(row)){
            return tmp;
        }
        if(oldNode->getNextRowNode()==oldNode){
            return oldNode;
        }
        while(tmp->getNextRowNode()!=oldNode && tmp->getNextRowNode()!=nullptr){
            tmp=tmp->getNextRowNode();
        }
        return tmp;
    }
    Node<T>* getLastNodeCol(unsigned column,Node<T>* oldNode){
        Node<T>* tmp=oldNode;
        if(isColumnEmpty(column)){
            return tmp;
        }

        if(oldNode->getNextColNode()==oldNode){
            return oldNode;
        }
        while(tmp->getNextColNode()!=oldNode && tmp->getNextColNode()!=nullptr){
            tmp=tmp->getNextColNode();
        }
        return tmp;
    }

    Node<T>* traverseToNodeRow(unsigned x, unsigned y){
        Node<T> * tmp=nullptr;
        tmp=X[x];
        if(isRowEmpty(x)){
            return tmp;
        }
        Node<T>* firstNode=X[x];
        while( (tmp->getNextRowNode()!=firstNode) && (tmp->getNextRowNode()->getPosY()<y) ){
            tmp=tmp->getNextRowNode();
        }
        return tmp;
    }

    Node<T>* traverseToNodeCol(unsigned x,unsigned y){
        Node<T>* tmp=nullptr;
        tmp=Y[y];
        if(isColumnEmpty(y)){
            return tmp;
        }
        Node<T>* firstNode=Y[y];
        while ((tmp->getNextColNode() != firstNode) &&  (tmp->getNextColNode()->getPosX() < x) ){
            tmp = tmp->getNextColNode();
        }
	    return tmp;
    }

    void connectNode(Node<T> *newNode){
        Node<T>* tmp=nullptr;
        unsigned newRow=newNode->getPosX();
        unsigned newColum=newNode->getPosY();
        Node<T> *prevInRow = nullptr;
	    Node<T> *prevInCol = nullptr;
        if(isRowEmpty(newRow)){
                X[newRow]=newNode;
                //primera instancia se apuenta a si mismo.
                newNode->setNextNodeRow(newNode);
        }else if ((prevInRow = traverseToNodeRow(newRow, newColum))->getPosY() > newColum){
            X[newRow] = newNode;
            newNode->setNextNodeRow(prevInRow);
            tmp = getLastNodeRow(newRow, prevInRow);
            tmp->setNextNodeRow(newNode);
        }
        else{
            prevInRow=traverseToNodeRow(newRow, newColum);
            tmp=prevInRow->getNextRowNode();
            prevInRow->setNextNodeRow(newNode);
            newNode->setNextNodeRow(tmp);
        }
        tmp=nullptr;

        if(isColumnEmpty(newColum)){
              Y[newColum]=newNode;
              newNode->setNextNodeCol(newNode);
        }
        else if ((prevInCol = traverseToNodeCol(newRow, newColum))->getPosX() > newRow){
            Y[newColum] = newNode;
            newNode->setNextNodeRow(prevInCol);
            tmp = getLastNodeCol(newColum, prevInCol);
            tmp->setNextNodeCol(newNode);
        }
        else{
            prevInCol=traverseToNodeCol(newRow, newColum);
            tmp=prevInCol->getNextColNode();
            prevInCol->setNextNodeCol(newNode);
            newNode->setNextNodeCol(tmp);
        }
    }

    void deleteNode(Node<T> *oldNode){
        Node<T> * tmp = nullptr;
        unsigned oldRow = oldNode->getPosX();
        unsigned oldCol = oldNode->getPosY();
        Node<T> * prevInRow = traverseToNodeRow(oldRow, oldCol);
	    Node<T> * prevInCol = traverseToNodeCol(oldRow, oldCol);

        //si es el primer elemento
        if(oldNode==X[oldRow]){
            if(oldNode->getNextRowNode()==X[oldRow]){
                X[oldRow]=nullptr;
            }else{
                //circular.
                 X[oldRow]=oldNode->getNextRowNode();
                 tmp=getLastNodeRow(oldRow,oldNode);
                 tmp->setNextNodeRow(X[oldRow]);
            }
        }
        //el a eliminar es el ultimo.
        else if(oldNode->getNextRowNode()==X[oldRow]){
            prevInRow->setNextNodeRow(X[oldRow]);

        }
        else{
            prevInRow->setNextNodeRow(oldNode->getNextRowNode());
        }
        tmp=nullptr;

        //replicamos con las columnas.
        if(oldNode==Y[oldCol]){
            if(oldNode->getNextColNode()==Y[oldCol]){
                Y[oldCol]=nullptr;
            }
            else{
                Y[oldCol]=oldNode->getNextColNode();
                tmp=getLastNodeCol(oldCol,oldNode);
                tmp->setNextNodeCol(Y[oldCol]);
            }
        }else if (oldNode->getNextColNode()==Y[oldCol]){
            prevInCol->setNextNodeCol(Y[oldCol]);
        }
        else{
            prevInCol->setNextNodeCol(oldNode->getNextColNode());
        }
    }

    void replaceNode(Node<T>* oldNode,T data){
        oldNode->setValue(data);
    }





public:
    Matrix();
    Matrix(unsigned rows, unsigned columns){
        this->rows = rows;
        this->columns = columns;
        for (int i = 0; i < this->rows; ++i)
            X.push_back(nullptr);
        for (int i = 0; i < this->columns; ++i)
            Y.push_back(nullptr);
    }

    T get(unsigned x,unsigned y){
        return this->operator()(x,y);
    }
    //sobrecargar el operador igual 
    //constructor que retorne matriz
    Matrix(const Matrix<T> &matrix):rows(matrix.rows),columns(matrix.columns),X(matrix.X),Y(matrix.Y){}
    Matrix<T> operator=(Matrix<T> matrix){
        auto matrixtmp = new Matrix<T>(matrix.rows,matrix.columns);
        matrixtmp->X=matrix.X;
        matrixtmp->Y=matrix.Y;
        return *(matrixtmp);
    }
 
    void set(unsigned _x, unsigned _y, T _data){
        if ((_x > rows-1) || (_y > columns-1))
            throw std::out_of_range("Invalid x o y values");
        auto oldNode=nodeExist(_x,_y);
        if(oldNode){
            if(toDelete(_data)){
                this->deleteNode(oldNode);
                delete oldNode;
                oldNode=nullptr;
            }
            else{
                this->replaceNode(oldNode,_data);
            }
        }else{
            auto newNode=new Node<T>(_x,_y,_data);
            connectNode(newNode);
        }
    }
    T operator()(unsigned x, unsigned y){
        if ((x > rows-1) || (y > columns-1)){
            throw std::out_of_range("Invalid coordinates");
        }
        auto exist=nodeExist(x,y);
        if(exist){
            return exist->getValue();
        }
        else
        {
           return 0;
        }
        

    };
    Matrix<T> operator*(T scalar){
        auto transpose = new Matrix<T>(this->rows, this->columns);
        Node<T> *current;
        Node<T> *firtNode;
        for (int i = 0; i < this->rows; ++i) {
            current = X[i];
            firtNode=current;
            if(current!=nullptr){
                while ( current->getNextRowNode()!=firtNode) {
                    transpose->set(current->getPosX(), current->getPosY(), current->getValue()*scalar);
                    current = current->getNextRowNode();
                }
                 transpose->set(current->getPosX(), current->getPosY(), current->getValue()*scalar);
            }
        }
        return *transpose;
    }
    Matrix<T> operator*(Matrix<T> &other){
        if(columns!=other.rows)
            throw std::out_of_range("Invalid matrix dimentions");
        auto result= new Matrix<T>(rows,other.columns);
        Node<T> *current;
        Node<T> *startNode;
        bool start=true;
        for (size_t i=0;i<rows;i++){
            current=X[i];
            startNode=current;
            if(current!=nullptr){
                while(current!=startNode || start){
                    for(size_t j=0;j<other.columns;++j){
                        auto resA=current->getValue();
                        auto resB=other.get(current->getPosY(),j);
                        if(result->get(current->getPosX(),j)!=this->operator()(current->getPosX(),j))
                            result->set(current->getPosX(),j,result->get(current->getPosX(),j)+ resA * resB);
                        else
                            result->set(current->getPosX(),j,resA*resB);
                    }
                    current=current->getNextRowNode();
                    start=false;
                }

            }
            start=true;
        }
        return *result;
    }
    Matrix<T> operator+(Matrix<T> &other){
      if (!(this->rows == other.rows && this->columns == other.columns))
            throw std::out_of_range("Invalid Matrix dimensions");
      auto result= new Matrix<T>(this->rows,this->columns);
      Node<T> *current=nullptr;
      Node<T> *oCurrent=nullptr;
      Node<T> *firtCurrent;
      Node<T> *firtOCurrent;

      for(size_t i=0; i<rows;i++){
          current=X[i];
          firtCurrent=current;
          if(current!=nullptr){
                while ( current->getNextRowNode()!=firtCurrent) {
                    result->set(current->getPosX(), current->getPosY(), current->getValue()+other.get(current->getPosX(),current->getPosY()));
                    current = current->getNextRowNode();
                }
                 result->set(current->getPosX(), current->getPosY(), current->getValue()+other.get(current->getPosX(),current->getPosY()));
          }
      }

      for (size_t i=0;i<rows;i++){
          oCurrent=other.X[i];
          firtOCurrent=oCurrent;
            if(oCurrent!=nullptr){
                while ( oCurrent->getNextRowNode()!=firtOCurrent) {
                    result->set(oCurrent->getPosX(), oCurrent->getPosY(), oCurrent->getValue()+this->get(oCurrent->getPosX(),oCurrent->getPosY()));
                    oCurrent = oCurrent->getNextRowNode();
                }
                 result->set(oCurrent->getPosX(), oCurrent->getPosY(), oCurrent->getValue()+this->get(oCurrent->getPosX(),oCurrent->getPosY()));
          }
      }

      return *result;
    }
    Matrix<T> operator-(Matrix<T> &other){
      if (!(this->rows == other.rows && this->columns == other.columns))
            throw std::out_of_range("Invalid Matrix dimensions");
      auto result= new Matrix<T>(this->rows,this->columns);
      Node<T> *current=nullptr;
      Node<T> *oCurrent=nullptr;
      Node<T> *firtCurrent;
      Node<T> *firtOCurrent;

      for(size_t i=0; i<rows;i++){
          current=X[i];
          firtCurrent=current;
          if(current!=nullptr){
                while ( current->getNextRowNode()!=firtCurrent) {
                    result->set(current->getPosX(), current->getPosY(), current->getValue()-other.get(current->getPosX(),current->getPosY()));
                    current = current->getNextRowNode();
                }
                 result->set(current->getPosX(), current->getPosY(), current->getValue()-other.get(current->getPosX(),current->getPosY()));
          }
      }

      for (size_t i=0;i<rows;i++){
          oCurrent=other.X[i];
          firtOCurrent=oCurrent;
            if(oCurrent!=nullptr){
                while ( oCurrent->getNextRowNode()!=firtOCurrent) {
                    result->set(oCurrent->getPosX(), oCurrent->getPosY(), oCurrent->getValue()-this->get(oCurrent->getPosX(),oCurrent->getPosY()));
                    oCurrent = oCurrent->getNextRowNode();
                }
                 result->set(oCurrent->getPosX(), oCurrent->getPosY(), oCurrent->getValue()-this->get(oCurrent->getPosX(),oCurrent->getPosY()));
          }
      }

      return *result;
    }
    Matrix<T> transpose(){
        Matrix<T> *transpose = new Matrix<T>(this->columns, this->rows);
        Node<T> *current;
        Node<T> *firtNode;
        for (int i = 0; i < this->rows; ++i) {
            current = X[i];
            firtNode=current;
            if(current!=nullptr){
                while ( current->getNextRowNode()!=firtNode) {
                    transpose->set(current->getPosY(), current->getPosX(), current->getValue());
                    current = current->getNextRowNode();
                }
                 transpose->set(current->getPosY(), current->getPosX(), current->getValue());
            }
        }
        return *transpose;
    }


    void outputfile(string filename){
        ofstream myfile (filename,fstream::out);
    }

    void print(){

        Node<T>* tmp=nullptr;
        Node<T>* firtsNodeinRow=nullptr;
        int col=0;
        for (size_t i=0; i<this->rows;i++){
                tmp=X[i];
                firtsNodeinRow=tmp;
                for(size_t j=0; j<this->columns;j++){
                    if(tmp!=nullptr){
                        col=tmp->getPosY();
                        if(col==j){
                            cout<<tmp->getValue()<<" ";
                            tmp=tmp->getNextRowNode();
                        }else if((tmp == firtsNodeinRow && j != 0 ) || isRowEmpty(i)){
                            cout<<"0"<<" ";
                        }
                        else{
                            cout<<"0"<<" ";
                        }
                    }
                    else{
                        cout<<"0"<<" ";
                    }
                }
                cout << "\n";
        }
        cout<<"\n";
    }
    ~Matrix(){
    //     Node<T> *current;
    //     Node<T> *firtNode;
    //     Node<T> *tmp=nullptr;
    //     for (int i = 0; i < this->rows; ++i) {
    //         current = X[i];
    //         firtNode=current;
    //             while ( current!=nullptr) {
    //                 if(current->getNextRowNode()==current)
    //                      tmp=nullptr;
    //                 else 
    //                      tmp=current->getNextRowNode();
    //                 this->deleteNode(current);
    //                 delete current;
    //                 current = tmp;
    //             }
    //         }
    }
};

#endif //SPARSE_MATRIX_MATRIX_H