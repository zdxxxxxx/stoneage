function addpay(charaindex, data)
	local value = other.atoi(other.getString(data, " ", 1))
	local cdkey = other.getString(data, " ", 2)
	if cdkey == "" then
		local oldvalue = sasql.getPayPoint(charaindex)
		sasql.setPayPoint(charaindex, sasql.getPayPoint(charaindex) + value)
		char.TalkToCli(charaindex, -1, "�ɹ����˺�[" .. char.getChar(charaindex, "�˺�") .. "]����[" .. char.getChar(charaindex, "����") .. "]���˳�ֵ����ԭ" .. oldvalue .. "����Ϊ" .. sasql.getPayPoint(charaindex), "��ɫ")
	else
		local maxplayer = char.getPlayerMaxNum()
		local cimelia = other.atoi(data)
		for i = 0, maxplayer - 1 do
			if char.check(i) == 1 then
				if char.getChar(i, "�˺�") ==  cdkey then
					local oldvalue = sasql.getPayPoint(i)
					sasql.setPayPoint(i, sasql.getPayPoint(i) + value)
					char.TalkToCli(charaindex, -1, "�ɹ����˺�[" .. char.getChar(i, "�˺�") .. "]����[" .. char.getChar(i, "����") .. "]���˳�ֵ����ԭ" .. oldvalue .. "����Ϊ" .. sasql.getPayPoint(i), "��ɫ")
					char.TalkToCli(i, -1, "GM[" .. char.getChar(charaindex, "����") .. "]����ĸ��˳�ֵ����ԭ" .. oldvalue .. "����Ϊ" .. sasql.getPayPoint(i), "��ɫ")
					return
				end
			end
		end
	end
end

function main()
	minute = 0
	magic.addLUAListFunction("��ֵ����", "addpay", "", 1, "[]")
end

