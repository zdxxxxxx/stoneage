--��LUA�������ж�ʱ��Ϳ۳�ʱ�仹�����ߺ��¼��һ�������ڼ�����ĸ�����Ϣ!
function getIntPart(x)
    if x <= 0 then
       return math.ceil(x);
    end

    if math.ceil(x) == x then
       x = math.ceil(x);
    else
       x = math.ceil(x) - 1;
    end
    return x;
end

function FreeCharLogout( charindex )
	if char.getWorkInt(charindex, "����") == 1 then
		--if char.getWorkInt(charindex, "��̯") == -1 then
		--	if config.getGameservername() ~= "���ʯ������" and config.getGameservername() ~= "���ʯ������" then
		--		local offtime = other.time() - char.getWorkInt(charindex, "��½ʱ��")
		--		char.setInt(charindex, "����ʱ��", math.max(char.getInt(charindex, "����ʱ��") - offtime / 60), 0)
		--	end
		--end
		other.setLuaPLayerNum(other.getLuaPLayerNum()-1)
	else
		local offtime = other.time() - char.getWorkInt(charindex, "��½ʱ��")
		if char.getInt(charindex, "�ȼ�") == 140 then
			local onlinetime = char.getInt(charindex, "����ʱ��") - char.getWorkInt(charindex, "��½ʱ��")
			if char.getInt(charindex, "ת��") == 5 then
				char.setInt(charindex, "����ʱ��", char.getInt(charindex, "����ʱ��") + getIntPart(onlinetime / 2) )
			elseif char.getInt(charindex, "ת��") == 6 then
				char.setInt(charindex, "����ʱ��", char.getInt(charindex, "����ʱ��") + onlinetime)
			end
		end
	end
	
	--if config.getGameservername() == "���ʯ������" then
	--	token = "DELETE FROM `OnlinePlayer` "
	--										.. " WHERE `cdkey` = '" .. char.getChar(charindex, "�˺�") .. "'"
	--	ret = sasql.query(token)
	--end
	
	token = "SELECT * FROM `CSAinfo` "
										.. " WHERE `cdkey` = '" .. char.getChar(charindex, "�˺�") .. "'"
										.. " AND `saveindex` = '" .. char.getInt(charindex, "�浵") .. "'"
	ret = sasql.query(token)
	if ret == 1 then
		sasql.free_result()
		sasql.store_result()
		num = sasql.num_rows()
		if num > 0 then
			token = "UPDATE `CSAinfo` SET "
						.. "`name` = '" .. char.getChar(charindex, "����") .. "',"
						.. "`famename` = '"  .. char.getChar(charindex, "����") .. "', "
						.. "`level` = '"  .. char.getInt(charindex, "�ȼ�") .. "',"
						.. "`trans` = '"  .. char.getInt(charindex, "ת��") .. "',"
						.. "`alive` = '"  .. char.getInt(charindex, "����") .. "',"
						.. "`fame` = '"  .. char.getInt(charindex, "����") .. "',"
						.. "`TITLE_LV` = '"  .. 0 .. "',"
						.. "`DUELBATTLECOUNT` = '" .. char.getInt(charindex, "PK����") .. "',"
						.. "`DUELWINCOUNT` = '" .. char.getInt(charindex, "PKӮ��") .. "',"
						.. "`DUELLOSECOUNT` = '" .. char.getInt(charindex, "PK����") .. "',"
						.. "`DUELSTWINCOUNT` = '" .. char.getInt(charindex, "PK��ʤ") .. "',"
						.. "`DUELSTLOSECOUNT` = '" .. char.getInt(charindex, "PK����") .. "',"
						.. "`DUELMAXSTWINCOUNT` = '" .. char.getInt(charindex, "PK�����ʤ") .. "',"
						.. "`vip` = '"  .. char.getInt(charindex, "��Ա") .. "',"
						.. "`time` = NOW()"
						.. " WHERE `cdkey` = '" .. char.getChar(charindex, "�˺�") .. "'"
						.. " AND `saveindex` = '" .. char.getInt(charindex, "�浵") .. "'"
			ret = sasql.query(token)
		else
			token = "INSERT INTO `CSAinfo` ( "
							.. "`cdkey` ,"
							.. "`saveindex` ,"
							.. "`name` ,"
							.. "`famename` ,"
							.. "`level` ,"
							.. "`trans` ,"
							.. "`alive` ,"
							.. "`fame` ,"
							.. "`TITLE_LV` ,"
							.. "`DUELBATTLECOUNT` ,"
							.. "`DUELWINCOUNT` ,"
							.. "`DUELLOSECOUNT` ,"
							.. "`DUELSTWINCOUNT` ,"
							.. "`DUELSTLOSECOUNT` ,"
							.. "`DUELMAXSTWINCOUNT` ,"
							.. "`vip` ,"
							.. "`time`"
							.. ")"
							.. "VALUES ("
							.. "'" .. char.getChar(charindex, "�˺�")
							.. "', '" .. char.getInt(charindex, "�浵")
							.. "', '" .. char.getChar(charindex, "����")
							.. "', '"  .. char.getChar(charindex, "����")
							.. "', '"  .. char.getInt(charindex, "�ȼ�")
							.. "', '"  .. char.getInt(charindex, "ת��")
							.. "', '"  .. char.getInt(charindex, "����")
							.. "', '"  .. char.getInt(charindex, "����")
							.. "', '"  .. char.getInt(charindex, "Ӣ�۳ƺ�")
							.. "', '"  .. char.getInt(charindex, "PK����")
							.. "', '"  .. char.getInt(charindex, "PKӮ��")
							.. "', '"  .. char.getInt(charindex, "PK����")
							.. "', '"  .. char.getInt(charindex, "PK��ʤ")
							.. "', '"  .. char.getInt(charindex, "PK����")
							.. "', '"  .. char.getInt(charindex, "PK�����ʤ")
							.. "', '"  .. char.getInt(charindex, "��Ա")
							.. "', NOW()"
							.. ");"

			ret = sasql.query(token)
		end
	end
	token = "update `CSAlogin` set `Offline`=0 where `Name`='" .. char.getChar(charindex,"�˺�") .. "'"
	sasql.query(token)
	
end

function data()

end

function main()
	data()
end
