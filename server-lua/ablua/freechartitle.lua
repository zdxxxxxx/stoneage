function GetCharNewTitleNo(charaindex,id)
	if char.check(charaindex)~=1 then
		return 0
	end
	if id< 32 then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬1"),id)~= 0 then
			return titleinfo[id+1][2]
		end
	elseif id< 64 then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬2"),id-32)~= 0 then
			return titleinfo[id+1][2]
		end
	elseif id< 96 then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬3"),id-64)~= 0 then
			return titleinfo[id+1][2]
		end
	end
	return 0
end

function GetCharNewTitleString(charaindex,id)
	if char.check(charaindex)~=1 then
		return 0
	end
	if id< 32 then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬1"),id)~= 0 then
			return titleinfo[id+1][2]
		end
	elseif id< 64 then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬2"),id-32)~= 0 then
			return titleinfo[id+1][2]
		end
	elseif id< 96 then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬3"),id-64)~= 0 then
			return titleinfo[id+1][2]
		end
	end
	return ""
end


function GetCharNewTitleName(charaindex,id)
	if char.check(charaindex)~=1 then
		return 0
	end

	if id< 32 then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬1"),id)~= 0 then
			return titleinfo[id+1][1]
		end
	elseif (id >=32 and id< 64) then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬2"),id-32)~= 0 then
			return titleinfo[id+1][1]
		end
	elseif(id >=64 and id< 96) then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬3"),id-64)~= 0 then
			return titleinfo[id+1][1]
		end
	end
	return ""
end

function GetCharNewTitleMode(charaindex,id)
	if char.check(charaindex)~=1 then
		return 0
	end

	if id< 32 then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬1"),id)~= 0 then
			return 1
		end
	elseif (id >=32 and id< 64) then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬2"),id-32)~= 0 then
			return 1
		end
	elseif(id >=64 and id< 96) then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬3"),id-64)~= 0 then
			return 1
		end
	end
	return 0
end

function TitleListSend(charaindex)
	if char.check(charaindex)~=1 then
		return
	end
	local Nodata=""
	local jiedata=""
	local indexdata=""
	local num=0
	local fd = char.getFd(charaindex)
	for i = 1,96 do
		if GetCharNewTitleMode(charaindex,i-1)>0 then
			num = num + 1
--			print(num, GetCharNewTitleMode(charaindex,i-1))
			Nodata = Nodata..GetCharNewTitleString(charaindex,i-1).."|"
			jiedata = jiedata..GetCharNewTitleName(charaindex,i-1).."|"
			indexdata = indexdata..(i-1).."|"
		end
	end
	local data = string.format("%d|%d#@%s#@%s#@%s#@",char.getInt(charaindex,"���ֳƺ�"),num,Nodata,jiedata,indexdata)
	lssproto.CharTitleSend(fd,data)
--	print("CharTitleSend���ͳɹ���"..data.."\n")
end

function GetTtileBuff(id)
	if id >96 or id < 1 then
		return 0,0,0,0
	end
	return titleinfo[id][3],titleinfo[id][4],titleinfo[id][5],titleinfo[id][6]
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
	data();
end
