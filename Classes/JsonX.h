#ifndef _JSONX_H_
#define _JSONX_H_

#include "cocos2d.h"

#include "rapidjson.h"
#include "document.h"
#include "writer.h"
#include "prettywriter.h"
#include "stringbuffer.h"

static const char *kTypeNames[] = { "NULL", "False", "True", "Object", "Array", "String", "Number" };

//rapidjsonǳ��װ
class JsonX
{
public:
	typedef rapidjson::Value Value;

	JsonX();
	JsonX(JsonX &other);
	JsonX(std::string file_name);
	JsonX(const char *json_str);

	bool read(std::string file_name);

	//���ַ�����ȡjson����
	bool readFromString(const char * json_str);

	//json���ݴ洢Ϊ�ļ�
	bool saveToFile(std::string file_name, bool pretty = true);

	//�����ֵ��
	Value& insert(const char *key, Value &value);
	Value& insert(Value &key, Value &value);
	template <typename T>
	Value& insert(const char *key, T value)
	{
		Value k(key, doc.GetAllocator());
		Value v(value, doc.GetAllocator());
		return insert(k, v);
	}
	//�������
	Value& insertArray(const char *key);
	Value& insertArray(Value &key);
	//ɾ������
	bool remove(const char *key);
	//�������
	void clear();
	//�����С
	int size();

	//���ҳ�Ա
	Value& find(const char *name);
	bool has(const char *key);

	//��ȡ�ĵ�����
	rapidjson::Document& getDocument();

	//�±����
	Value& operator[](const char *name);
	const Value& operator[](const char *name) const;

	//��ȡ�ڴ������
	rapidjson::Document::AllocatorType& getAllocator();

private:
	rapidjson::Document doc;
};

#endif