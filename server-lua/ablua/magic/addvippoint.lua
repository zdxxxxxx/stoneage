function addvip(charaindex, data)
	local value = other.atoi(other.getString(data, " ", 1))
	local cdkey = other.getString(data, " ", 2)
	if cdkey == "" then
		local oldvalue = sasql.getVipPoint(charaindex)
		sasql.setVipPoint(charaindex, sasql.getVipPoint(charaindex) + value)
		char.TalkToCli(charaindex, -1, "�ɹ����˺�[" .. char.getChar(charaindex, "�˺�") .. "]����[" .. char.getChar(charaindex, "����") .. "]���˻�Ա��ԭ" .. oldvalue .. "����Ϊ" .. sasql.getVipPoint(charaindex), "��ɫ")
	else
		local maxplayer = char.getPlayerMaxNum()
		local cimelia = other.atoi(data)
		for i = 0, maxplayer - 1 do
			if char.check(i) == 1 then
				if char.getChar(i, "�˺�") ==  cdkey then
					local oldvalue = sasql.getVipPoint(i)
					sasql.setVipPoint(i, sasql.getVipPoint(i) + value)
					char.TalkToCli(charaindex, -1, "�ɹ����˺�[" .. char.getChar(i, "�˺�") .. "]����[" .. char.getChar(i, "����") .. "]���˻�Ա��ԭ" .. oldvalue .. "����Ϊ" .. sasql.getVipPoint(i), "��ɫ")
					char.TalkToCli(i, -1, "GM[" .. char.getChar(charaindex, "����") .. "]����ĸ��˻�Ա��ԭ" .. oldvalue .. "����Ϊ" .. sasql.getVipPoint(i), "��ɫ")
					return
				end
			end
		end
	end
end

function main()
	minute = 0
	magic.addLUAListFunction("��Ա��", "addvip", "", 1, "[]")
end

