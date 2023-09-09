#include <iostream>
#include <map>

int main ()
{
  std::map<char,int> mymap;
  std::map<char,int>::iterator it;
  std::map<int,char> myseg;

  mymap['c']=50;
  mymap['b']=100;
  mymap['d']=200;
  mymap['a']=150;
  mymap['z']=0;
  mymap['h']=12;
std::cout << "b => " << mymap.find('b')->second << '\n';
  it = mymap.find('b');
  if (it != mymap.end()){
    mymap.erase (it);
  }
  mymap['b']=100;
  // print content:
  std::cout << "elements in mymap:" << '\n';
  std::cout << "a => " << mymap.find('a')->second << '\n';
  std::cout << "c => " << mymap.find('c')->second << '\n';
  std::cout << "d => " << mymap.find('d')->second << '\n';
  std::cout << "elements in mymap interretor:" << '\n';
  for(auto i = mymap.begin(); i != mymap.end(); i++){
    std::cout <<i->first << "=> " << i->second << '\n';
  }
  myseg[111] = 'A';
  myseg[500] = 'Z';
  myseg[200] = 'B';
  myseg[222] = 'X';
  myseg[110] = 'W';
  myseg[99]  = 'D';
  std::cout << "elements in myseg interretor:" << '\n';
  for(auto i = myseg.begin(); i != myseg.end(); i++){
    std::cout <<i->first << "=> " << i->second << '\n';
  }

  return 0;
}