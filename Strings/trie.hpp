#ifndef _TRIE_HPP
#define _TRIE_HPP

#include <iostream>
#include <string>
#include <set>
using namespace std;

namespace trie_implementation{

struct Node;

struct Key{
    char data;
    Node* ptr{};
    unsigned count{};
    bool endOfString{};

    Key(){}
    Key(char c): data(c){}
    Key(char c, Node* p, bool swapEOS): data(c), ptr(p){
        if(swapEOS) endOfString = !endOfString;
    }
    Key(const Key& a, unsigned sum){
        *this = a;
        this->count += sum;
    }
};

bool operator<(const Key& a, const Key& b){
    return a.data < b.data;
}

bool operator>(const Key& a, const Key& b){
    return a.data > b.data;
}

bool operator==(const Key& a, const Key& b){
    return a.data == b.data;
}

bool operator!=(const Key& a, const Key& b){
    return a.data != b.data;
}

bool operator>=(const Key& a, const Key& b){
    return a.data >= b.data;
}

bool operator<=(const Key& a, const Key& b){
    return a.data >= b.data;
}

struct Node{
    set<Key> keys;
    Node(){
    }

    // SWAP FUCTION PARA APAGAR O PRIMEIRO PARAMETRO E INSERIR O SEGUNDO
    set<Key>::iterator swap(Key& a, Key& b){
        keys.erase(a);
        return keys.insert(b).first;
    }
    set<Key>::iterator swap(set<Key>::iterator& it, Key& b){
        keys.erase(*it);
        return keys.insert(b).first;
    }
};

class Trie{
protected:
    enum {MAXSIZE=27, MAX_IN_EXTRACT_FUNCTION=50};
    enum classificadores {MISMATCH, MATCH, MATCH_AND_COUNT};

    Node* root{};
    unsigned nodeCount{};

public:
    Trie(){}
    ~Trie(){
        kill(root);
        root = nullptr;
    }

    Node* getRoot(void){ return this->root; }

protected:
    Node* insert(Node* node, char* input, bool& swapEOS){
        if(!node and *input){
            node = new Node();
            nodeCount++;
            if(!root) root = node;
        }
        if(*input){
            auto needle = node->keys.find(Key(*input));
            if(needle != node->keys.end()){ // achou
                Node* inserted = insert(needle->ptr, input+1, swapEOS);
                Key out = Key(*input, inserted, swapEOS);
                out.endOfString = needle->endOfString;
                if(out.ptr != needle->ptr)
                    node->swap(needle, out);
            }
            else{
                Key in(*input);
                auto needle = node->keys.insert(in).first;
                Node* inserted = insert(needle->ptr, input+1, swapEOS);
                Key out = Key(*input, inserted, swapEOS);
                if(out.ptr != in.ptr or out.endOfString != in.endOfString)
                    node->swap(in, out);
            }
            swapEOS = false;
        }
        else swapEOS = true;
        return node;
    }

    int search(Node* node, char* input){
        if(node){
            if(*input != '\0'){
                Key needle(*input);
                auto itFound = node->keys.find(needle);
                if(itFound != node->keys.end()){
                    int classf = search(itFound->ptr, input+1);
                    if(classf == MATCH_AND_COUNT){
                        Key newKey(*itFound, 1);
                        node->swap(itFound, newKey);
                        return MATCH;
                    }
                    else return classf == MATCH;
                }
                else return MISMATCH;
            }
            else return MATCH_AND_COUNT;
        }
        else if(*input) return MISMATCH;
        return MATCH_AND_COUNT;
    }

    int searchStrict(Node* node, char* input){
        if(node){
            if(*input != '\0'){
                Key needle(*input);
                auto itFound = node->keys.find(needle);
                if(itFound != node->keys.end()){
                    int classf = search(itFound->ptr, input+1);
                    if((classf == MATCH_AND_COUNT or classf == MATCH) and itFound->endOfString){
                        if(classf == MATCH_AND_COUNT){
                            Key newKey(*itFound, 1);
                            node->swap(itFound, newKey);
                        }
                        return MATCH;
                    }
                    else return MISMATCH;
                }
                else return MISMATCH;
            }
            else return MATCH_AND_COUNT;
        }
        if(*input) return MISMATCH;
        return MATCH_AND_COUNT;
    }

    void printPath(Node* node, string& aux){
        if(node){
            set<Key>::iterator it = node->keys.begin();
            while(it != node->keys.end()){
                aux.push_back(it->data);
                printPath(it++->ptr, aux);
            }
            if(it == node->keys.end())
                aux.pop_back();
        }
        else{
            cout << aux << endl;
            aux.pop_back();
        }
    }

    // FUNÇÃO ABAIXO MOSTRA TODO O CAMINHO MARCADO POR END_OF_STRING
    void printRestrict(Node* node, string& aux){
        if(node){
            for(auto item: node->keys){
                aux.push_back(item.data);
                if(item.endOfString)
                    cout << aux << endl;
                printRestrict(item.ptr, aux);
            }
        }
        aux.pop_back();
    }

    //COLOCA EM HANDLE A PROXIMA STRING VALIDA A PARTIR DE NODE
    bool extractNextStr(Node* node, string& handle, bool first){
        if(node){
            auto itFound = node->keys.find(Key(handle.back()));
            if(itFound != node->keys.end() and !first){
                return extractNextStr(itFound->ptr, handle, first);
            }
            else{
                for(auto item: node->keys){
                    if(item.endOfString){
                        handle.push_back(item.data);
                        return true;
                    }
                }
                for(auto item: node->keys){
                    handle.push_back(item.data);
                    if(item.endOfString) return true;
                    if(extractNextStr(item.ptr, handle, first)) return true;
                }
            }
        }
        handle.pop_back();
        return false;
    }

    // ESSA FUNÇÃO ASSUME QUE A TRIE NAÕ ESTÁ VAZIA
    void extractAll(Node* node, string& aux, vector<string>& container, unsigned& cursor){
        if(node){
            for(auto item: node->keys){
                aux.push_back(item.data);
                if(item.endOfString and cursor < MAX_IN_EXTRACT_FUNCTION)
                    container[cursor++] = aux;
                extractAll(item.ptr, aux, container, cursor);
            }
        }
        aux.pop_back();
    }

    unsigned heightAux(Node* node){
        if(node){
            unsigned max = 0, current;
            for(auto item: node->keys){
                current = heightAux(item.ptr);
                max = (current > max)?current:max;
            }
            return max + 1;
        }
        return 0;
    }

    void kill(Node* node){
        if(node){
            set<Key>::iterator handle;
            while(!node->keys.empty()){
                handle = node->keys.begin();
                kill(handle->ptr);
                node->keys.erase(*handle);
            }
            delete node;
        }
    }

public:
    void push(char* input){
        bool endOfString = false;
        insert(root, input, endOfString);
    }

    bool find(char* input, bool strict = false){
        int classf;
        if(!strict) classf = search(root, input);
        else classf = searchStrict(root, input);
        return (classf == MATCH or classf == MATCH_AND_COUNT);
    }

    void print(){
        string aux;
        printRestrict(root, aux);
    }

    bool empty(void){ return !nodeCount; }
    unsigned size(void){ return nodeCount; }
    unsigned height(void){ return heightAux(root); }

};

} // trie_implementation namespace

#endif //_TRIE_HPP