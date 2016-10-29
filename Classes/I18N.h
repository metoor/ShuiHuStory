#ifndef __I18N_H__
#define __I18N_H__

#include <unordered_map>

class I18N{
public:
	~I18N();

	static I18N* getInstance();
	
	//加载字符文件
	void loadStringFile(std::string fileName);

	//通过键值获取中文字符串---注意：调用本方法前，请确保已经调用过loadStringFile
	const std::string* getStringByKey(std::string key);

	void dstoryInstance();

private:
	I18N();

private:
	std::unordered_map<std::string, std::string>* _map;

	static I18N* _instance;
};

#endif // I18N_H_
