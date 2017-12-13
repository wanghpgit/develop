#include <set>
#include <string>
#include <iostream>

class cfolder;
class cmessage{
public:
	cmessage(const std::string & str = "");
	cmessage(const cmessage & that);
	cmessage & operator= (const cmessage & that) ;
	~cmessage();
	void save(cfolder & floder);
	void remove(cfolder & floder);

private:
	void put_msg_into_floder();
	void remove_msg_into_floder();
	void swap(cmessage & that);

	std::string   		m_msg;
	std::set<cfolder*>	m_folders;
};
class cfolder {
public:
	cfolder();
	~cfolder();
	void add_msg(cmessage & msg);
	void rem_msg(cmessage & msg);

private:
	std::set<cmessage*> m_messages;
};

cmessage::cmessage(const std::string & str):m_msg(str){}
cmessage::cmessage(const cmessage & that): m_msg(that.m_msg), m_folders(that.m_folders){
	put_msg_into_floder();
}

cmessage & cmessage::operator= (const cmessage & that) {
	if (this != &that) {
		cmessage tmp(that);
		this->swap(tmp);
	}
}

cmessage::~cmessage(){
	remove_msg_into_floder();
}

void cmessage::save(cfolder & floder){
	m_folders.insert(&floder);
}

void cmessage::remove(cfolder & floder){
	m_folders.erase(m_folders.find(&floder));
}

void cmessage::put_msg_into_floder(){
	for (std::set<cfolder*>::iterator it=m_folders.begin(); it!=m_folders.end(); ++it)	{
		(*it)->add_msg(*this);
	}
}

void cmessage::remove_msg_into_floder() {
	for (std::set<cfolder*>::iterator it=m_folders.begin(); it!=m_folders.end(); ++it)	{
		(*it)->rem_msg(*this);
	}
}

void cmessage::swap(cmessage & that){
	this->remove_msg_into_floder();
	that.remove_msg_into_floder();
	m_msg.swap(that.m_msg);
	m_folders.swap(that.m_folders);
	this->put_msg_into_floder();
	that.put_msg_into_floder();
}

cfolder::cfolder(){}
cfolder::~cfolder(){}
void cfolder::add_msg(cmessage & msg) {
	m_messages.insert(&msg);
}

void cfolder::rem_msg(cmessage & msg) {
	m_messages.erase(m_messages.find(&msg));
}


int main() {

	return 0;
}
