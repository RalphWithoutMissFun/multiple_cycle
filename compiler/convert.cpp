#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
using namespace std;
map<string, string> data;
map<string, int> mode;
map<string, string> binary_convert;
int map_len = 32;
int map_len2 = 17;
void print_vec(vector<int> v){
	//cout << v.size();
  int len = v.size();
	for (int index =0 ; index < len ; index++){
		cout << v[index];
		if (index % 8 == 7)
		 cout << " ";
	}
	cout << endl;
	/*
	vector<int>::iterator ite = v.begin();
	for (ite ; ite != v.end() ; ite++)
		cout << *ite ;
	cout << endl;
	*/
}

/*
  * read from a file named code.txt and output the corresponding
  * machine code to the file convert.txt
  * use the basic knowledge of map
  */
/*
 * part 0 , read the map data and store it in the map
 */
void initialize(){
   fstream file ;
   file.open("map_data.txt");
   string str1, str2;
   for (int index = 0 ; index < map_len ; index++){
   	 file >> str1 >> str2;
   	 data.insert(pair<string,string>(str1, str2));
   }
   file.close();
   file.open("map_data2.txt");
   int num;
   for (int index = 0 ; index < map_len2; index++){
   	file >> str1 >> num;
    mode.insert(pair<string,int>(str1, num));
   }
   file.close();
   file.open("map_data3.txt");
   for (int index = 0 ; index < map_len ; index++){
   	 file >> str1 >> str2;
   	 binary_convert.insert(pair<string,string>(str1, str2));
   }
   file.close();
}
/*
 * part1 : convert the opcode to the binary to vector of binary code
 * 1 and 0
 * and use the least byte to signal the type of the instruction
 * R, S and I
 */
vector<int> convert_opcode(string target, int len){
	vector<int> opcode;
	for (int index = 0 ; index < len; index++)
		opcode.push_back(0);
  string str = data[target];
	//cout << target << "----" << endl;
  int leng = str.size();
	//cout << "debug " << leng << "  " << str << endl;
  for (int index = 0 ; index < leng ; index++)
  {
  	opcode[index] = str[index] - '0';
  }
    return opcode;
}
/*
 * part2 : convert the number to given buyes
 */
vector<int> convert_num(string str, int bytes, int zero_sign){

	if (str[0] == '0' && str.size() >= 2 && str[1] == 'x'){
		vector<int> binary_display ;
		for (int index = 2 ; index < 9 ; index++){
			string temp;
			temp = temp + str[index];
			//cout << temp << endl;
			int l = (index == 8) ? 2:4;
			for (int index = 0 ; index < l ; index++){
				int num = binary_convert[temp][index] - '0';
				binary_display.push_back(num);
			}
		}
		return binary_display;
	}
    istringstream is(str);
    int num;

    is >> num;
    vector<int> res;

    //cout << num <<" " << bytes<< endl;
    int sign = (num >= 0 || zero_sign == 0)? 0:1;
    for (int index = 0 ; index < bytes ; index++){
    	res.push_back(sign);
    }
    if (num < 0){
    	int radix = pow(2, bytes);
    	num = radix + num;
    }
    int back = num, index = 0;

    while (back != 0 && index < bytes){
    	if (back % 2 == 1){
    		//cout << index << endl;
    		res[index] = 1;
    		num -= 1;
    	}else{
    		res[index] = 0;
    	}
    	index++;
    	back /= 2;
    //	cout << res[index] << endl;
    }
    //reverse(res.begin(), res.end());
    //cout << res.size() << endl;
/*
    for (int index = 0 ; index < res.size() ; index++)
    	cout << res[index] ;
    cout << endl;*/
    //print_vec(res);
    return res;

}
string pre_processing(string str){

	string::size_type pos = 0;
    pos=str.find(',', pos);
    while ((pos != string::npos))
    {
        str.replace(pos, 1, " ");
        pos=str.find(',', (pos+1));
    }

    pos=str.find('(', pos);
    if ((pos != string::npos))
    {
        str.replace(pos, 1, " ");
    }
    pos=str.find(')', pos);

    if ((pos != string::npos))
    {
        str.replace(pos, 1, " ");
    }

    return str;
}
vector<int> parser(string str){

	istringstream in(str);
	string code;
	in >> code;
	int final_mode = mode[code];
	vector<int> opcode = convert_opcode(code,6);
	//print_vec(opcode);
	//cout << "--------" << opcode << endl;
	string rd;
	string rs;
	string rt;
	string imm;
	string sa;
	string addr;

	vector<int> res(opcode.begin(), opcode.end());
	//cout << "hello" << endl;
	//res.insert(res.end(), opcode.begin(), opcode.end());
	//cout << "hello" << endl;
	if (final_mode == 0){
		in >> rd >> rs >> rt;
		vector<int>frd = convert_opcode(rd,5);
		vector<int>frs = convert_opcode(rs,5);
		vector<int>frt = convert_opcode(rt,5);
		res.insert(res.end(), frs.begin(), frs.end());
		res.insert(res.end(), frt.begin(), frt.end());
		res.insert(res.end(), frd.begin(), frd.end());
		for (int index = 0 ; index < 11 ; index++)
			res.push_back(0);
	}
	if (final_mode == 1){
		in >> rt >> rs >> imm;
		//cout << rt << " " << rs << " " << imm << endl;

		vector<int> frt = convert_opcode(rt, 5);
		vector<int> frs = convert_opcode(rs, 5);

		vector<int> fimm = convert_num(imm, 16, 1);
		//print_vec(fimm);
		//print_vec(frt);
		//print_vec(frs);
		//print_vec(fimm);
		res.insert(res.end(), frs.begin(), frs.end());
		res.insert(res.end(), frt.begin(), frt.end());
		res.insert(res.end(), fimm.begin(), fimm.end());

	}
	if (final_mode == 2){

		in >> rd >> rt >> sa;
		vector<int> frd = convert_opcode(rd, 5);
		vector<int> frt = convert_opcode(rt, 5);
		vector<int> fsa = convert_num(sa, 6, 0);
		for (int index = 0 ; index < 5 ; index++)
			res.push_back(0);
		res.insert(res.end(), frt.begin(), frt.end());
		res.insert(res.end(), frd.begin(), frd.end());
		res.insert(res.end(), fsa.begin(), fsa.end());
		for (int index = 0 ; index < 5 ; index++)
			res.push_back(0);
	}
	if (final_mode == 3){
		in >> rs >> rt >> imm;
		vector<int> frs = convert_opcode(rs, 5);
		vector<int> frt = convert_opcode(rt, 5);
		vector<int> fimm = convert_num(imm, 16, 1);
		res.insert(res.end(), frs.begin(), frs.end());
		res.insert(res.end(), frt.begin(), frt.end());
		res.insert(res.end(), fimm.begin(), fimm.end());
	}
	if (final_mode == 4){
        in >> rt >> imm >> rs;
        vector<int> frt = convert_opcode(rt, 5);
        vector<int> fimm = convert_num(imm, 16, 1);
        vector<int> frs = convert_opcode(rs, 5);
        res.insert(res.end(), frs.begin(), frs.end());
        res.insert(res.end(), frt.begin(), frt.end());
        res.insert(res.end(), fimm.begin(), fimm.end());
	}
	if (final_mode == 5){
		in >> rs >> imm;
		vector<int> frs = convert_opcode(rs, 5);
		vector<int> fimm = convert_num(imm, 16,1);
		res.insert(res.end(), frs.begin(), frs.end());
		for (int index = 0 ; index < 5 ; index++)
			res.push_back(0);
		res.insert(res.end(), fimm.begin(), fimm.end());
	}
	if (final_mode == 6){
		in >> addr;
		vector<int> faddr = convert_num(addr, 28, 1);
		res.insert(res.end(), faddr.begin(), faddr.end());
	}
	if (final_mode == 7){
		in >> rs;
		vector<int> frs = convert_opcode(rs, 5);
		res.insert(res.end(), frs.begin(), frs.end());
		for (int index = 0 ; index < 21 ; index++)
			res.push_back(0);
	}
	if (final_mode == 8){
		for (int index = 0 ; index < 26 ; index++)
			res.push_back(0);
	}
	return res;
}

int main(){
	initialize();
	fstream file ;
    file.open("data.txt");
    for (int index = 0 ; index < 20 ; index++){
    	string str;
    	getline(file, str);
    	//cout << str << endl;
    	//cout << pre_processing(str) << endl;
    	print_vec(parser(pre_processing(str)));
 	}
    file.close();
}
