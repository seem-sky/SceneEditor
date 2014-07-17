#ifndef _JSONX_H_
#define _JSONX_H_

//
//  JsonX.h
//  test-json
//
//  Created by ��Ϸ���� on 14-6-6.
//
//

#ifndef test_json_JsonX_h
#define test_json_JsonX_h

#include "cocos2d.h"

#include "rapidjson.h"
#include "document.h"
#include "writer.h"
#include "prettywriter.h"
#include "stringbuffer.h"

//rapidjsonǳ��װ
class JsonX
{
public:
	typedef rapidjson::Value Value;

	JsonX();
	JsonX(std::string file_name);

	bool read(std::string file_name);

	//���ַ�����ȡjson����
	bool readFromString(const char * json_str);

	//json���ݴ洢Ϊ�ļ�
	bool saveToFile(std::string file_name);

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
	void clear() { doc.Clear(); }

	//���ҳ�Ա
	Value& find(const char *name);

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

#endif