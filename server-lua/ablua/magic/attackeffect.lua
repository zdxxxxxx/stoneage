function ������Ч(charaindex, data)
	local ��Ч����ID = other.atoi(other.getString(data, " ", 2));
	char.setWorkInt(char.getCharPet(charaindex,0),"������Ч",��Ч����ID);
	--char.setWorkInt(charaindex,"������Ч",��Ч����ID);
end

function main()
	magic.addLUAListFunction("������Ч", "������Ч", "", 1, "[gm ������Ч �������]")
end
