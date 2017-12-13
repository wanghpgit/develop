#include <set>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class TextQuery{
public:
	typedef std::vector<std::string>::size_type line_no;
	void read_file(std::ifstream & is) {
		store_file(is);
		builld_map();
	}

	std::set<line_no>  run_query(const std::string & word) const {
		map_word_cit cit = m_word_map.find(word);
		if (m_word_map.end() != cit) {
			return cit->second;
		} else {
			return std::set<line_no>();
		}
	}

	std::string text_line(line_no no){
		if ((no > 0) && (no <= m_lines_of_text.size()+1)){
			return m_lines_of_text[no+1];
		} else {
			return std::string();
		}
	}

private:
	typedef std::vector<std::string>					vector_line;
	typedef std::map<std::string, std::set<line_no> >	map_word;
	typedef map_word::const_iterator					map_word_cit;

	void store_file(std::ifstream & is) {
		std::string line;
		while (is.good() && getline(is, line)) {
			m_lines_of_text.push_back(line);
		}
	}

	void builld_map(){
		for (vector_line::size_type i=0; i!=m_lines_of_text.size(); ++i){
			std::stringstream strFormat(m_lines_of_text[i]);
			std::string word;
			while (strFormat >> word) {
				m_word_map[word].insert(i);
			}
		}
	}

	vector_line					m_lines_of_text;
	map_word					m_word_map;

};

class Query;
class Query_base{
	friend class Query;
protected:
	typedef TextQuery::line_no line_no;
	virtual ~Query_base(){}

private:
		virtual std::set<line_no> eval(const TextQuery &) const = 0;
		virtual std::ostream & display(std::ostream & os = std::cout) const = 0;
};

class Query {
	friend Query operator~(const Query &);
	friend Query operator|(const Query &, const Query &);
	friend Query operator&(const Query &, const Query &);

public:
	Query(const std::string & str);
	Query(const Query & rh): m_pQ(rh.m_pQ), m_pUse(rh.m_pUse) {
		++(*m_pUse);
	}

	~Query() {
		derc_use();
	}

	std::set<TextQuery::line_no> eval(const TextQuery & t) const {
		return m_pQ->eval(t);
	}

	std::ostream & display(std::ostream & os) {
		return m_pQ->display(os);
	}

private:
	Query(Query_base * pQuery) : m_pQ(pQuery), m_pUse(new std::size_t(1)){}
	void derc_use() {
		if ( (--*m_pUse) == 0) {
			delete m_pQ, m_pQ = NULL;
			delete m_pUse, m_pUse = NULL;
		}
	}


	Query_base *		m_pQ;
	std::size_t	*		m_pUse;
};

class  WordQuery : public Query_base {
	friend class Query;
	WordQuery(const std::string & str): m_query_word(str){}

	std::set<line_no> eval(const TextQuery & t) const {
		return t.run_query(m_query_word);
	}

	std::ostream & display(std::ostream & os) {
		return os << m_query_word;
	}

	std::string  m_query_word;
};

class NotQuery : public Query_base {
	friend Query operator~ (const Query &);
	NotQuery(Query q): m_query(q){}

	std::set<line_no> eval(const TextQuery & rf) const{
		std::set<line_no> has_val = m_query.eval(rf);
		std::set<line_no> ret_lines;
		for (line_no n=0; n!=rf.size(); ++n) {
			if (has_val.find(n) == has_val.end()) {
				ret_lines.insert(n);
			}
		}
		return ret_lines;
	}
	
	std::ostream & display(std::ostream & os /* = std::cout */){
		return os << "~(" << m_query << ")";
	}

	const Query m_query;
};

class BingQuery : public Query_base {
	BingQuery(Query left, Query right, std::string op): m_left(left), m_right(right), m_oper(op){}

	std::ostream & display(std::ostream & os /* = std::cout */){
		return os << "(" << m_left << " " << m_oper << " " << m_right << ")";
	}

	const Query m_left, m_right;
	std::string	m_oper;
};

class AndQuery : public BingQuery {
	friend class Query operator& (const Query &, const Query &);

	AndQuery(Query left, Query right): BingQuery(left, right, "&"){}
	
	std::set<line_no> eval(const TextQuery & rf) const{
		set<line_no> left = m_left.eval(rf),  right = m_right.eval(rf);
		set<line_no> ret_lines;
		set_intersection(left.begin(), left.end(), 	right.begin(), right.end(),  inserter(ret_lines, ret_lines.begin()));
		return ret_lines;
	}
};

class OrQuery : public BingQuery {
	friend class Query;

	AndQuery(Query left, Query right): BingQuery(left, right, "|"){}

	std::set<line_no> eval(const TextQuery & rf) const {
		set<line_no> right = m_right.eval(rf), ret_lines = m_left.eval(rf);
		ret_lines.insert(right.begin(), right.end());
		return ret_lines;
	}
};

inline Query operator& (const Query & lhs, const Query & lhr) {
	return new AndQuery(lhs, lhr);
}

inline Query operator| (const Query & lhs, const Query & lhr) {
	return new AndQuery(lhs, lhr);
}

inline Query operator~ (const Query & oper) {
	return new NotQuery(oper);
}

inline std::ostream & operator << (std::ostream & os, const Query & q){
	return q.display(os);
}

int main() {

	return 0;
}