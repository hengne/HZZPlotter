//config and parser
//Chad Jarvis
// modified and extended by 
//  Hengne Li @ LPSC Grenoble, 2012

#include <config.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <cstdlib>


config::config(char* filename) {
  std::vector<std::string> rcplist;
  std::vector <std::string> parsestring;
  std::ifstream rcpfile(filename);
  if(rcpfile.fail()) {
    std::cout << "Cant open file " << filename << std::endl;
  }
  std::string line;

  while(std::getline(rcpfile,line)) {
    rcplist.push_back(line);
  }
  for(unsigned int i=0; i<rcplist.size(); i++) {
    if((rcplist[i].find("//",0))!=string::npos) {
      rcplist[i].erase(rcplist[i].find("//",0));
    }
    parsestring=parser(rcplist[i]);
    if(parsestring.size()==0) continue;
    parselist.push_back(parsestring);
  }
  listsize=parselist.size();
}

config::config(std::string filename) {
  config(filename.c_str());
}

std::vector<std::string> config::parser(std::string line) {
  std::vector<std::string> parsestring;
  int start=0;
  int finish=0;
  //  int len=0;
  bool word=false;
  
  for(unsigned int i=0; i<line.size(); i++) {
    if(isspace(line[i])) {
     //    if(line[i]==' ' || line[i]=='\t' || line[i]=='\n') {
      if(word) {
	finish = i;
	parsestring.push_back(line.substr(start,finish-start));	
	word=false;
      }
    }
    else { 
      if(i==(line.size()-1)) {
	finish=i+1;

	if(!word) start=i; 	// bug fix May 24 2004
	parsestring.push_back(line.substr(start,finish-start));
      }
      if(!word) {
	start=i;
	word=true;
      }      
    }
  }
  return parsestring;
}

std::string config::find(std::string cond, std::string type_str) {
  std::string var;
  int rep=0;
  for(int i=0; i<listsize; i++) {
    if(parselist[i].size()!=3) continue;
    std::string type=parselist[i][0];
    std::string tag=parselist[i][1];
    std::string svar=parselist[i][2];
    if(tag==cond) {
      if(type!=type_str) 
	std::cout << "type mismatch for " << tag << std::endl;
      var=svar;
      rep++;
    }
  }
  error(rep,cond);
  return var;
}

double config::getFloat(std::string cond) {
  return atof(find(cond,"float").c_str());
}

double config::getDouble(std::string cond) {
  return atof(find(cond,"double").c_str());
}

int config::getInt(std::string cond) {
  return atoi(find(cond,"int").c_str());
}

bool config::getBool(std::string cond) {
  std::string svar;
  bool var=false;
  svar=find(cond,"bool");
  if(svar=="true") var=true;
  else if(svar=="false") var=false;
  else std::cout << "boolean not set to true or false for " << cond << std::endl;
  return var;
}

std::string config::getString(std::string cond) {
  std::string astring = find(cond,"string");
  while(astring.find('"')!=std::string::npos) {
    astring.replace(astring.find('"'), 1, 1, ' ');
  }
  return astring;
}

std::vector <int> config::getIntArray(std::string cond) {
  std::vector <int> array;
  int rep=0;
  for(int i=0; i<listsize; i++) {
    std::string type=parselist[i][0];
    std::string tag=parselist[i][1];
    if(tag==cond) {
      if(type!="int_array") std::cout << "type mismatch for " << tag << std::endl;
      rep++;
      if(rep>1) break;
      for(unsigned int j=2; j<parselist[i].size(); j++) {
	array.push_back(atoi(parselist[i][j].c_str()));
      }      
    }
  }
  error(rep,cond);
  return array;  
}

std::vector <float> config::getFloatArray(std::string cond) {
  std::vector <float> array;
  int rep=0;
  for(int i=0; i<listsize; i++) {
    std::string type=parselist[i][0];
    std::string tag=parselist[i][1];
    if(tag==cond) {
      if(type!="float_array") std::cout << "type mismatch for " << tag << std::endl;
      rep++;
      if(rep>1) break;
      for(unsigned int j=2; j<parselist[i].size(); j++) {
	array.push_back(atof(parselist[i][j].c_str()));
      }      
    }
  }
  error(rep,cond);
  return array;  
}

std::vector <double> config::getDoubleArray(std::string cond) {
  std::vector <double> array;
  int rep=0;
  for(int i=0; i<listsize; i++) {
    std::string type=parselist[i][0];
    std::string tag=parselist[i][1];
    if(tag==cond) {
      if(type!="double_array") std::cout << "type mismatch for " << tag << std::endl;
      rep++;
      if(rep>1) break;
      for(unsigned int j=2; j<parselist[i].size(); j++) {
	array.push_back(atof(parselist[i][j].c_str()));
      }      
    }
  }
  error(rep,cond);
  return array;  
}

std::vector <std::string> config::getStringArray(std::string cond) {
  std::vector <std::string> array;
  int rep=0;
  for(int i=0; i<listsize; i++) {
    std::string type=parselist[i][0];
    std::string tag=parselist[i][1];
    if(tag==cond) {
      if(type!="string_array") std::cout << "type mismatch for " << tag << std::endl;
      rep++;
      if(rep>1) break;
      for(unsigned int j=2; j<parselist[i].size(); j++) {
        std::string astring = parselist[i].at(j);
        
        while(astring.find('"')!=std::string::npos) {
          astring.replace(astring.find('"'), 1, 1, ' ');
        }

        array.push_back(astring);
      }
    }
  }
  error(rep,cond);
  return array;
}

std::vector<std::vector <int> > config::getIntMatrix(std::string cond) {
  std::vector <int> array;
  std::vector <std::vector <int> > matrix;
  int rep=0;
  int pos=0;
  for(int i=0; i<listsize; i++) {
    std::string type=parselist[i][0];
    std::string tag=parselist[i][1];
    if(tag==cond) {
      if(type!="int_matrix") std::cout << "type mismatch for " << tag << std::endl;
      if(rep<2) {
	for(unsigned int j=2; j<parselist[i].size(); j++) {
	  array.push_back(atoi(parselist[i][j].c_str()));
	}
	matrix.push_back(array);
      }
      pos=i+1;
      rep++;
    }
  }
  array.clear();
  for(int i=pos; i<listsize; i++) {
    std::string type=parselist[i][0];
    if(type!="\\") break;
    for(unsigned int j=1; j<parselist[i].size(); j++) {
      array.push_back(atoi(parselist[i][j].c_str()));
    }      
    matrix.push_back(array);
    array.clear();
  }
  error(rep,cond);
  return matrix;  
}

void config::error(int rep, std::string cond) {
  if(rep>1)
    std::cout << "There is more the one occurrence of \"" << cond << "\" in the config file!" << std::endl;
  if(rep==0) 
    std::cout << "I could not find " << cond << " in config file." << std::endl;
}
