#include <iostream>
#include <fstream>
#include <string>
#include <cstring> // strtok strstr
#include <stdio.h> // strtok strstr
#include <stdlib.h> // strtok strstr
#include <math.h>  // sqrt pow
#include <utility> // pair
#include <vector>

using namespace std;

//======================================
//parse each line and save the results
//======================================
//key words
//string kw_tot_fl_t = "total flush time = ";
//string kw_bf_fl_t = "time before clflush = ";
//string kw_af_fl_t = "time after clflush = ";
string kw_thp_cc = "htp_cc : throughput = ";
string kw_thp_bp = "htp_bp : throughput = ";

//results
//uint64_t tot_tot_fl_t = 0;
//vector<int> v_tot_fl_t;
//uint64_t tot_bf_fl_t = 0;
//vector<int> v_bf_fl_t;
//uint64_t tot_af_fl_t = 0;
//vector<int> v_af_fl_t;
double tot_thp_cc = 0.0;
vector<double> v_thp_cc;
double tot_thp_bp = 0.0;
vector<double> v_thp_bp;

//======================================
//get the target string number
//======================================
////get the total flush time
//string get_tot_fl_t(string line) {
	//////convert string into c str
	////const char * cline = line.c_str();
	////const char * ws = " \t\n";

	//char * token;
	//token = strtok((char *)line.c_str(), " \t\n"); //"total"
	//token = strtok(NULL, " \t\n"); //"flush"
	//token = strtok(NULL, " \t\n"); //"time"
	//token = strtok(NULL, " \t\n"); // "="
	//token = strtok(NULL, " \t\n"); // num

	////convert char* to string
	//string res(token);

	////cout << "res = " << res << endl;

	//return res;
//}

////get cycles before clflush
//string get_bf_fl_t(string line) {
	//////convert string into c str
	////const char * cline = line.c_str();
	////const char * ws = " \t\n";

	//char * token;
	//token = strtok((char *)line.c_str(), " \t\n"); //"total"
	//token = strtok(NULL, " \t\n"); //"flush"
	//token = strtok(NULL, " \t\n"); //"time"
	//token = strtok(NULL, " \t\n"); // "="
	//token = strtok(NULL, " \t\n"); // num

	////convert char* to string
	//string res(token);

	////cout << "res = " << res << endl;

	//return res;
//}

////get cycles after clflush
//string get_afbf_fl_t(string line) {
	//////convert string into c str
	////const char * cline = line.c_str();
	////const char * ws = " \t\n";

	//char * token;
	//token = strtok((char *)line.c_str(), " \t\n"); //"i"
	//token = strtok(NULL, " \t\n"); //"="
	//token = strtok(NULL, " \t\n"); //"num"
	//token = strtok(NULL, " \t\n"); //"time"
	//token = strtok(NULL, " \t\n"); //"before/after"
	//token = strtok(NULL, " \t\n"); //"clflush"
	//token = strtok(NULL, " \t\n"); // "="
	//token = strtok(NULL, " \t\n"); // num

	////convert char* to string
	//string res(token);

	////cout << "res = " << res << endl;

	//return res;
//}

//get the total throughput
string get_tot_thp(string line) {
	char * token;
	token = strtok((char *)line.c_str(), " \t\n"); //"htp_cc/htp_bp"
	token = strtok(NULL, " \t\n"); //":"
	token = strtok(NULL, " \t\n"); // "throughput"
	token = strtok(NULL, " \t\n"); // "="
	token = strtok(NULL, " \t\n"); // num

	//convert char* to string
	string res(token);

	//cout << "res = " << res << endl;

	return res;
}

//convert string number to double
double myStod(string n_str) {
	double n = 0.0;
	bool af_pt = false; //after the point
	double tens = 1.0;
	for (size_t i = 0; i < n_str.length(); ++i) {
		if (n_str[i] == '.') {
			af_pt = true;
			continue;
		}
		if (af_pt) 
			tens *= 10.0;

		n = n * 10 + n_str[i] - '0';
	}
	n /= tens;

	//cout << "n = " << n << endl;
	return n;
}




//======================================
//calculate mean and standard deviation
//======================================
pair<double, double> calc(vector<double> vec, double tot) {
	pair<double, double> res(0.0, 0.0);
	if (vec.size() == 0) return res;

	//calculate the mean
	double mean = tot / (double)vec.size();

	//cout << "mean = " << mean << endl;

	//calculate the standard deviation
	double sd;
	double sum = 0.0;
	//sum up
	for (size_t i = 0; i < vec.size(); ++i) {
		sum += (vec[i] - mean) * (vec[i] - mean);
		//cout << sum << endl;
	}
	//cout << "sum = " << sum << endl;
	//divide and sqrt
	sd = sqrt(sum / (double)vec.size());

	//cout << "sd = " << sd << endl;

	res.first = mean;
	res.second = sd;

	return res;
}

	

int main() {
	//======================================
	//open file
	//======================================
	const string filename = "stat.txt";
	ifstream infile(filename);
	if (!infile.is_open()) {
		cout << "cannot open the file\n";
		exit(EXIT_FAILURE);
	}




	string line, str_num;
	double num;
	while (getline(infile, line)) {
		//cout << line << endl;
		//find target line for tot flush time
		if (strstr(line.c_str(), kw_thp_cc.c_str())) {
			//get the string num
			str_num = get_tot_thp(line);
			//convert string to int
			num = myStod(str_num);
			//save the result in 
			v_thp_cc.push_back(num);
			//sum the results
			tot_thp_cc += num;
		}
		//find target line for before flush time
		if (strstr(line.c_str(), kw_thp_bp.c_str())) {
			//get the string num
			str_num = get_tot_thp(line);
			//convert string to int
			num = stod(str_num);
			//save the result in 
			v_thp_bp.push_back(num);
			//sum the results
			tot_thp_bp += num;
		}
		////find target line for after flush time
		//if (strstr(line.c_str(), kw_af_fl_t.c_str())) {
			////get the string num
			//str_num = get_afbf_fl_t(line);
			////convert string to int
			//num = stoi(str_num);
			////save the result in 
			//v_af_fl_t.push_back(num);
			////sum the results
			//tot_af_fl_t += num;
		//}
	}

	//for (size_t i = 0; i < v_tot_fl_t.size(); ++i) 
		//cout << v_tot_fl_t[i] << endl;
			
	////calculate
	//pair<double, double> tot_fl_t = calc(v_tot_fl_t, tot_tot_fl_t);
	////show results
	//cout << "total flush time:\n";
	//cout << "mean = " << tot_fl_t.first << ",\t";
	//cout << "sd = " << tot_fl_t.second << endl;

	////calculate
	//pair<double, double> bf_fl_t = calc(v_bf_fl_t, tot_bf_fl_t);
	////show results
	//cout << "before flush time:\n";
	//cout << "mean = " << bf_fl_t.first << ",\t";
	//cout << "sd = " << bf_fl_t.second << endl;

	////calculate
	//pair<double, double> af_fl_t = calc(v_af_fl_t, tot_af_fl_t);
	////show results
	//cout << "after flush time:\n";
	//cout << "mean = " << af_fl_t.first << ",\t";
	//cout << "sd = " << af_fl_t.second << endl;
	
	//calculate
	pair<double, double> p_thp_cc = calc(v_thp_cc, tot_thp_cc);
	//show results
	cout << "cache log throughput:\n";
	cout << "mean = " << p_thp_cc.first << ",\t";
	cout << "sd = " << p_thp_cc.second << endl;

	//calculate
	pair<double, double> p_thp_bp = calc(v_thp_bp, tot_thp_bp);
	//show results
	cout << "bypass log throughput:\n";
	cout << "mean = " << p_thp_bp.first << ",\t";
	cout << "sd = " << p_thp_bp.second << endl;
	
	return 0;
}
