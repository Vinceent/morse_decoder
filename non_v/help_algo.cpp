#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include<fstream>
#include<algorithm>
#include<iterator>
#include<sstream>
#include<cstring>
#include<set>
#include<chrono>
#include<array>
#include<queue>
#define POS_CASE
using namespace std;

bool can_split(vector<string>&, string::iterator&, string::iterator&, string::iterator&);
bool can_split_index(const vector<string>&, size_t, size_t, size_t, const string&);
void make_first_valid(const string&, string&);
void split(string, size_t, bool, vector<string>&, vector<string>&, size_t&);
bool if_one_space_diff(const string& s1, const string& s2);
vector<vector<int>> make_graph(const vector<string>& valids);
vector<vector<int>> BFS_alg(vector<vector<int>>& graph);

size_t fill_valid(string& undecoded, vector<string>& valid_lines, vector<string>& morse_table)
{
	size_t counter =0;
	string valid;
	make_first_valid(undecoded, valid);
	valid_lines.push_back(valid);
	++counter;
	split(valid, 0, false, morse_table, valid_lines, counter);
	return counter;
}

void split(string val, size_t pos_space, bool remove_space, vector<string>& morse_table, vector<string>& valid_lines, size_t &counter)
{
	if(remove_space) {
		val.erase(pos_space, 1);
		valid_lines.push_back(val);
		++counter;
	}
	//it_space = find(it_space+1, val.end(), ' '); //addition if we don't do split, [first,last) anyways
	pos_space = val.find(' ', pos_space+1);
	if(pos_space == string::npos) {return;}

	#ifdef ITERATOR_CASE
	string::iterator it_space = val.begin()+pos_space;
	string::reverse_iterator it_prev_space(it_space);
	it_prev_space = find(it_prev_space,val.rend(),' ');
	string::iterator it_prev_past_space = it_prev_space.base();
	string::iterator it_next_space = find(it_space+1, val.end(),' ');
	bool split_able = can_split(morse_table, it_prev_past_space, it_space, it_next_space);
	#endif

	#ifdef POS_CASE
	size_t pos_prev_space = val.rfind(' ', pos_space-1);
	if(pos_prev_space == string::npos) { pos_prev_space = 0;}

	size_t pos_next_space = val.find(' ', pos_space+1);
	if(pos_next_space== string::npos) { pos_next_space =val.size();}
	bool split_able = can_split_index(morse_table, pos_prev_space, pos_space, pos_next_space, val);
	#endif
	if (split_able) {
		split(val, pos_space, true, morse_table, valid_lines, counter);
	}
	split(val, pos_space, false, morse_table, valid_lines, counter);
}

bool can_split(vector<string>& morse_table, string::iterator& it_prev, string::iterator& it_sp,\
	       	string::iterator& it_next)
{
	if(it_sp == it_next) return false;
	return find(morse_table.begin(), morse_table.end(),\
		       	(string(it_prev, it_sp)+ string(it_sp+1, it_next))) != morse_table.end();
}

bool can_split_index( const vector<string>& morse_table, const size_t pos_prev, const size_t pos_sp, const size_t pos_next, const string& str)
{
	if(pos_sp == pos_next) return false;
	cout<<pos_prev<<' '<< pos_sp<<' '<< pos_next<<endl;
	cout<< str.substr(pos_prev+1, pos_sp -pos_prev-1) + str.substr(pos_sp+1, pos_next -pos_sp-1)<<endl;
	return find(morse_table.begin(), morse_table.end(), str.substr(pos_prev+1, pos_sp - pos_prev-1 )  + str.substr(pos_sp+1, pos_next-pos_sp-1)) !=morse_table.end();
}
	
void make_first_valid(const string& coded, string& valid)
{
	bool first = true;
	for(const char &x: coded) {
		if(!first) {
			valid.push_back(' ');
		}
		valid.push_back(x);
		first = false;
	}
}

bool if_one_space_diff(const string& s1, const string& s2)
{
        size_t pos=0;
        while((pos !=min(s1.size(), s2.size())) && s1[pos] == s2[pos]){
                        ++pos;
        }
        if(pos == min(s1.size(), s2.size())) { return false;}
        return (s1[pos] ==' ')?
                (s1.substr(pos+1) == s2.substr(pos)):
                (s1.substr(pos) == s2.substr(pos+1));

}

void clown_fiesta(string str, vector<string>& vault, const set<string>& m_table, vector<vector<string>> &valid_seqs, size_t start_pos = 0)
{
	static size_t code_limit = (*m_table.rbegin()).size()+1;
/*	if(start_pos==str.size()-1) { 
		printf("test3\n");
		valid_seqs[start_pos].push_back(string(1,str.back()));
		return;
	} */
	auto it = str.begin() + start_pos;
	for(size_t offset = 1; (offset!=code_limit) && (str.end() - it>=offset ); ++offset) {
		if(m_table.find(string(it, it+offset))!=m_table.end()) {
			if(it+offset == str.end()) {
				valid_seqs[start_pos].push_back(string(it,it+offset));
				break;
			}
			if(valid_seqs[start_pos+offset].empty()) {
				clown_fiesta(str, vault, m_table, valid_seqs, offset+start_pos);
			}
			for(auto& x:valid_seqs[start_pos+offset]) {
					valid_seqs[start_pos].push_back(string(it,it+offset) +' '+ x);
			}
		} 
	}
}

vector<vector<int>> make_graph(const vector<string>& valids)
{
	size_t valid_count = valids.size();
	vector<vector<int>> graph(valid_count);
	for(size_t raws =0; raws!=valid_count;++raws) {
		for(size_t columns =0; columns!=valid_count; ++columns) {
			graph[raws].push_back( if_one_space_diff(valids[raws], valids[columns]));
		}
	}
	return graph;
}

string form_letters(stringstream& ss,const string& m_str, const vector<pair<char,string>>& m_codes) 
{
	ss.str(m_str);
	string temp_str, ret_str;
	while(ss>>temp_str) {
		auto symb = find_if(m_codes.begin(), m_codes.end(), [&temp_str](const pair<char,string> &p){ return p.second == temp_str;});
		if(symb!=m_codes.end()){
			ret_str.push_back((*symb).first);
		}
	}
	ss.clear();	
	return ret_str;
}


vector<vector<int>> BFS_alg(vector<vector<int>>& graph)
{
	enum {WHITE, GRAY, BLACK};
	vector<vector<int>> spanning_tree(graph.size());
	for(auto& x:spanning_tree) {
		x.assign(graph.size(),0);
	}
	vector<int> colors(graph.size(), WHITE);
	colors[0] = GRAY;
	queue<int> vert_in_progress;
	vert_in_progress.push(0);
	while(!vert_in_progress.empty()) {
		cout<<"size of queue: "<<vert_in_progress.size()<<endl;
		int index = vert_in_progress.front();
		vert_in_progress.pop();
		cout<<"pop. "<<"size of a queue: "<<vert_in_progress.size()<<endl;
		auto it = graph[index].begin();
		while((it = find(it, graph[index].end(), 1))!=graph[index].end()) {
			size_t next_vert_pos = distance(graph[index].begin(), it);
			cout<<"nextvpos: "<<next_vert_pos<<endl;
			++it;
			cout<<"colors: ";
			for_each(colors.begin(), colors.end(), [](int i) { cout<<i<<' ';});
			cout<<endl;
			if (colors[next_vert_pos] == WHITE) {
				
				colors[next_vert_pos] =GRAY;
				spanning_tree[index][next_vert_pos] = spanning_tree[next_vert_pos][index] = 1;
				vert_in_progress.push(next_vert_pos);
			}
		}
		colors[index] = BLACK;
	}	
	return spanning_tree;
}

void outp_vv_int(vector<vector<int>> & vec) 
{
	for(size_t i =0; i!=vec.size();++i) {
		for(size_t j =0; j!=vec[i].size();++j) {
			cout<<vec[i][j]<<' ';
			//cout<<(vec[i][j]?'#':' ');
		}
		cout<<endl;
	}
}



int main()
{
	string inp;
	set<string> morse_storage;
	ifstream morse_file("morse");
	vector<pair<char, string>> letters;
	while(getline(morse_file, inp )) {
		letters.push_back(make_pair(inp[0], inp.substr(2)));
		morse_storage.insert(inp.substr(2));
	}

	morse_file.close();


	ifstream coded_file("code");
	if(!coded_file.is_open()) { cout<<"no file named \'coded\' in directory\n"; return 0;}

	string coded_seq;
	getline(coded_file, coded_seq);
	coded_file.close();
	
	ofstream out_file("valids_help");
	vector<string> valid_strings;
	vector<vector<string>> valid_seqs(coded_seq.size());
	cout<<valid_seqs.size()<<endl;
	auto t1 = std::chrono::high_resolution_clock::now();
	clown_fiesta(coded_seq, valid_strings, morse_storage, valid_seqs);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();
	cout<<"Running time: "<<duration<<endl;
	cout<<"1231\n";
/*	for(auto& a:valid_seqs) {
		for(auto& b:a) {
			cout<<b<<endl;
		}
		cout<<"end of the vec\n";
	}*/
	string buffer;
	const size_t BUF_SIZE = 16000;
	buffer.reserve(BUF_SIZE);
	stringstream sym_buf;
	for(auto it = valid_seqs[0].begin(); it!=valid_seqs[0].end(); ++it)
	{
		out_file<<*it<<"\t\t";
		stringstream let(*it);
		string sym;
		while(let>>sym) {
			for(auto& x:letters){
				if(sym==x.second) {out_file<<std::right<<x.first<<' ';}
			}
		}
		out_file<<endl;
	}

	const size_t valid_count = valid_seqs[0].size();
	cout<<valid_count<<" lines was generated\n";
	//int **graph_one_space =new int[valid_count][valid_count]; 	//	TODO:the data structure is not determined
	vector<vector<int>> graph_one_space = make_graph(valid_seqs[0]);	
	outp_vv_int(graph_one_space);

	vector<vector<int>> spanning_tree = BFS_alg(graph_one_space);
	cout<<endl;
	outp_vv_int(spanning_tree);


/*
	for(auto it = valid_seqs[0].begin(); it!=valid_seqs[0].end(); ++it)
	{
		out_file<<*it<<"\t\t";
		stringstream let(*it);
		string sym;
		while(let>>sym) {
			for(auto& x:letters){
				if(sym==x.second) {out_file<<std::right<<x.first<<' ';}
			}
		}
		out_file<<endl;
	} */
/*	size_t counter = fill_valid(coded_seq, valid_strings, morse_storage);
	cout<<"fill valid is over\n"<<counter<<" strings generated\n";
	for(auto& x: valid_strings){
		out_file<<x<<"\t\t";
		stringstream let(x);
		string sym;
		while(let>>sym) {
			for(auto& x:letters) {
				if(sym==x.second) { out_file<<std::right<< x.first<<' ';}
			/out}
		}
		out_file<<endl;
	}
*/
	return 0;
}

	
