��¼���߼�¼={};


function WindowTalked ( meindex, charaindex, seqno, select, data)
	if seqno == 1 then
		local ����λ�� = other.atoi(data);
		if ����λ�� >=1 and ����λ��<=4 then
			local �������� = char.getCharPet(charaindex, ����λ��-1);
			if char.check(��������) == 1 then
				��¼���߼�¼[charaindex][3] = ��������;
				local �Ի����� = "�Ƿ��� "..char.getChar(��������,"����") .." ѧϰ����Ч��"
				lssproto.windows(charaindex, "�Ի���", "ȷ��|ȡ��", 2,  char.getWorkInt( npcindex, "����"),�Ի����� )
			end
		end
	elseif seqno == 2 then
		if select == 1 then
			local ������ЧID = other.atoi(item.getChar(��¼���߼�¼[charaindex][1], "�ֶ�"))
			char.setInt(��¼���߼�¼[charaindex][3],"������Ч",������ЧID);
			char.DelItem(charaindex, ��¼���߼�¼[charaindex][2]);
			char.TalkToCli(charaindex, -1, "���ĳ���"..char.getChar(��¼���߼�¼[charaindex][3],"����").."ϰ�ù�����Ч,��ȥ����ɣ�", "��ɫ")
			��¼���߼�¼[charaindex]=nil;
		end
	end
end

function attackeffect(itemindex, charaindex, toindex, haveitemindex)
	��¼���߼�¼[charaindex]={};
	��¼���߼�¼[charaindex][1]=itemindex;
	��¼���߼�¼[charaindex][2]=haveitemindex;
	lssproto.windows(charaindex, "�����", "ȷ��", 1,  char.getWorkInt( npcindex, "����"), "")
end

function Create(name, metamo, floor, x, y, dir)
	--����NPC(NPC���֣�ͼ��ţ���ͼ�ţ�����X������Y�������)������һ��NPC����
	npcindex = npc.CreateNpc(name, metamo, floor, x, y, dir)
	char.setFunctionPointer(npcindex, "�����¼�", "WindowTalked", "")
end


function data()
end

function main()
	Create("������ЧNPC",100000,777,20,20,3);

	item.addLUAListFunction( "ITEM_ATTACKEFFECT", "attackeffect", "")
end
