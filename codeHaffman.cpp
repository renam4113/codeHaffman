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
    Node* next = NULL;
    Node* left = NULL;
    Node* right = NULL;
 public:
    Node(char s, int fr=0, Node* N=NULL, Node* L=NULL, Node* R=NULL)
      {str = s; freq = fr;   next = N; left = L; right = R;}

 friend class Code;
};


class Code
{	Node* root;
 public:
    Code() { root = NULL;}
    Code(string);
    ~Code(){ if(root) delete []root; root = NULL;};
    void PrintList();
    void PrintTree(int,Node *);
    void encode(Node* root, string str,
      unordered_map<char, string> &huffmanCode);
    Node* GetHead() {return root;} 
    void FormTree();	 
    void DecodeText(string code);
    void printTree();
    void HaffmanCode(string s);
    void decode(Node* root, int &index,const string & str);
  string trueDecode(Node* root, const string& encodedText);
};


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
  


void Code::printTree(){
    PrintTree(0, root);
}

void Code::PrintTree(int level, Node* node) {
    if (node != NULL) {
        cout << node->freq << "(" << node->str << ")" << " Level: " << level << endl;
        PrintTree(level + 1, node->left);  
        PrintTree(level + 1, node->right); 
    }
}


void Code:: HaffmanCode(string S){
    unordered_map<char, string> huffmanCode;
    (*this).encode((*this).GetHead(), "", huffmanCode);
    string codedText = "";
    for (char i : S) {
        codedText += huffmanCode[i];
    }
    ofstream out;
    out.open("encode.txt");
    if (out.is_open())
    {   
        out << "all code:" << endl;
        for(auto it = huffmanCode.begin(); it != huffmanCode.end(); it++) out << it->first << " " << it->second << endl;
        out << "encode text:" << binaryToAscii(codedText) << endl;
    }
    out.close();
    string decodedtext = (*this).trueDecode(root, codedText);   
    out.open("decode.txt");     
    if (out.is_open())
    {
        out <<" decode text:"<<decodedtext << endl;
    }
    out.close(); 
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
    while (root->next != NULL){
        Node *p = root;
        Node *q = root->next;
        Node *r = new Node('\0', p->freq + q->freq, NULL, p, q);
        root = q -> next;
        p->next = nullptr;
        q->next = nullptr;
        if (root == NULL || r->freq < root->freq) {
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
  while(p != NULL){
    cout << p -> freq << " ";
    p = p -> next;
  }
  cout << endl;
}


Code:: Code(string S){
  unordered_map<char, int> freq;
  root = NULL;
  for (char ch: S) {
    freq[ch]++;
  } 
 for (auto pair: freq){
      Node* p = new Node(pair.first, pair.second, NULL, NULL);
      if (root == NULL || p -> freq < root -> freq) {
              if(root) p -> next = root;
              root = p;
      } 
      else {
              Node* current = root;
              while (current->next != NULL && p -> freq > current->next->freq) {
                  current = current->next;
              }
              p -> next = current->next;
              current->next = p;
      }
  }
}

int main() {
  string S;
  ifstream in("text.txt");
  if (in.is_open()) getline(in, S);
  in.close();   
  Code c(S);
  c.FormTree();
  c.HaffmanCode(S);
}