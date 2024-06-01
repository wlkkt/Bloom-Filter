#pragma once


struct HashFuncBKDR
{
	// BKDR
	size_t operator()(const string& s)
	{
		size_t hash = 0;
		for (auto ch : s)
		{
			hash *= 131;
			hash += ch;
		}

		return hash;
	}
};

struct HashFuncAP
{
	// AP
	size_t operator()(const string& s)
	{
		size_t hash = 0;
		for (size_t i = 0; i < s.size(); i++)
		{
			if ((i & 1) == 0) // ż��λ�ַ�
			{
				hash ^= ((hash << 7) ^ (s[i]) ^ (hash >> 3));
			}
			else              // ����λ�ַ�
			{
				hash ^= (~((hash << 11) ^ (s[i]) ^ (hash >> 5)));
			}
		}

		return hash;
	}
};

struct HashFuncDJB
{
	// DJB
	size_t operator()(const string& s)
	{
		size_t hash = 5381;
		for (auto ch : s)
		{
			hash = hash * 33 ^ ch;
		}

		return hash;
	}
};

template<size_t N,
	class K = string,
	class Hash1 = HashFuncBKDR,
	class Hash2 = HashFuncAP,
	class Hash3 = HashFuncDJB>
class BloomFilter
{
public:
	void Set(const K& key)
	{
		size_t hash1 = Hash1()(key) % M;
		size_t hash2 = Hash2()(key) % M;
		size_t hash3 = Hash3()(key) % M;

		_bs->set(hash1);
		_bs->set(hash2);
		_bs->set(hash3);
	}

	bool Test(const K& key)
	{
		size_t hash1 = Hash1()(key) % M;
		if (_bs->test(hash1) == false)
			return false;

		size_t hash2 = Hash2()(key) % M;
		if (_bs->test(hash2) == false)
			return false;

		size_t hash3 = Hash3()(key) % M;
		if (_bs->test(hash3) == false)
			return false;

		return true; // ��������(�п���3��λ���Ǹ����˳�ͻ�ģ���������)
	}

private:
	static const size_t M = 10 * N;
	//bitset<M> _bs;
	std::bitset<M>* _bs = new std::bitset<M>;
};

//���Ժ���
void TestBloomFilter1()
{
	string strs[] = { "�ٶ�","�ֽ�","��Ѷ" };//���������ַ���
	BloomFilter<10> bf;//����һ��ʮ��λ��С��λͼ
	for (auto& s : strs)//ѭ�����������ַ���ӳ�䵽λͼ��
	{
		bf.Set(s);
	}

	//���������ַ�����λͼ���Ƿ��Ѿ�����
	for (auto& s : strs)
	{
		cout << bf.Test(s) << endl;
	}

	//�������ַ������в���'a'Ȼ���ٴ��ж���Щ�¹��ɵ��ַ����Ƿ������λͼ��
	for (auto& s : strs)
	{
		cout << bf.Test(s + 'a') << endl;
	}
}