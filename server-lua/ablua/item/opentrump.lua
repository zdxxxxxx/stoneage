function opentrump(itemindex, charaindex, toindex, haveitemindex)

	if char.getInt(charaindex,"����״̬")>0 then
		char.TalkToCli(charaindex, -1,"���Ѿ������˷���װ����!", "��ɫ")
		return
	end
	char.DelItem(charaindex, haveitemindex)
	char.setInt(charaindex,"����״̬",1);
	char.UpCahrData(charaindex,"p");
	char.TalkToCli(charaindex, -1,"���ķ���װ�����ѳɹ�����!", "��ɫ")
end

function main()
	item.addLUAListFunction( "ITEM_OPENTRUMP", "opentrump", "")
end
