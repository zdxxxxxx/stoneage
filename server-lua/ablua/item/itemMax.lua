function useitemmax(itemindex, charaindex, toindex, haveitemindex)

	local data = other.atoi(item.getChar(itemindex, "�ֶ�"))
	if (char.getInt(charaindex,"������״̬")~=2 and data == 3)
		or (char.getInt(charaindex,"������״̬")~=0 and data == 2) then
		char.TalkToCli(charaindex, -1,"�������Ӧ����չ��������չ������", "��ɫ")
		return
	end
	if char.getInt(charaindex,"������״̬") == 2 then
		char.setInt(charaindex,"������״̬",6);
		char.DelItem(charaindex, haveitemindex)
		char.TalkToCli(charaindex, -1,"��ϲ������������������!", "��ɫ")
		char.UpCahrData(charaindex,"p");
	elseif char.getInt(charaindex,"������״̬") == 6 then
		char.TalkToCli(charaindex, -1,"���Ѿ�ȫ��������!", "��ɫ")
	else
		char.DelItem(charaindex, haveitemindex)
		char.setInt(charaindex,"������״̬",2);
		char.TalkToCli(charaindex, -1,"��ϲ�������ڶ���������!", "��ɫ")
		char.UpCahrData(charaindex,"p");
	end
end

function main()
	item.addLUAListFunction( "ITEM_USEITEMMAX", "useitemmax", "")
end
