function �ű�����(·��)
	return dofile(·��);
end

function ������()
	�ű�����("./data/lua/onlineshop/main.lua");
	--ʱ������
	math.randomseed(os.time());
end

������()

