function FamilyRideCheck( meindex, petindex,pethaveindex)
	local petNo = char.getInt( petindex, "ԭͼ���")
	for i=1,table.getn(ridepet) do
		if petNo == ridepet[i] then
			return 1
		end
	end
	return 0
end

function FreeRideQuery(charaindex)
	local petNo = 0
	local token = "".. table.getn(ridepet)
	for i=1,table.getn(ridepet) do
		token = token .. "|"..petname[i].."|"..CheckPetRideByPetNo( charaindex, ridepet[i]).."|"..ridepet[i]
	end
	lssproto.windows(charaindex, 1023, 0, 1, char.getWorkInt( npcindex, "����"), token)
end

function CheckPetRideByPetNo( meindex, petNo)
	local playerid = getNOindex(char.getInt( meindex, "ԭͼ���"))

	if char.getInt( meindex, "ͼ���") == 101178 then
		if petNo == 101177 then
			return 101179
		end
	elseif char.getInt( meindex, "ͼ���") == 101177 then
		if petNo == 101178 then
			return 101179
		end
	elseif playerid > 0 then
		for i = 1, table.getn(petlist) do
			if petNo == petlist[i] then
				if other.DataAndData(char.getInt(meindex, "֤�����"), i - 1) ~= 0 then
					return 1
				end
			end
		end

		for i = 1, table.getn(PlayerColor[1]) do
			if PlayerColor[playerid][i] == char.getInt( meindex, "ԭͼ���") or (char.getInt( meindex, "ԭͼ���") >= 100700 and char.getInt( meindex, "ԭͼ���") < 108400) then
				if petNo == CommonPetList[i] then
					return 1
				end
			end
		end

		for i = 1, table.getn(CommonPetList1) do
			if char.getInt( meindex, "ԭͼ���") == CommonPetList1[i][2] and petNo == CommonPetList1[i][3] then
				return 1
			end
		end


		for i = 1, table.getn(Pointpetlist) do
			if petNo == Pointpetlist[i][1] then
				floorid = char.getWorkInt( meindex, "�����ͼ")
				for j = 1, table.getn(Pointpetlist[i][2]) do
					if Pointpetlist[i][2][j] == floorid then
						if char.getInt( meindex, "�����λ") > 0 and char.getInt( meindex, "�����λ") ~= 2 then
							if char.getInt( meindex, "��������") == Pointpetlist[i][3] then
								if PointRideNoList[playerid][i] > -1 then
									return 1
								end
							end
						end
					end
				end
			end
		end
		for i = 1, table.getn(EquipagePetlist) do
			if petNo == EquipagePetlist[i][1] then
				for j = 0, 5 do
					itemindex = char.getItemIndex(meindex, j)
					if itemindex > -1 then
						if item.getChar(itemindex, "���ʹ���") == "INSLAY" or other.getString(item.getChar(itemindex, "���ʹ���"), " ", 1) == "INSLAY"  then
							local field = {"", "", "", "", ""}
							local data = item.getChar(itemindex, "��Ƕ����")

							field[1] = other.getString(data, "|", 1)
							field[2] = other.getString(data, "|", 2)
							field[3] = other.getString(data, "|", 3)
							field[4] = other.getString(data, "|", 4)
							field[5] = other.getString(data, "|", 5)
							for k = 1, table.getn(field) do
								if field[k] == EquipagePetlist[i][2] then
									return 1
								end
							end
						end
					end
				end
			end
		end
	end
	return 0

end

function Loop(meindex)
	if char.getWorkInt(meindex,"NPC��ʱ7") == char.getInt(meindex, "ͼ���") then
		char.setInt(meindex, "ͼ���", 102201)
		char.ToAroundChar(meindex)
		char.setInt(meindex, "ѭ���¼�ʱ��", 300)
	else
		char.setInt(meindex, "ͼ���", char.getWorkInt(meindex,"NPC��ʱ7"))
		char.ToAroundChar(meindex)
		char.setInt(meindex, "ѭ���¼�ʱ��", 0)
		char.delFunctionPointer(meindex, "ѭ���¼�")
		char.setWorkInt(meindex,"NPC��ʱ7",0)
	end
end

function FamilyRideFunction( meindex, petindex,pethaveindex)
	if char.getInt( petindex, "ת��") >0 then
		char.TalkToCli(meindex, -1, "��ϵͳ��ʾ������[" .. char.getChar(petindex, "����") .. "]�Ѿ�ת���������޷���ˡ�", "��ɫ")
		return 0
	end

	local playerid = getNOindex(char.getInt( meindex, "ԭͼ���"))

	local petNo = char.getInt( petindex, "ԭͼ���")
	if char.getInt( meindex, "ͼ���") == 101178 then
		if petNo == 101177 then
			char.setWorkInt(meindex,"NPC��ʱ7",101179)
			char.setFunctionPointer(meindex, "ѭ���¼�", "Loop", "")
			char.setInt(meindex, "ѭ���¼�ʱ��", 1000)
			return 101179
		end
	elseif char.getInt( meindex, "ͼ���") == 101177 then
		if petNo == 101178 then
			char.setWorkInt(meindex,"NPC��ʱ7",101179)
			char.setFunctionPointer(meindex, "ѭ���¼�", "Loop", "")
			char.setInt(meindex, "ѭ���¼�ʱ��", 1000)
			return 101179
		end
	elseif playerid > 0 then
		for i = 1, table.getn(petlist) do
			if petNo == petlist[i] then
				if other.DataAndData(char.getInt(meindex, "֤�����"), i - 1) ~= 0 then
					char.setWorkInt(meindex,"NPC��ʱ7",NewRideNoList[playerid][i])
					char.setFunctionPointer(meindex, "ѭ���¼�", "Loop", "")
					char.setInt(meindex, "ѭ���¼�ʱ��", 1000)
					return NewRideNoList[playerid][i]
				end
			end
		end

		for i = 1, table.getn(PlayerColor[1]) do
			if PlayerColor[playerid][i] == char.getInt( meindex, "ԭͼ���") or (char.getInt( meindex, "ԭͼ���") >= 100700 and char.getInt( meindex, "ԭͼ���") < 108400) then
				if petNo == CommonPetList[i] then
					char.setWorkInt(meindex,"NPC��ʱ7",CommonRideNoList[playerid][i])
					char.setFunctionPointer(meindex, "ѭ���¼�", "Loop", "")
					char.setInt(meindex, "ѭ���¼�ʱ��", 1000)
					return CommonRideNoList[playerid][i]
				end
			end
		end

		for i = 1, table.getn(CommonPetList1) do
			if char.getInt( meindex, "ԭͼ���") == CommonPetList1[i][2] and petNo == CommonPetList1[i][3] then
				char.setWorkInt(meindex,"NPC��ʱ7",CommonPetList1[i][1])
				char.setFunctionPointer(meindex, "ѭ���¼�", "Loop", "")
				char.setInt(meindex, "ѭ���¼�ʱ��", 1000)
				return CommonPetList1[i][1]
			end
		end


		for i = 1, table.getn(Pointpetlist) do
			if petNo == Pointpetlist[i][1] then
				floorid = char.getWorkInt( meindex, "�����ͼ")
				for j = 1, table.getn(Pointpetlist[i][2]) do
					if Pointpetlist[i][2][j] == floorid then
						if char.getInt( meindex, "�����λ") > 0 and char.getInt( meindex, "�����λ") ~= 2 then
							if char.getInt( meindex, "��������") == Pointpetlist[i][3] then
								if PointRideNoList[playerid][i] > -1 then
									char.setWorkInt(meindex,"NPC��ʱ7",PointRideNoList[playerid][i])
									char.setFunctionPointer(meindex, "ѭ���¼�", "Loop", "")
									char.setInt(meindex, "ѭ���¼�ʱ��", 1000)
									return PointRideNoList[playerid][i]
								end
							end
						end
					end
				end
			end
		end
		for i = 1, table.getn(EquipagePetlist) do
			if petNo == EquipagePetlist[i][1] then
				for j = 0, 5 do
					itemindex = char.getItemIndex(meindex, j)
					if itemindex > -1 then
						if item.getChar(itemindex, "���ʹ���") == "INSLAY" or other.getString(item.getChar(itemindex, "���ʹ���"), " ", 1) == "INSLAY"  then
							local field = {"", "", "", "", ""}
							local data = item.getChar(itemindex, "��Ƕ����")

							field[1] = other.getString(data, "|", 1)
							field[2] = other.getString(data, "|", 2)
							field[3] = other.getString(data, "|", 3)
							field[4] = other.getString(data, "|", 4)
							field[5] = other.getString(data, "|", 5)
							for k = 1, table.getn(field) do
								if field[k] == EquipagePetlist[i][2] then
									char.setWorkInt(meindex,"NPC��ʱ7",EquipageRideNoList[playerid][i])
									char.setFunctionPointer(meindex, "ѭ���¼�", "Loop", "")
									char.setInt(meindex, "ѭ���¼�ʱ��", 1000)

									return EquipageRideNoList[playerid][i]
								end
							end
						end
					end
				end
			end
		end
	end
	return 0
end



function getNOindex( baseNo)
	if baseNo >= 100000 and baseNo < 100240 then
		metamo = baseNo - 100000
		for i = 1, 12 do
			if metamo >=  (i-2) * 20 and metamo <  i * 20 then
				return i;
			end
		end
	elseif baseNo >= 100700 and baseNo < 100820 then
		metamo = baseNo - 100700
		for i = 1, 12 do
			if metamo >=  (i-2) * 10 and metamo <  i * 10 then
				return i;
			end
		end
	elseif baseNo >= 100400 and baseNo < 100410 then
		return 13;
	end
	return -1
end

function data()
	ridepet = {100329,    100327,     100330,     100328,    100351,    100352,    100353,     100354,     100396,    100374, 			100358, 			100362,		 100279, 			100288, 		100283, 			100346,		 100310, 		100372, 		100373,		 101532, 101576,100907}
	petname = {"��³��˹","��³��","�������³","��³����","�����˹","������˹","�������˹","˹���˹","�����˹","����������", "���Ħ��˹", "������˹", "����³��", "������˹", "����³��˹", "������", "�����", "�����˹", "�Ͷ�����", "ʷ��³", "�޶����","����Ͽ�"}

	 --˵����, ����00, ����01, ���02, �칷03, ����04, �̼�05, ����06, ����07, ����08, �챩09, ˮ��10, ˮ��11, �׻�12,����Ͽ�13
	petlist = {100374, 100358, 100362, 100279, 100288, 100283, 100346, 100310, -1, -1, 101532, 101576, 100872, 100907}

					--˵����, ����00, ����01, ���02, �칷03, ����04, �̼�05, ����06, ����07, ����08, �챩09, ˮ��10, ˮ��11, �׻�12, ��13
	NewRideNoList = {{101305, 101306, 101307, 101308, 101309, 101310, 101311, 101312, 101008, 101009, 101978, 101989, 104025, 103266}
									,{101313, 101314, 101315, 101316, 101317, 101318, 101319, 101320, 101018, 101019, 101986, 101988, 104026, 103262}
									,{101321, 101322, 101323, 101324, 101325, 101326, 101327, 101328, 101028, 101029, 101975, 101965, 104027, 103263}
									,{101329, 101330, 101331, 101332, 101333, 101334, 101335, 101336, 101038, 101039, 101976, 101966, 104028, 103264}
									,{101337, 101338, 101339, 101340, 101341, 101342, 101343, 101344, 101048, 101049, 101984, 101973, 104029, 103272}
									,{101345, 101346, 101347, 101348, 101349, 101350, 101351, 101352, 101058, 101059, 101985, 101974, 104030, 103273}
									,{101353, 101354, 101355, 101356, 101357, 101358, 101359, 101360, 101068, 101069, 101977, 101967, 104031, 103265}
									,{101361, 101362, 101363, 101364, 101365, 101366, 101367, 101368, 101078, 101079, 101979, 101968, 104032, 103267}
									,{101369, 101370, 101371, 101372, 101373, 101374, 101375, 101376, 101088, 101089, 101981, 101970, 104033, 103269}
									,{101377, 101378, 101379, 101380, 101381, 101382, 101383, 101384, 101098, 101099, 101980, 101969, 104034, 103268}
									,{101385, 101386, 101387, 101388, 101389, 101390, 101391, 101392, 101108, 101109, 101983, 101972, 104035, 103271}
									,{101393, 101394, 101395, 101396, 101397, 101398, 101399, 101400, 101118, 101119, 101982, 101971, 104036, 103270}
									}

	PlayerColor = {{ 100000, 100005, 100010, 100015}
								,{ 100025, 100030, 100035, 100020}
								,{ 100055, 100050, 100045, 100040}
								,{ 100060, 100065, 100070, 100075}
								,{ 100095, 100085, 100090, 100080}
								,{ 100100, 100115, 100110, 100105}
								,{ 100135, 100120, 100125, 100130}
								,{ 100145, 100140, 100150, 100155}
								,{ 100165, 100170, 100160, 100175}
								,{ 100190, 100195, 100185, 100180}
								,{ 100200, 100210, 100215, 100205}
								,{ 100230, 100225, 100220, 100235}
								}


										 --  �컢,   �̻�,   ��,   �ƻ�
	CommonPetList   =   {100329, 100327, 100330, 100328}
	CommonRideNoList = {{101004, 101005, 101006, 101007}
										 ,{101015, 101016, 101017, 101014}
										 ,{101027, 101026, 101025, 101024}
										 ,{101034, 101035, 101036, 101037}
										 ,{101047, 101045, 101046, 101044}
										 ,{101054, 101057, 101056, 101055}
										 ,{101067, 101064, 101065, 101066}
										 ,{101075, 101074, 101076, 101077}
										 ,{101085, 101086, 101084, 101087}
										 ,{101096, 101097, 101095, 101094}
										 ,{101104, 101106, 101107, 101105}
										 ,{101116, 101115, 101114, 101117}
										 }
	CommonPetList1  = {{ 101000, 100000, 100352}
										,{ 101001, 100005, 100352}
										,{ 101002, 100010, 100352}
										,{ 101003, 100015, 100352}
										,{ 101000, 100700, 100352}
										,{ 101000, 100705, 100352}

										,{ 101010, 100020, 100396}
										,{ 101011, 100025, 100396}
										,{ 101012, 100030, 100396}
										,{ 101013, 100035, 100396}
										,{ 101010, 100710, 100396}
										,{ 101010, 100715, 100396}

										,{ 101020, 100040, 100351}
										,{ 101021, 100045, 100351}
										,{ 101022, 100050, 100351}
										,{ 101023, 100055, 100351}
										,{ 101020, 100720, 100351}
										,{ 101020, 100725, 100351}

										,{ 101030, 100060, 100353}
										,{ 101031, 100065, 100353}
										,{ 101032, 100070, 100353}
										,{ 101033, 100075, 100353}
										,{ 101030, 100730, 100353}
										,{ 101030, 100735, 100353}

										,{ 101040, 100080, 100396}
										,{ 101041, 100085, 100396}
										,{ 101042, 100090, 100396}
										,{ 101043, 100095, 100396}
										,{ 101040, 100740, 100396}
										,{ 101040, 100745, 100396}

										,{ 101050, 100100, 100353}
										,{ 101051, 100105, 100353}
										,{ 101052, 100110, 100353}
										,{ 101053, 100115, 100353}
										,{ 101050, 100750, 100353}
										,{ 101050, 100755, 100353}

										,{ 101060, 100120, 100354}
										,{ 101061, 100125, 100354}
										,{ 101062, 100130, 100354}
										,{ 101063, 100135, 100354}
										,{ 101060, 100760, 100354}
										,{ 101060, 100765, 100354}

										,{ 101070, 100140, 100354}
										,{ 101071, 100145, 100354}
										,{ 101072, 100150, 100354}
										,{ 101073, 100155, 100354}
										,{ 101070, 100770, 100354}
										,{ 101070, 100775, 100354}

										,{ 101080, 100160, 100352}
										,{ 101081, 100165, 100352}
										,{ 101082, 100170, 100352}
										,{ 101083, 100175, 100352}
										,{ 101080, 100780, 100352}
										,{ 101080, 100785, 100352}

										,{ 101090, 100180, 100351}
										,{ 101091, 100185, 100351}
										,{ 101092, 100190, 100351}
										,{ 101093, 100195, 100351}
										,{ 101090, 100790, 100351}
										,{ 101090, 100795, 100351}

										,{ 101100, 100200, 100353}
										,{ 101101, 100205, 100353}
										,{ 101102, 100210, 100353}
										,{ 101103, 100215, 100353}
										,{ 101100, 100800, 100353}
										,{ 101100, 100805, 100353}

										,{ 101110, 100220, 100396}
										,{ 101111, 100225, 100396}
										,{ 101112, 100230, 100396}
										,{ 101113, 100235, 100396}
										,{ 101110, 100810, 100396}
										,{ 101110, 100815, 100396}
										}

	Pointpetlist = {--����
								 {100372, {1041, 2031, 3031, 4031, 5031}, 0}
								  --�챩
								,{100373, {1041, 2031, 3031, 4031, 5031}, 1}
								}
	                  --����,  --�챩
	PointRideNoList = {{101008, 101009}	--С����
					  ,{101018, 101019}	--������
					  ,{101028, 101029}	--�����к�
					  ,{101038, 101039}	--���
					  ,{101048, 101049}	--��Ƥ��
					  ,{101058, 101059}	--���
					  ,{101068, 101069}	--С����
					  ,{101078, 101079}	--��Ƥ��
					  ,{101088, 101089}	--ñ����
					  ,{101098, 101099}	--�̷�������
					  ,{101108, 101109}	--����Ů
					  ,{101118, 101119}	--����
										}

	EquipagePetlist = {--ʯ����
										 {100307, "Cb"}
										 --������
										 ,{100348, "Ca"}
										 --���
										 ,{100360, "Cc"}
										 --����
										 ,{104124, "Cd"}
										 --�ڼ�
										 ,{100904, "Ce"}
										 --�켦
										 ,{100370, "Cf"}
										 --�Ҽ�
										 ,{100369, "Cg"}
										}
	                  		--ʯ����,������, ���  , ����  , �ڼ�  , �켦  , �Ҽ�
	EquipageRideNoList = {{104100, 104112, 104137, 104125, 100444, 100425, 100424}	--С����
											 ,{104101, 104113, 104138, 104126, 100444, 100425, 100424}	--������
											 ,{104102, 104114, 104139, 104127, 100444, 100425, 100424}	--�����к�
											 ,{104103, 104115, 104140, 104128, 100444, 100425, 100424}	--���
											 ,{104104, 104116, 104141, 104129, 100444, 100425, 100424}	--��Ƥ��
											 ,{104105, 104117, 104142, 104130, 100444, 100425, 100424}	--���
											 ,{104106, 104118, 104143, 104131, 100444, 100425, 100424}	--С����
											 ,{104107, 104119, 104144, 104132, 100444, 100425, 100424}	--��Ƥ��
											 ,{104108, 104120, 104145, 104133, 100444, 100425, 100424}	--ñ����
											 ,{104109, 104121, 104146, 104134, 100444, 100425, 100424}	--�̷�������
											 ,{104110, 104122, 104147, 104135, 100444, 100425, 100424}	--����Ů
											 ,{104111, 104123, 104148, 104136, 100444, 100425, 100424}	--����
											 }
end


function Create(name, metamo, floor, x, y, dir)
	--����NPC(NPC���֣�ͼ��ţ���ͼ�ţ�����X������Y�������)������һ��NPC����
	npcindex = npc.CreateNpc(name, metamo, floor, x, y, dir)
end

function main()
	data()
	Create("��ѯ����NPC��ʱ", 100000, 777, 20, 20, 4)
end
