--��¼���̶�ȡ����֤
function FreeLoginCheck(fd)
	local mun = 0
	local maxnum = 0
	local maxplayer = config.getFdnum() - 1
	local mac = net.getMac(fd)
	local ip = net.getIP(fd)

	if mac == "" then
		lssproto.CharList(fd, "������ʹ�÷Ǳ������Ͽɵĳ��������Ϸ�����Խ�ֹ��ĵ�½��")
		return 0
	end
	if string.len(mac) < 32 then
		lssproto.CharList(fd, "������ʹ�÷Ǳ������Ͽɵĳ��������Ϸ�����Խ�ֹ��ĵ�½��")
		return 0
	end
	local key = other.getString(mac, "-", 1)
	if key ~= "ver100" then
		lssproto.CharList(fd, "������İ汾���ɣ�����º��ٽ�����Ϸ��")
		return 0
	end

	local mainmac = other.getString(mac, "-", 2)

	maxnum = 2
	for i = 5, maxplayer do
		if net.getUse(i) == 1 then
			if mac == net.getMac(i) then
				mun = mun + 1
				if mun > maxnum then
					lssproto.CharList(fd, "�����Ϸ�˺ŵ�½���Դ�" .. mun - 1 .. "�����벻Ҫ����������Ϸ��")
					return 0
				end
			end
		end
	end

	token = "SELECT count(*) FROM `CSAlogin` "
					.. " WHERE `MAC` = '" .. mac .. "' and `Online` > 0"
	ret = sasql.query(token)
	local totalnum = 1
	if ret == 1 then
		sasql.free_result()
		sasql.store_result()
		num = sasql.num_rows()
		if num > 0 then
			sasql.fetch_row(0)
			onlinenum = other.atoi(sasql.data(1))
			if onlinenum > totalnum then
				lssproto.CharList(fd, "������Ϸ��¼�˺������Ѵ����ޣ��벻Ҫ����������Ϸ��")
				return 0
			end
		end
	end

	return 1
end

function CheckNologin(mac)
	for j=1,table.getn(nologin) do
		if mac==nologin[j] then
			return 1
		end
	end
	return 0
end

function data()


end

function main()
	data()
end
