function EquipEffectFunction( charaindex, id )
	if id == 2 then

		itemindex = char.getItemIndex(charaindex,id)
		if itemindex > -1 then
			local itemlv = GetItemShowIndex(itemindex)
			if itemlv>0 then
				char.setWorkInt(charaindex,"ׯ԰��Ч",itemshow[itemlv][2])
				return 1
			end
		else
			char.setWorkInt(charaindex,"ׯ԰��Ч",0)
		end
	end
	if id == 6 then
		itemindex = char.getItemIndex(charaindex,id)
		if itemindex > -1 then
			local data = item.getChar(itemindex, "�ֶ�")
			local effectid = other.atoi(other.getString(data, "|", 4))
			char.setInt(charaindex,"����������Ч",effectid)
		else
			char.setInt(charaindex,"����������Ч",0)
		end
	end
	return 1

end

function GetItemShowIndex(itemindex)
	local name = item.getChar(itemindex,"����")
	for i = 1,table.getn(itemshow) do
		local tagindex = string.find(name,itemshow[i][1])
		if tagindex ~= nil then
			return i
		end
	end
	return 0
end


function data()
	itemshow = {
				 {"����", 102201}
				,{" 14", 102203}
				,{" 15", 102220}
				,{" 16", 102223}
				,{" 17", 102219}
				,{" 18",102232}
				,{" 19",102248}
				,{" 20",102234}
				}

end

function main()
	data()
end
