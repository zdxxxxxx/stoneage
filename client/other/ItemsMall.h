#ifndef _ITEMS_MALL_H__
#define _ITEMS_MALL_H__

#include <map>


#define ITEMS_MALL_FILE_NAME "shopsellitems.bin"


typedef struct ITEMSMALL
{
	int id;                  //id
	int itemId;              //��Ʒ/����ID
	char itemName[128];      //��Ʒ/��������
	int itemFlag;            //����Ʒ���ǳ����ƷΪ��1������Ϊ��2��
	int bmpNo;               //����/��Ʒ���ͼƬ
	int currencyType;        //����Щ��Ʒ����Ļ�������
	int currencyNum;        //�������Ʒ����Ľ�������������ô˻��ҹ�������ֶ�Ϊ��-1
	int recordType;          //��Ʒ���ࣺ1��ʾ�����ϼۣ�4��ʾ������Ʒ��
	char szLine1[128];
	char szLine2[128];
	char szLine3[128];
}*LP_ITEMSMALL;

//�����̳���ز�����ʾ��
class ItemsMall
{
public:
	static ItemsMall& MakeInstance();
	static void Destroy();

	void SetItemDatas(char* pszData);
	
	int GetItemsNums();
	int GetPetsNums();
	int GetAllItemSize() { return g_mapItemsMall.size(); }

	ITEMSMALL* GetItemByIndex(int nIndex);
	ITEMSMALL* GetItemByType(int nIndex, int nType, int recordType = 1);   //recodTypeĬ��ֵΪ1��ʾ�����ϼ���Ʒ��4Ϊ������Ʒ
	ITEMSMALL* GetAllItemsByIndex(int nIndex, int nType);
	ITEMSMALL* GetAllPetsByIndex(int nIndex, int nType);
	ITEMSMALL* GetAllOtherItemsByIndex(int nIndex, int nType);

	int GetCurrentItemNum(int nCurrencyType, int nType);

	//int GetAllLatestNum();  //�����ϼۻ�Ʒ����
	//int GetAllItemsNum(); //װ����Ʒ����
	//int GetAllPetsNum(); //�����Ʒ����
	//int GetAllOthersNum(); //������Ʒ����

protected:
	ItemsMall(void){}
	ItemsMall(ItemsMall&);

	~ItemsMall(void){}

	static ItemsMall* instance;
	static std::map<int, ITEMSMALL> g_mapItemsMall;  

	//static int m_nAllLatestNum;
	//static int m_nAllItemsNum;
	//static int m_nAllPetsNum;
	//static int m_nAllOthersNum;
};

#endif