function NetLoopFunction()
	if minute > 0 then
		if math.mod(looptime , minute) == 0 then
			char.talkToAllServer("[��ܰ��ʾ]" .. message)
		end

		looptime = looptime + 1
	end
end

function announce(charaindex, data)
	message = other.getString(data, " ", 1)
	char.talkToAllServer("[��ܰ��ʾ]" .. message)
	minute = other.atoi(other.getString(data, " ", 2))
	looptime = 1
end

function main()
	minute = 0
	magic.addLUAListFunction("����", "announce", "", 1, "[���� ���� ���(����)")
end

