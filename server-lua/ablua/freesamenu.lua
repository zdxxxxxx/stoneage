function FreeSaMenu(meindex, index)
--	char.TalkToCli(meindex, -1, char.getWorkInt(meindex,"NPC��ʱ3"), "��ɫ")
	local սģ = char.getWorkInt(meindex,"ս��");
	parameter = {meindex}
	if index == 11 then
		if սģ ==0 then
			char.Encounter(meindex)
			char.TalkToCli(meindex, -1, "����ԭ�����У�", "��ɫ")
		else
			char.TalkToCli(meindex, -1, "���Ѿ�ս���У�", "��ɫ")
		end
	elseif index == 12 then
		char.ClearEncounter(meindex);
		char.TalkToCli(meindex, -1, "�ر�ԭ�����У�", "��ɫ")
	elseif index == 13 then
		if սģ == 0 then
			other.CallFunction("CheckTalked", "data/ablua/npc/check/check.lua", parameter)
		end
	--	other.CallFunction("ShowHead", "data/ablua/npc/vipshop/vipshop.lua", parameter)
	elseif index == 14 then
			other.CallFunction("QueryMissionTalked", "data/ablua/npc/querymission/querymission.lua", parameter)
	elseif index == 16 then
		other.CallFunction("FreeDataInfoShow", "data/ablua/freedatainfoshow.lua", parameter)
	elseif index == 17 then
		lssproto.SaMenu(meindex, index, "http://www.stoneage.es")
	elseif index == 18 then
		lssproto.SaMenu(meindex, index, "http://www.stoneage.es/forum.php")
	elseif index == 19 then
		lssproto.SaMenu(meindex, index, "http://www.stoneage.es/pay.php")
	elseif index == 20 then
		char.setWorkInt(meindex,"NPC��ʱ3",1)
		other.CallFunction("OnlineService", "data/ablua/npc/onlineservice/onlineservice.lua", parameter)
	elseif index == 21 then
		lssproto.SaMenu(meindex, index, "http://www.stoneage.es/forum.php?mod=viewthread&tid=54&extra=page%3D1")
	elseif index == 22 then
		lssproto.SaMenu(meindex, index, "http://www.stoneage.es/bbs/forum.php?mod=viewthread&tid=226")
	elseif index == 30 then--�л��ӳ�
		if char.getWorkInt(meindex, "���") == 1 then
			ת�ƶ���[meindex]={};
			local ��Ա�� = "";
			local ����=0;
			for i=1,4 do
				local ��Ա���� = char.getWorkInt(meindex, "��Ա" .. i+1)
				if char.check(��Ա����) == 1 then
					���� = ���� + 1;
					ת�ƶ���[meindex][����] = ��Ա����;
					��Ա�� = string.format("%s        %s\n",��Ա��,char.getChar(ת�ƶ���[meindex][����],"����"));
				end
			end
			local ���� = "1 						��ѡ���µĶӳ�\n"..��Ա��;
			lssproto.windows(meindex, 2, 8, 0, char.getWorkInt( npcindex, "����"), ����)
		else
			char.TalkToCli(meindex, -1, "�ù�����Ҫ�ӳ�Ȩ��", "��ɫ")
		end
	else
		char.TalkToCli(meindex, -1, "��δ����", "��ɫ")
	end
end


function WindowTalked( NPC����, �������, ���, ��ť, ����)
	if ��� == 0 then
		local ѡ�� = tonumber(����);
		if ѡ�� > 0 and ѡ�� < 5 then
			char.DischargeParty(�������,0);
			char.JoinParty(ת�ƶ���[�������][ѡ��],�������,0);
			char.TalkToCli(ת�ƶ���[�������][ѡ��], -1, "���Ķ��ѡ�"..char.getChar(�������,"����").."���Ѷӳ�Ȩ����������", "��ɫ")
			char.TalkToCli(�������, -1, "���ɹ��Ѷӳ�Ȩ���������ѡ�"..char.getChar(ת�ƶ���[�������][ѡ��],"����").."��", "��ɫ");
			for i =1,table.getn(ת�ƶ���[�������]) do
				if i ~= ѡ�� then
					char.JoinParty(ת�ƶ���[�������][ѡ��],ת�ƶ���[�������][i],0);
				end
			end
			char.Encounter(ת�ƶ���[�������][ѡ��])
		end
	end
end

function Create(name, metamo, floor, x, y, dir)
	npcindex = npc.CreateNpc(name, metamo, floor, x, y, dir)
	char.setFunctionPointer(npcindex, "�����¼�", "WindowTalked", "")
end

function data()
 ת�ƶ���={};
--[[

11 ԭ������
12 ȡ��ԭ��
13 ֧Ʊ����
14 �����ѯ
16 ������Ϣ
20 ����ʹ��
21 ������
22 ���ְ���


]]


end

function main()
	data()
	Create("��ѯ����NPC��ʱ", 100000, 777, 20, 21, 4);
end

