#include "DynamicStruct.hpp"
#ifndef LIB
#define LIB
#include <iostream>
#include <list>
#include <string>
#include <cstring>
using namespace std;
#endif

int main(){
  return Menu();
}

void Record::MergeAndInsert(){
   DynamicStruct* temp = new DynamicStruct;
   temp->Insert();
   auto Found = this->Search(string(temp->Name));
   if(Found == nullptr)
    this->Values.push_back(temp);
   else{
    cout << "Já existe um campo com esse nome no registro. " << endl;
    cout << "Campo não inserido no registro. " << endl;
    Pause();
   }
}

DynamicStruct* Record::Search(string elementToFind) const{
   for(auto it = this->Values.begin(); it != this->Values.end(); it++){
      if((*it)->Name == elementToFind)
         return *it;
   }
   return nullptr;
}  

void Record::FindByName(){
   string elementToFind;
   cout << "Digite o elemento a ser encontrado: ";
   cin.ignore();
   getline(cin, elementToFind);
   auto Found = this->Search(elementToFind);
   if(Found != nullptr)
      cout << endl << *Found << endl;
   else cout << "Elemento não foi encontrado!" << endl;
   Pause();
}

void Record::Burn(){
   while(this->Values.size()){
      auto FrontField = this->Values.front();
      delete FrontField;
      this->Values.pop_front();
   }
}

void Record::AreUSure(){
   char op;
   cout << "Você deseja MESMO excluir o registro atual? ";
   cin >> op;
   if(op == 'S' or op == 's') Burn();
   else cout << "Lúcido!" << endl;
}

void Record::RemoveByName(){
   string name;
   cout << "Qual o nome do campo que você deseja excluir? ";
   cin.ignore();
   getline(cin, name);
   for(auto Field: this->Values){
      if(Field->Name == name){
        this->Values.remove(Field);
        delete Field;
        cout << "Elemento removido com sucesso. " << endl;
        Pause();
        return;
      }
   }
   cout << "Elemento não foi encontrado. " << endl;
   Pause();
}

void Record::FindAll(){
   if(this->Values.size()){
      cout << "Todos os campos do registro atual: " << endl;
      for(auto Fields: this->Values)
         cout << (*Fields).Name << endl;
   }
   else
      cout << "Não há registros. " << endl;
   Pause();
}

void DynamicStruct::Insert(){
      string name, type, value;
      cout << "Insira o nome do campo: ";
      cin.ignore();
      getline(cin, name);
      cout << "Insira o tipo do campo: ";
      cin >> type;
      cout << "Insira o valor do campo: ";
      if(type == "string"){
         cin.ignore();
         getline(cin, value);
     }
      else cin >> value;
      
      this->Name = name;
      this->Type = type;

      if(type == "int")
         this->Key = new int(stoi(value));
      else if(type == "unsigned int" or type == "unsigned")
         this->Key = new unsigned(stoul(value));
      else if(type == "unsigned long long")
         this->Key = new unsigned long long(stoull(value));
      else if(type == "float")
         this->Key = new float(stof(value));
      else if(type == "double")
         this->Key = new double(stod(value));
      else if(type == "long double")
         this->Key = new long double(stold(value));
      else if(type == "string"){
         size_t length = value.size() + 1;
         this->Key = new char[length];
         strcpy((char*)this->Key, value.c_str());
      }
      else
         cout << "Tipo não suportado." << endl;
}

ostream &operator <<(ostream &out, const DynamicStruct &Other){
   out << "Field Name: " << Other.Name << endl;
   out << "Field Type: " << Other.Type << endl;
   out << "Field Value: ";
   string type = Other.Type;
   if(type == "int")
      out << *((int*)Other.Key);
   else if(type == "unsigned int" or type == "unsigned")
      out << *((unsigned int*)Other.Key);
   else if(type == "unsigned long long")
      out << *((unsigned long long*)Other.Key);
   else if(type == "float")
      out << *((float*)Other.Key);
   else if(type == "double")
      out << *((double*)Other.Key);
   else if(type == "long double")
      out << *((long double*)Other.Key);
   else{
      out << string((char*)Other.Key);
   }
   out << endl;
   return out;
}

bool operator==(const DynamicStruct &This, const DynamicStruct &Other){
   if(This.Name == Other.Name and This.Type == Other.Type)
      return true;
   return false;
}

void ClearWindow(){
   for(int n=0; n<10; n++)
      cout << "\n\n\n\n\n\n\n\n\n\n";
}

void Pause(){
   cin.ignore();
   scanf("%*c");
}

int Menu(){
   char op;
   Record Records;
   while(true){
      ClearWindow();
      cout << " ## MENU ## " << endl;
      cout << "(1) Inserir um novo campo. " << endl;
      cout << "(2) Mostrar campos. " << endl;
      cout << "(3) Buscar campo pelo nome. " << endl;
      cout << "(4) Apagar um campo pelo nome. " << endl;
      cout << "(5) Apagar todos os campos. " << endl;
      cout << "(6) Sair. " << endl << endl;
      cout << "Insira a opção: ";
      cin >> op;
      switch(op){
         case '1':
            Records.MergeAndInsert();
         break;
         
         case '2':
            Records.FindAll();
         break;

         case '3':
            Records.FindByName();
         break;
         
         case '4':
            Records.RemoveByName();
         break;

         case '5':
            Records.AreUSure();
         break;

         case '6': return 0;
      }
   }
   return 0;
}