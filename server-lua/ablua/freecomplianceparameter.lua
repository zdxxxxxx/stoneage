--��luaӦ����װ����Ƕ���ں�C��أ����ӿ�����!
function getIntPart(x)
    if x <= 0 then
       return math.ceil(x);
    end
    if math.ceil(x) == x then
       return math.ceil(x);
    else
       return math.ceil(x) - 1;
    end
end

function GetTtileBuff(id)
	if id >96 or id < 1 then
		return 0,0,0,0
	end
	return titleinfo[id][3],titleinfo[id][4],titleinfo[id][5],titleinfo[id][6],titleinfo[id][7],titleinfo[id][8],titleinfo[id][9],titleinfo[id][10],titleinfo[id][11]
end

function FreeComplianceParameter( charaindex )
	if char.getInt(charaindex, "����") == 3 then
		local data= char.getChar(charaindex,"ף��״̬")
		if data~="" then
			local hp = other.atoi(other.getString(data, "|", 1))
			local at = other.atoi(other.getString(data, "|", 2))
			local df = other.atoi(other.getString(data, "|", 3))
			local dx = other.atoi(other.getString(data, "|", 4))

			char.setWorkInt(charaindex, "���HP", char.getWorkInt(charaindex, "���HP") + hp)
			char.setWorkInt(charaindex, "������", char.getWorkInt(charaindex, "������") + at)
			char.setWorkInt(charaindex, "������", char.getWorkInt(charaindex, "������") + df)
			char.setWorkInt(charaindex, "������", char.getWorkInt(charaindex, "������") + dx)

		end
	end



	if char.getInt(charaindex, "����") == 1 then
		local itemindex =  char.getItemIndex(charaindex,6)
		if itemindex > 0 then
			local data1 = item.getChar(itemindex, "�ֶ�")
			local attrate = other.atoi(other.getString(data1, "|", 1))
			local defrate = other.atoi(other.getString(data1, "|", 2))
			local dexrate = other.atoi(other.getString(data1, "|", 3))
			char.setWorkInt(charaindex, "������", math.floor(char.getWorkInt(charaindex, "������") * (100+attrate)/100))
			char.setWorkInt(charaindex, "������", math.floor(char.getWorkInt(charaindex, "������") * (100+defrate)/100))
			char.setWorkInt(charaindex, "������", math.floor(char.getWorkInt(charaindex, "������") * (100+dexrate)/100))
			char.setWorkInt(charaindex, "���HP", char.getWorkInt(charaindex, "���HP") + 150)
		end

		for i = 1,96 do
			if char.getCharNewTitleMode(charaindex,i-1)== 1 then
				temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9 = GetTtileBuff(i)
				addhp = addhp + temp1
				addat = addat + temp2
				adddf = adddf + temp3
				adddx = adddx + temp4
				addpo = addpo + temp5
				addpa = addpa + temp6
				addsl = addsl + temp7
				addst = addst + temp8
				addco = addco + temp9
			end
		end
		char.setWorkInt(charaindex, "���HP", char.getWorkInt(charaindex, "���HP") + addhp)
		char.setWorkInt(charaindex, "������", char.getWorkInt(charaindex, "������") + addat)
		char.setWorkInt(charaindex, "������", char.getWorkInt(charaindex, "������") + adddf)
		char.setWorkInt(charaindex, "������", char.getWorkInt(charaindex, "������") + adddx)
		char.setWorkInt(charaindex, "��������", addpo)
		char.setWorkInt(charaindex, "�鿹����", addpa)
		char.setWorkInt(charaindex, "˯������", addsl)
		char.setWorkInt(charaindex, "ʯ������", addst)
		char.setWorkInt(charaindex, "�쿹����", addco)

	end

end

function data()
	titleinfo =	{	--���� ��� �� �� �� �� �� �� ˯ ʯ ��
				 {"��صľ��� ��+5",1,0,0,5,0,0,0,0,0,0}
				,{"ˮ�ľ��� HP+20",2,20,0,0,0,0,0,0,0,0}
				,{"���׵ľ��� ��+5",3,0,5,0,0,0,0,0,0,0}
				,{"����ľ��� ��+5",4,0,0,0,5,0,0,0,0,0}
				,{"�������ľ��� ȫ+1",5,1,1,1,1,0,0,0,0,0}
				,{"�ʺ�ľ��� ȫ+2",6,2,2,2,2,0,0,0,0,0}
				,{"����ʹ�� HP+10",7,10,0,0,0,0,0,0,0,0}
				,{"���������� ȫ+2",8,2,2,2,2,0,0,0,0,0}
				,{"·�ߵ���Ҷ ȫ+4",9,4,4,4,4,0,0,0,0,0}
				,{"ˮ���С�� ȫ+6",10,6,6,6,6,0,0,0,0,0}
				,{"��૵ĸ��� ȫ+8",11,8,8,8,8,0,0,0,0,0}
				,{"���ϵ���Ӱ ȫ+10",12,10,10,10,10,0,0,0,0,0}
				,{"���ܵĴ��� ȫ+12",13,12,12,12,12,0,0,0,0,0}
				,{"���ķ��� ȫ+14",14,14,14,14,14,0,0,0,0,0}
				,{"ҡҷ�Ľ��� ȫ+16",15,16,16,16,16,0,0,0,0,0}
				,{"��ϲ�Ĵ��� ȫ+18",16,18,18,18,18,0,0,0,0,0}
				,{"�̺���̫�� ȫ+20",17,20,20,20,20,0,0,0,0,0}
				,{"��η�ļž� ȫ+30",18,30,30,30,30,0,0,0,0,0}
				,{"���`��ӡ�� ��+5",19,0,5,0,0,0,0,0,0,0}
				,{"��ת�ֻ� ��+20",20,0,0,20,0,0,0,0,0,0}
				,{"�����ַ��� ��+5",21,0,5,0,0,0,0,0,0,0}
				,{"�������� ���ҿ���+5",22,0,0,0,0,0,0,0,0,5}
				,{"�ᱦ��� ʯ������+5",23,0,0,0,0,0,0,0,5,0}
				,{"�´��ᱦ�� ˯����+5",24,0,0,0,0,0,0,5,0,0}
				,{"�ϳɴ��� ��+5",25,0,0,0,5,0,0,0,0,0}
				,{"�ϳɸ��� ��+3",26,0,0,0,3,0,0,0,0,0}
				,{"������� ��+5",27,0,5,0,0,0,0,0,0,0}
				,{"������� ��+3",28,0,3,0,0,0,0,0,0,0}
				,{"������� ��+5",29,0,0,5,0,0,0,0,0,0}
				,{"������� ��+3",30,0,0,3,0,0,0,0,0,0}
				,{"����սʿ HP+10",31,10,0,0,0,0,0,0,0,0}
				,{"ʥ����ʿ HP+15",32,15,0,0,0,0,0,0,0,0}
				,{"������˿Ӣ�� HP+20",33,20,0,0,0,0,0,0,0,0}
				,{"��ķ����Ӣ�� HP+25",34,25,0,0,0,0,0,0,0,0}
				,{"������˹ʹ�� HP+30",35,30,0,0,0,0,0,0,0,0}
				,{"��˹��½Ӣ�� HP+100",36,100,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				,{"","",0,0,0,0,0,0,0,0,0}
				}




end

function main()
	data()
end
