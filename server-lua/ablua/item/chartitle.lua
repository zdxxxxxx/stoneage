function titleuse(itemindex, charaindex, toindex, haveitemindex)
	local id = other.atoi(item.getChar(itemindex, "�ֶ�"))
	if GetCharNewTitleMode(charaindex,id -1)== 1 then
		char.TalkToCli(charaindex, -1, "����ӵ�иóƺţ�", "��ɫ")
		return
	end
	local titlename = item.getChar(itemindex,"��ʾ��")
	char.DelItem(charaindex, haveitemindex)
	char.setCharNewTitleMode(charaindex,id -1)
--	char.setInt(charaindex, "�ƺ�״̬2", 0)
---	char.setInt(charaindex, "�ƺ�״̬3", 0)

	char.TalkToCli(charaindex, -1, "��ϲ�����³ƺ�"..titlename.."��", "��ɫ")
	char.talkToServer(-1, "[��������]��ϲ���� " .. char.getChar(charaindex, "����") .. " ���"..titlename.."�ƺ�", "��ɫ")
	local param = {charaindex}
	other.CallFunction("TitleListSend", "data/ablua/freechartitle.lua", param)
end

function GetCharNewTitleMode(charaindex,id)
	if id< 32 then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬1"),id)~= 0 then
			return 1
		end
	elseif id< 64 then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬2"),id-32)~= 0 then
			return 1
		end
	elseif id< 96 then
		if other.DataAndData(char.getInt(charaindex,"�ƺ�״̬3"),id-64)~= 0 then
			return 1
		end
	end
	return 0
end

function main()
	item.addLUAListFunction( "ITEM_TITLE", "titleuse", "")
end
