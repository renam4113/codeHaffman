#include <iostream>
#include <string.h>
#include <string>
#include <unordered_map>
#include <bitset>
#include <fstream>

using namespace std;

string binaryToAscii(const string& binaryString) {
    string result;
    for (size_t i = 0; i < binaryString.length(); i += 8) {
        bitset<8> bits(binaryString.substr(i, 8));
        char asciiChar = static_cast<char>(bits.to_ulong());
        result += asciiChar;
    }
    return result;
}



class Node
{ 	char str;
    int freq = 0;
    Node* next = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
 public:
    Node(char s, int fr=0, Node* N=nullptr, Node* L=nullptr, Node* R=nullptr)
      {str = s; freq = fr;   next = N; left = L; right = R;}

 friend class Code;
};


class Code
{	Node* root;
 public:
    Code() { root = nullptr;}
    Code(string);
    ~Code(){ if(root) delete []root; root = nullptr;};
    void PrintList();
    void PrintTree(Node *, ofstream &);
    void encode(Node* root, string str, unordered_map<char, string> &huffmanCode);
    Node* GetHead() {return root;} 
    void FormTree();	 
    void printTree(ofstream &);
    string CodeText(string);
    string Decodetext(string);
    void HaffmanCode(string s);
    string trueDecode(Node* root, const string& encodedText);
};



  string Code:: Decodetext(string code){
    string s = trueDecode(root, code);
    return s;
  }
  
  
  string Code::trueDecode(Node* root, const string& encodedText) {
      Node* current = root;
      string decodedText = "";
      for (char bit : encodedText) {
          if (bit == '0') {
              current = current->left;
          } else {
              current = current->right;
          } 
          if (current->left == nullptr && current->right == nullptr) {
              decodedText += current->str;
              current = root;
          }
      }
      return decodedText;
  }
  


void Code::printTree(ofstream& out){
    PrintTree(root, out);
}

void Code::PrintTree(Node* node, ofstream & out) {
    if (node != nullptr) {

        if(node -> str != '\0') {
          string s = "";
          s += node -> str;
          out << s << ":" << node->freq  << endl;
        }
        PrintTree(node->left, out);  
        PrintTree(node->right, out); 
    }
}

string Code:: CodeText(string S){
    unordered_map<char, string> huffmanCode;
    (*this).encode((*this).GetHead(), "", huffmanCode);
    string codedText = "";
    for (char i : S) {
        codedText += huffmanCode[i];
    }
    return codedText;
    // ofstream out;
    // out.open("encode.txt");
    // if (out.is_open())
    // {   
    //     out << "all code:" << endl;
    //     for(auto it = huffmanCode.begin(); it != huffmanCode.end(); it++) out << it->first << " " << it->second << endl;
    //     out << "encode text:" << binaryToAscii(codedText) << endl;
    // }
} 








void Code:: encode(Node* root, string str,
  unordered_map<char, string> &huffmanCode)
{
  if (root == nullptr)
    return;

  // found a leaf node
  if (!root->left && !root->right) {
    huffmanCode[root->str] = str;
  }

  encode(root->left, str + "0", huffmanCode);
  encode(root->right, str + "1", huffmanCode);
}

void Code::FormTree() {
    while (root->next != nullptr){
        Node *p = root;
        Node *q = root->next;
        Node *r = new Node('\0', p->freq + q->freq, nullptr, p, q);
        root = q -> next;
        p->next = nullptr;
        q->next = nullptr;
        if (root == nullptr || r->freq < root->freq) {
            r->next = root;
            root = r;
        } else {
            Node* current = root;
            while (current->next != NULL && r->freq > current->next->freq) {
                current = current->next;
            }
            r->next = current->next;
            current->next = r;
        }
    }
}

void Code:: PrintList(){
  Node *p = root;
  while(p != nullptr){
    cout << p -> freq << " ";
    p = p -> next;
  }
  cout << endl;
}


Code:: Code(string S){
  unordered_map<char, int> freq;
  root = nullptr;
  for (char ch: S) {
    freq[ch]++;
  } 
 for (auto pair: freq){
      Node* p = new Node(pair.first, pair.second, nullptr, nullptr);
      if (root == nullptr || p -> freq < root -> freq) {
              if(root) p -> next = root;
              root = p;
      } 
      else {
              Node* current = root;
              while (current->next != nullptr && p -> freq > current->next->freq) {
                  current = current->next;
              }
              p -> next = current->next;
              current->next = p;
      }
  }
}

int main() {
  string S;
  string cod;
  string decod;
  bool flag = true;
  ifstream in("text.txt");
  char a;
  if (in.is_open()){
    while (!in.eof()) {
            in.get(a);
            S += a;
    }
  }
  in.close();

  Code c(S);
  c.FormTree();

  

  while(flag){

  int x;
  cout << endl;
  cout << "выберите один из вариантов" << endl;
  cout << "1) узнать кодируемый текст" << endl;
  cout << "2) закодировать текст" << endl;
  cout <<"3) декодировать текст" << endl;
  cout << "4) выход" << endl;

  cin >> x;


  if(x == 1) cout << S << endl;   
  else if(x == 2){
      cod = c.CodeText(S);
      ofstream out("encode.txt");
      if(out.is_open()){
        c.printTree(out);
        string Code = binaryToAscii(cod);
        out << Code << endl;
      }
      out.close();
  }
    
  else if(x == 3){  
    if(cod.size() > 0){
      decod = c.Decodetext(cod);
      ofstream fout("decode.txt");
      if(fout.is_open()) fout << decod << endl;
      fout.close();
    }
    else cout << "неизвестна кодируемая строка" << endl;
    }

  else if(x == 4) flag = false;
  }
}