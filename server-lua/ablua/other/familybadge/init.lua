local �����������={};
local ��������="";



function �ַ����и�(����,�и��ַ�)
	���� = ���� .. �и��ַ�
	local t = {}
	local Ŀǰλ�� = 1
	repeat
			local nexti = string.find(����, �и��ַ�, Ŀǰλ��)
			table.insert(t, string.sub(����, Ŀǰλ��,nexti-string.len(�и��ַ�)))
			Ŀǰλ�� = nexti + string.len(�и��ַ�)
	until Ŀǰλ�� > string.len(����)
	return t
end


function LoadFamilyBadge(fd)
	lssproto.FamilyBadge(fd,��������);
end

function ��ȡ�����������()
	local �ļ���;
	local ����,����;
	�ļ��� = assert(io.open("./data/ablua/other/familybadge/data.txt", "r"))
	���� = �ļ���:read("*a"); -- ��ȡ��������
	�ļ���:close();
	�������� = �����¼۸�.."|"
	if ���� ~= "" then
		���� = 0;
		���� = �ַ����и�(����,"\n");
		for b = 1,table.getn(����) do 
			if string.sub(����[b],1,1) ~= "#" then
				���� = ���� + 1;
				�����������[����] = tonumber(����[b]);
			end
		end
		for b=1,table.getn(�����������) do 
			if b~=table.getn(�����������) then
				�������� = �������� .. �����������[b].."|";
			else
				�������� = �������� .. �����������[b];
			end
		end
	end
end

function main()
	�����¼۸�=1000;
	��ȡ�����������();
	lssproto.GetFBData(�����������,�����¼۸�);
end
main();

